#include <stdio.h>
#include <stdlib.h>

#define TRI_STATE_ALL 214

int pwm_gpio_ed[4]={103,105,106,109};
int pwm_gpio_l[4]={12,13,182,183};
int pwm_oe_l[4]={251,253,254,257};
int pwm_pullup_l[4] = {219,221,222,225};

void set_pwm(int pwm, unsigned long t_high, unsigned long period)
{
  char *path;
  FILE *fptr;
  // Export the gpios for the pwm io, gpio pullup, & TRI_STATE_ALL
  // Assume the gpios are already exported for now

  //TRISTATE all the pins by writing into the TRI_STATE_ALL
  asprintf(&path,"/sys/class/gpio/gpio%d/direction",TRI_STATE_ALL);
  printf("PATH TO TRISTATE\n%s\n",path);
  fptr = fopen(path,"w");
  fprintf(fptr, "low");
  fclose(fptr);
  free(path);

  //Set the gpio for the corresponding pwm as output
  asprintf(&path,"/sys/class/gpio/gpio%d/direction",pwm_oe_l[pwm]);
  printf("PATH TO ENABLE GPIO as OUTPUT\n%s\n",path);
  fptr = fopen(path,"w");
  fprintf(fptr, "high");
  fclose(fptr);
  free(path);

  //Set the corresponding pin as HighZ input to disable pull-up resister
  asprintf(&path,"/sys/class/gpio/gpio%d/direction",pwm_pullup_l[pwm]);
  printf("PATH TO set HighZ input\n%s\n",path);
  fptr = fopen(path,"w");
  fprintf(fptr, "in");
  fclose(fptr);
  free(path);

  // Set the gpio pin to mode1 to enable it as pwm instead of gpio
  asprintf(&path,"/sys/kernel/debug/gpio_debug/gpio%d/current_pinmux",pwm_gpio_l[pwm]);
  printf("PATH TO set pinmux\n%s\n",path);
  fptr = fopen(path,"w");
  fprintf(fptr, "mode1");
  fclose(fptr);
  free(path);

  // Disable TRISTATing the pins
  asprintf(&path,"/sys/class/gpio/gpio%d/direction",TRI_STATE_ALL);
  printf("PATH TO DISABLE TRISTATE\n%s\n",path);
  fptr = fopen(path,"w");
  fprintf(fptr, "high");
  fclose(fptr);
  free(path);

  //Check if pwm channel has been 'exported'
  // Assume pwm channel has been exported

  // Disable pwm channel first to avoid gitter
  asprintf(&path,"/sys/class/pwm/pwmchip0/pwm%d/enable",pwm);
  printf("PATH TO set pwm duty-cycle\n%s\n",path);
  fptr = fopen(path,"w");
  fprintf(fptr, "0");
  fclose(fptr);
  free(path);

  // Write the Thigh as the duty cycle
  asprintf(&path,"/sys/class/pwm/pwmchip0/pwm%d/duty_cycle",pwm);
  printf("PATH TO set pwm duty-cycle\n%s\n",path);
  fptr = fopen(path,"w");
  fprintf(fptr, "%d",t_high);
  fclose(fptr);
  free(path);

  //Enable pwm channel
  asprintf(&path,"/sys/class/pwm/pwmchip0/pwm%d/enable",pwm);
  printf("PATH TO set pwm duty-cycle\n%s\n",path);
  fptr = fopen(path,"w");
  fprintf(fptr, "1");
  fclose(fptr);
  free(path);



  
}
void main(int argc, char *argv[])
{
  int duty_cycle;
  unsigned long period, Thigh;
  int pwm;
  if (argc == 4)
  {
    //printf("pwm signal requested\n");
    //printf("pwm channel: %s\n",argv[1]);
    //printf("Duty cycle: %s%\n",argv[2]);
    //printf("Period    : %snSec\n",argv[3]);
  } else {
    printf("Illegal arguments passed\n");
    return;
  }
  pwm = strtol(argv[1],NULL,10);
  duty_cycle = strtol(argv[2],NULL,10);
  period     = strtol(argv[3],NULL,10);
  Thigh      = (period/100)*duty_cycle;

  printf("PWM channel sel : %d\n",pwm);
  printf("Calculated Thigh: %lu\n",Thigh);
  printf("Total Period    : %lu nSec\n",period);

  set_pwm(pwm,Thigh,period);
}
