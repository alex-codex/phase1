#include <Servo.h>
#define LDR A1
#define moove 2

void attendre(int duree)
{
  int start=millis();
  while(millis()-start < duree)
  {
  }
}  
unsigned int value;
Servo porte;
Servo fenetre;
int pirVal;
void setup()
{
  porte.attach(8);
  porte.write(0);
  Serial.begin(9600);
  pinMode(LDR, INPUT);
  pinMode(9,OUTPUT);
  pinMode(moove , INPUT);
}

void loop()
{
  pirVal=digitalRead(moove);
  if(pirVal==HIGH)
  {
    porte.write(130);
    //attendre(1300);
  }
  else
  {
    porte.write(0);    
  }  
    
  value=analogRead(LDR);
  //value=map(value,0,1023,0,255);
  if(value < 525)
  {
    digitalWrite(9,LOW);
    //porte.write(120);
  }
  else
  {
    //porte.write(0);
    digitalWrite(9,HIGH);
  }
  //analogWrite(9,value);
}