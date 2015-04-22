#include <Servo.h> 
Servo myservo;

void setup()
{
  // put your setup code here, to run once:
  myservo.attach(PB_5);
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);
  attachInterrupt(PUSH2, left, FALLING);
  attachInterrupt(PUSH1, right, FALLING);
}
int dir=90;
int delta = 1;
void left(){
  
}
void right(){
  
}
void loop()
{
//  if(!digitalRead(PUSH1))dir += delta;
//  if(!digitalRead(PUSH2))dir -= delta;
//  if(dir<0)dir=180;
//  if(dir>180)dir=0;
//  myservo.write(dir);
//  delay(10);

  int i;
  for(i=0;i<60;i++){
    
  myservo.write(150-i*2);
  delay(1000);
  }
}
