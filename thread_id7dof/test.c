#include "orocos_cnc.h"
#include <stdio.h>

struct momentType moment;

int main(void)
{
  moment = inverse_dynamic(2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5);
  printf("Moment 1: %f\n", moment.mm1);
  printf("Moment 2: %f\n", moment.mm2);
  printf("Moment 3: %f\n", moment.mm3);
  printf("Moment 4: %f\n", moment.mm4);
  printf("Moment 5: %f\n", moment.mm5);
  printf("Moment 6: %f\n", moment.mm6);
}
