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
#include "orocos_cnc.h"

//Slave 0 XMC4300
#define BusCouplerPos    0, 0
#define XMC_ESC 0x0000034e, 0x00000000

struct momentType moment;

int slave_working = 0;
uint32_t error_num = 0;

/****************************************************************************/
// Application parameters
#define FREQUENCY 4000
#define CLOCK_TO_USE CLOCK_REALTIME
#define NUMBER_OF_SAMPLE 1000000

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

/****************************************************************************/

// process data
static uint8_t *domain1_pd = NULL;

// offsets for PDO entries
static int off_dig_out;
static int off_counter_in;

static unsigned int counter = FREQUENCY;
static unsigned int blink = 0;
static unsigned int sync_ref_counter = 0;
const struct timespec cycletime = {0, PERIOD_NS};
