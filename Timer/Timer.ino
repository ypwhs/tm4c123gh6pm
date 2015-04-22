#include <stdint.h>
#include <stdbool.h>

#define PART_TM4C123GH6PM
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/pwm.h"


int p = BLUE_LED;

long i = 0;
int ci = 0;
void lala(){
  ci ++;
  if(ci>=25){
  Serial.print(i);
  Serial.print("\t");
  Serial.println(millis());
  ci=0;
  }
}

void setup()
{
  //SysCtlClockSet(SYSCTL_SYSDIV_64 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  pinMode(p, OUTPUT);
  pinMode(PD_0, INPUT);
  Serial.begin(115200);
  SysCtlClockSet(SYSCTL_SYSDIV_2_5 |SYSCTL_USE_PLL|SYSCTL_OSC_MAIN |SYSCTL_XTAL_16MHZ);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);//使能PWM0模块
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//使能PWM0和PWM1输出所在GPIO
  GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
  GPIOPinConfigure(GPIO_PB6_M0PWM0);    //#define GPIO_PB6_M0PWM0         0x00011804
  SysCtlPWMClockSet(SYSCTL_PWMDIV_64);     // PWM分频
  //配置PWM发生器0：加减计数，不同步
  PWMGenConfigure(PWM0_BASE,PWM_GEN_0,PWM_GEN_MODE_UP_DOWN| PWM_GEN_MODE_NO_SYNC);
  //设置PWM发生器1的频率，时钟频率/PWM分频数/n，80M/1/800=100KHZ
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 50000);
  //设置PWM0/PWM1输出的脉冲宽度
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 20);//50%占空比
  //使能PWM0和PWM1的输出
  PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
  //使能PWM发生器
  PWMGenEnable(PWM0_BASE, PWM_GEN_0);

  attachInterrupt(PB_6, lala, FALLING);
  Serial.println("begin");
}
void loop()
{
  i++;
}

