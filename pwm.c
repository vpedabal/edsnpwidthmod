#include <stdio.h>

void main(int argc, char *argv[])
{
  int duty_cycle;
  unsigned long period, Thigh;
  if (argc == 3)
  {
    printf("pwm signal requested\n");
    printf("Duty cycle: %s%\n",argv[1]);
    printf("Period    : %snSec\n",argv[2]);
  } else {
    printf("Illegal arguments passed\n");
    return;
  }
  duty_cycle = strtol(argv[1],NULL,10);
  period     = strtol(argv[2],NULL,10);
  Thigh      = (period/100)*duty_cycle;

  printf("Calculated Thigh: %lu\n",Thigh);
  printf("Total Period    : %lu\n",period);

}
