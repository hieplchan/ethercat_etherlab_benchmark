#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <malloc.h>
#include <pthread.h>
#include <sched.h>

/****************************************************************************/

#include "ecrt.h"

/****************************************************************************/

// Application parameters
#define FREQUENCY 4000
#define CLOCK_TO_USE CLOCK_REALTIME
#define NUMBER_OF_SAMPLE 1000000

/****************************************************************************/

#define NSEC_PER_SEC (1000000000L)
#define PERIOD_NS (NSEC_PER_SEC / FREQUENCY)

//Time differrent between 2 timestamp
#define DIFF_NS(A, B) (((B).tv_sec - (A).tv_sec) * NSEC_PER_SEC + (B).tv_nsec - (A).tv_nsec)

//Time to ns
#define TIMESPEC2NS(T) ((uint64_t) (T).tv_sec * NSEC_PER_SEC + (T).tv_nsec)

/****************************************************************************/

// EtherCAT
static ec_master_t *master = NULL;
static ec_master_state_t master_state = {};

static ec_domain_t *domain1 = NULL;
static ec_domain_state_t domain1_state = {};

int slave_working = 0;
uint32_t error_num = 0;

/****************************************************************************/

// process data
static uint8_t *domain1_pd = NULL;

#define BusCouplerPos    0, 0

#define XMC_ESC 0x0000034e, 0x00000000

// offsets for PDO entries
static int off_dig_out;
static int off_counter_in;

static unsigned int counter = FREQUENCY;
static unsigned int blink = 0;
static unsigned int sync_ref_counter = 0;
const struct timespec cycletime = {0, PERIOD_NS};

/*****************************************************************************/
//Cong 2 gia tri time
struct timespec timespec_add(struct timespec time1, struct timespec time2)
{
  struct timespec result;

  if ((time1.tv_nsec + time2.tv_nsec) >= NSEC_PER_SEC) {
    result.tv_sec = time1.tv_sec + time2.tv_sec + 1;
    result.tv_nsec = time1.tv_nsec + time2.tv_nsec - NSEC_PER_SEC;
  } else {
    result.tv_sec = time1.tv_sec + time2.tv_sec;
    result.tv_nsec = time1.tv_nsec + time2.tv_nsec;
  }

  return result;
}

/*****************************************************************************/

void check_domain1_state(void)
{
  ec_domain_state_t ds;

  ecrt_domain_state(domain1, &ds);

  if (ds.working_counter != domain1_state.working_counter)
  {
    //printf("Domain1 state change to: WC %u.\n", ds.working_counter);
    if (slave_working) error_num++;
  }
  if (ds.wc_state != domain1_state.wc_state)
  {
    if (slave_working) error_num++;
    //printf("Domain1 state change to: State %u.\n", ds.wc_state);
  }
  if (ds.wc_state == 2)
  {
    slave_working = 1;
  }

  domain1_state = ds;
}

/*****************************************************************************/

void check_master_state(void)
{
  ec_master_state_t ms;

  ecrt_master_state(master, &ms);

  if (ms.slaves_responding != master_state.slaves_responding)
  {
    if (slave_working) error_num++;
    //printf("Found %u slave(s).\n", ms.slaves_responding);
  }

  if (ms.al_states != master_state.al_states)
  {
    if (slave_working) error_num++;
    //printf("AL states: 0x%02X.\n", ms.al_states);
  }
  if (ms.link_up != master_state.link_up)
  {
    if (slave_working) error_num++;
    //printf("Link is %s.\n", ms.link_up ? "up" : "down");
  }

  master_state = ms;
}

/****************************************************************************/

void *cyclic_task()
{
  struct timespec wakeupTime, time;
  struct timespec startTime, endTime, lastStartTime = {};
  uint32_t sample_no = 0;
  uint32_t period_ns = 0, exec_ns = 0, latency_ns = 0;

  // get current time
  clock_gettime(CLOCK_TO_USE, &wakeupTime);

  while(sample_no < NUMBER_OF_SAMPLE)
  {
    wakeupTime = timespec_add(wakeupTime, cycletime);
    clock_nanosleep(CLOCK_TO_USE, TIMER_ABSTIME, &wakeupTime, NULL);

    clock_gettime(CLOCK_TO_USE, &startTime);
    latency_ns = DIFF_NS(wakeupTime, startTime);
    period_ns = DIFF_NS(lastStartTime, startTime);
    exec_ns = DIFF_NS(lastStartTime, endTime);
    lastStartTime = startTime;

    // receive process data
    ecrt_master_receive(master);
    ecrt_domain_process(domain1);

    // check process data state (optional)
    check_domain1_state();
    check_master_state();

    if (slave_working)
    {
      // output timing stats
      sample_no++;
      printf("%u,", sample_no);
      printf("%u,", period_ns);
      printf("%u,", exec_ns);
      printf("%u,", latency_ns);
      printf("%u\n", error_num);
    }

    if (counter)
    {
      counter--;
    }
    else
    { // do this at 1 Hz
      counter = FREQUENCY;
      // check for master state (optional)

      // calculate new process data
      blink = !blink;
    }

    // write process data
    EC_WRITE_U8(domain1_pd + off_dig_out, blink ? 0x00 : 0xFF);

    // write application time to master
    clock_gettime(CLOCK_TO_USE, &time);
    ecrt_master_application_time(master, TIMESPEC2NS(time));

    if (sync_ref_counter)
    {
      sync_ref_counter--;
    }
    else
    {
      sync_ref_counter = 1; // sync every cycle
      ecrt_master_sync_reference_clock(master);
    }

    ecrt_master_sync_slave_clocks(master);

    // send process data
    ecrt_domain_queue(domain1);
    ecrt_master_send(master);

    clock_gettime(CLOCK_TO_USE, &endTime);
  }
}

/****************************************************************************/

int main(int argc, char **argv)
{
  ec_slave_config_t *sc;

  master = ecrt_request_master(0);
  if (!master) return -1;

  domain1 = ecrt_master_create_domain(master);
  if (!domain1) return -1;

  // Create configuration for XMC_ESC(alias, position, vendor, product)
  sc = ecrt_master_slave_config(master, BusCouplerPos, XMC_ESC);
  if (!sc) return -1;

  off_dig_out = ecrt_slave_config_reg_pdo_entry(sc, 0x7000, 0x16, domain1, NULL);
  if (off_dig_out < 0) return -1;

  // configure SYNC signals for this slave
  ecrt_slave_config_dc(sc, 0x0700, PERIOD_NS, 4400000, 0, 0);

  //printf("Activating master...\n");
  if (ecrt_master_activate(master)) return -1;

  if (!(domain1_pd = ecrt_domain_data(domain1)))
  {
    return -1;
  }
  ////////// THREAD ////////

  struct sched_param param;
  pthread_attr_t attr;
  pthread_t thread;
  int ret;

  //Lock all memory of current process
  if(mlockall(MCL_CURRENT) == -1) {
    printf("mlockall failed: \n");
    exit(-2);
  }

  /* Initialize pthread attributes (default values) */
  ret = pthread_attr_init(&attr);
  if (ret) {
    printf("init pthread attributes failed\n");
    goto out;
  }

  /* Set a specific stack size (can be modified for different application)  */
  ret = pthread_attr_setstacksize(&attr, 1000000);
  if (ret) {
    printf("pthread setstacksize failed\n");
    goto out;
  }

  /* Set scheduler policy and priority of pthread */
  ret = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  if (ret) {
    printf("pthread setschedpolicy failed\n");
    goto out;
  }

  param.sched_priority = 99;
  ret = pthread_attr_setschedparam(&attr, &param);
  if (ret) {
    printf("pthread setschedparam failed\n");
    goto out;
  }

  /* Use scheduling parameters of attr */
  ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
  if (ret) {
    printf("pthread setinheritsched failed\n");
    goto out;
  }

  /* Create a pthread with specified attributes */
  ret = pthread_create(&thread, &attr, cyclic_task, NULL);
  if (ret) {
    printf("create pthread failed\n");
    goto out;
  }

  /* Join the thread and wait until it is done */
  ret = pthread_join(thread, NULL);
  if (ret)
    printf("join pthread failed:\n");

out:

  return 0;
}

/****************************************************************************/
