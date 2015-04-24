void setup()
{
  // put your setup code here, to run once:
  pinMode(PB_0,INPUT);
  attachInterrupt(PB_0, add, RISING);
  Serial.begin(9600);
}
long a = 0,last=0;
void add(){
  a++;
}
void loop()
{
  // put your main code here, to run repeatedly:
  Serial.println(a-last);
  last=a;
  delay(100);
}
