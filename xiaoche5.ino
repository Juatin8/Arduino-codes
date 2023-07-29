#include<Servo.h>

int echoPin=11;
int trigPin=13;
 float pingtime;
 float Fdistance;
 float Ldistance;
 float Rdistance;
  float speedofsound=340.00;

Servo myservo;

void setup(){
  Serial.begin(9600);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
   pinMode(7,OUTPUT);
     pinMode(8,OUTPUT);
     pinMode(10,OUTPUT);
     pinMode(12,OUTPUT);
     pinMode(trigPin,OUTPUT);
     pinMode(echoPin,INPUT);
        pinMode(16,OUTPUT);
     pinMode(17,OUTPUT);
       pinMode(18,OUTPUT);
     pinMode(19,OUTPUT);
      myservo.attach(9);
}

void advance(int timee)
{
  digitalWrite(3,HIGH);
    digitalWrite(4,LOW);
     analogWrite(19,255);
      digitalWrite(5,LOW);
digitalWrite(6,HIGH);
 analogWrite(18,255);
 digitalWrite(7,LOW);
  digitalWrite(8,HIGH);
  analogWrite(17,255);
   digitalWrite(10,LOW);
digitalWrite(12,HIGH);
analogWrite(16,255);
delay(timee*100);
}
void back(int timee){
   digitalWrite(3,LOW);
    digitalWrite(4,HIGH);
      analogWrite(19,255);
      digitalWrite(5,HIGH);
digitalWrite(6,LOW);
 analogWrite(18,255);
 digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
    analogWrite(17,255);
   digitalWrite(10,HIGH);
digitalWrite(12,LOW);
analogWrite(16,255);
delay(timee*100);
}
void stopp(int timee){
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
      digitalWrite(5,LOW);
digitalWrite(6,LOW);
 digitalWrite(7,LOW);
  digitalWrite(8,LOW);
   digitalWrite(10,LOW);
digitalWrite(12,LOW);
delay(timee*100);
}
void turnL(int timee){
  digitalWrite(3,HIGH);
    digitalWrite(4,LOW);
     analogWrite(19,255);
      digitalWrite(5,HIGH);
digitalWrite(6,LOW);
 analogWrite(18,255);
 digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
  analogWrite(17,255);
   digitalWrite(10,LOW);
digitalWrite(12,HIGH);
analogWrite(16,255);
delay(timee*100);
}
void turnR(int timee){
 digitalWrite(3,LOW);
    digitalWrite(4,HIGH);
     analogWrite(19,255);
      digitalWrite(5,LOW);
digitalWrite(6,HIGH);
 analogWrite(18,255);
 digitalWrite(7,LOW);
  digitalWrite(8,HIGH);
  analogWrite(17,255);
   digitalWrite(10,HIGH);
digitalWrite(12,LOW);
analogWrite(16,255);
delay(timee*100);
}

void ask_pin_F()
{
  myservo.write(80);
 digitalWrite(trigPin,LOW);
  delayMicroseconds(2000);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin,LOW);
  pingtime=pulseIn(echoPin,HIGH);
  Fdistance=speedofsound*pingtime/20000;
  Serial.print("Fdistance");
  Serial.println(Fdistance);
}
void ask_pin_L()
{
  myservo.write(5);
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2000);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin,LOW);
  pingtime=pulseIn(echoPin,HIGH);
  Ldistance=speedofsound*pingtime/20000;
  Serial.print("Ldistance");
  Serial.println(Ldistance);
}

void ask_pin_R()
{
  myservo.write(177);
 digitalWrite(trigPin,LOW);
  delayMicroseconds(2000);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin,LOW);
  pingtime=pulseIn(echoPin,HIGH);
  Rdistance=speedofsound*pingtime/20000;
  Serial.print("Rdistance");
  Serial.println(Rdistance);
}

 

void loop(){
  myservo.write(80);
  ask_pin_F();
  if(Fdistance<10){
    stopp(1);
    back(2);
  }
  if(Fdistance<25)
 {stopp(3);
  ask_pin_L();
  delay(200);
  ask_pin_R();
  delay(200);
  if((Ldistance<25)&&(Rdistance<25)){
    back(8);
    turnL(1);
    Serial.print("BACK");
    Serial.println(Fdistance);}
 else if(Ldistance>Rdistance)
  { back(1);
    turnL(12);
    Serial.print("turnL");}
  else
  { back(1);
    turnR(12);
    Serial.print("turnR");}
 }
  else
  {  advance(1);
  Serial.print("advance");}                                                                        
 
}
