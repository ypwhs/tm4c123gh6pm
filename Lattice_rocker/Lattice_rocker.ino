#define DIN PD_0
#define CS PD_1
#define CLK PD_2

#define VRX PE_1
#define VRY PE_2
#define SW PE_3

#define LED1 RED_LED
#define LED2 GREEN_LED
#define LED3 BLUE_LED
#define SW1 PUSH2
#define SW2 PUSH1

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"


char num[12][12] = {
{0x70,0x88,0x88,0x88,0x88,0x88,0x88,0x70},/*"0",0*/
{0x20,0x60,0xA0,0x20,0x20,0x20,0x20,0xF8},/*"1",1*/
{0x70,0x88,0x88,0x10,0x20,0x40,0x80,0xF8},/*"2",2*/
{0x70,0x88,0x08,0x30,0x08,0x08,0x88,0x70},/*"3",3*/
{0x10,0x30,0x50,0x50,0x90,0x78,0x10,0x38},/*"4",4*/
{0xF8,0x80,0x80,0xF0,0x08,0x08,0x88,0x70},/*"5",5*/
{0x70,0x88,0x80,0xF0,0x88,0x88,0x88,0x70},/*"6",6*/
{0xF8,0x88,0x10,0x20,0x20,0x20,0x20,0x20},/*"7",7*/
{0x70,0x88,0x88,0x70,0x88,0x88,0x88,0x70},/*"8",8*/
{0x70,0x88,0x88,0x88,0x78,0x08,0x88,0x70},/*"9",9*/
{0x10,0xFE,0x9A,0x92,0xFE,0x92,0x10,0x10},//中
{0x10,0xFF,0x44,0x4C,0x28,0x30,0x3C,0xC7},//文
};

void setup() {
  //SysCtlClockSet(SYSCTL_SYSDIV_64 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  //2.5分频，使用PLL，外部晶振16M，system时钟源选择 main osc。
  //系统时钟200MHZ/2.5=80MHZ
  pinMode(DIN, OUTPUT); 
  pinMode(CS, OUTPUT); 
  pinMode(CLK, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(SW1, INPUT_PULLUP); 
  pinMode(SW2, INPUT_PULLUP);
  
  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);
  pinMode(SW, INPUT);
  
  pinMode(PD_3, OUTPUT);
  digitalWrite(PD_3, HIGH);
  pinMode(PF_1, OUTPUT);
  digitalWrite(PB_5, LOW);
  
  Serial.begin(9600);
  init_max7219();
}



void init_max7219()
{
  w_max7219(0x0a,0x01);  //显示亮度，取值范围0~f
  w_max7219(0x0b,0x07);  //8位扫描显示，取值范围0x01~0x07
  w_max7219(0x09,0x00);  //译码方式，0x00为不译码，0xff为译码
  w_max7219(0x0c,0x01);  //操作方式，0x00为低功耗模式，0x01为正常操作模式
  w_max7219(0x0f,0x00);  //显示状态，0x00为正常显示，0x01为显示测试
}

void w_max7219(unsigned char addr,unsigned char wdata)
{
  digitalWrite(CS, 0);
  shiftOut(DIN, CLK, MSBFIRST, addr);
  shiftOut(DIN, CLK, MSBFIRST, wdata);
  digitalWrite(CS, 1);
}

int button = 0;
int lightPin = 0;
int disp = 0;
int pianx = 0;
int piany = 0;
void loop() {

  int vx = analogRead(VRX);
  int vy = analogRead(VRY);
  int sw = analogRead(SW);
  
  if(vx<1000|vx>3000){
    if(button){
      lightPin = LED2;
      if(vx<1000)pianx--;
      else pianx++;
    }
    button=0;
  }
  
  if(vy<1000|vy>3000){
    if(button){
      lightPin = LED3;
      if(vy<1000)piany++;
      else piany--;
    }
    button=0;
  }
  
  if(sw<500){
    if(button){
      lightPin = LED2;
      pianx=0;
      piany=0;
    }
    button=0;
  }
  
  if(!digitalRead(SW1)){
    if(button){
      lightPin = LED2;
      disp++;
      if(disp>=12)disp=0;
    }
    button=0;
  }

  if(!digitalRead(SW2)){
    if(button){
      lightPin = LED3;
      disp--;
      if(disp<0)disp=11;
    }
    button=0;
  }
  
  

  if(lightPin){
    digitalWrite(lightPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(lightPin, LOW);
    lightPin = 0;
  }

  if(digitalRead(SW1) && digitalRead(SW2) && 
    (vx>1000&&vx<3000)&&(vy>1000&&vy<3000)&&(sw>500))button=1;

  int i=0;
  for(i=0;i<8;i++){
//    if(disp<10)w_max7219(i+1, num[disp][i]>>1);else 
    if(pianx>=0){
      if(i+piany>=0&&i+piany<8)w_max7219(i+1, num[disp][i+piany]>>pianx);
      else w_max7219(i+1, 0);
    }else{
      if(i+piany>=0&&i+piany<8)w_max7219(i+1, num[disp][i+piany]<<-pianx);
      else w_max7219(i+1, 0);
    }
  }
  Serial.println(piany);
}

