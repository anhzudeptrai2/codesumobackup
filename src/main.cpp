#include <Arduino.h>
int RPWM1 = 7;
int LPWM1 = 6;
int RPWM = 8;
int LPWM = 9;
int IRR = 2;
int IRL = 3;
int IRB = 22;
int trig1 = 4; // SR trướcttttt
int echo1 = 5;
int trig2 = 12; // SR phải
int echo2 = 13;
int trig3 = 10; // SR trái
int echo3 = 11;
int valIRR;
int valIRL;
int valIRB;
unsigned long duration1, duration2, duration3;
int scan = 0;
int distance1;
int distance2;
int distance3;

void FORWARD()
{
  analogWrite(LPWM, 255);
  analogWrite(LPWM1, 255);
  analogWrite(RPWM, 0);
  analogWrite(RPWM1, 0);
}
void BACKWARD()
{
  analogWrite(LPWM, 0);
  analogWrite(LPWM1, 0);
  analogWrite(RPWM, 155);
  analogWrite(RPWM1, 155);
}

void STOP()
{
  analogWrite(LPWM, 0);
  analogWrite(RPWM, 0);
  analogWrite(LPWM1, 0);
  analogWrite(RPWM1, 0);
}

void ROTATE()
{
  analogWrite(LPWM, 100);
  analogWrite(RPWM, 0);
  analogWrite(LPWM1, 0);
  analogWrite(RPWM1, 100);
}
void RBACKWARD()
{
  analogWrite(RPWM, 0);
  analogWrite(LPWM, 100);
  analogWrite(RPWM1, 0);
  analogWrite(LPWM1, 50);
}
void LBACKWARD()
{
  analogWrite(RPWM, 0);
  analogWrite(RPWM1, 0);
  analogWrite(LPWM, 50);
  analogWrite(LPWM1, 100);
}
void RFORWARD()
{
  analogWrite(RPWM, 20);
  analogWrite(RPWM1, 0);
  analogWrite(LPWM, 120);
  analogWrite(LPWM1, 0);
}
void LFORWARD()
{
  analogWrite(RPWM, 0);
  analogWrite(RPWM1, 20);
  analogWrite(LPWM, 0);
  analogWrite(LPWM1, 120);
}
void PUSH()
{
  analogWrite(LPWM, 255);
  analogWrite(RPWM, 0);
  analogWrite(LPWM1, 255);
  analogWrite(RPWM1, 0);
}

void proval()
{
  valIRR = digitalRead(IRR);
  valIRL = digitalRead(IRL);
  valIRB = digitalRead(IRB);

  if (valIRR == 0)
  {
    STOP();
    delay(100);
    BACKWARD();
    delay(500);
  }
  if (valIRR == 0)
  {
    STOP();
    delay(100);
    BACKWARD();
    delay(500);
  }
  else if (valIRB == 0)
  {
    STOP();
    delay(100);
    FORWARD();
    delay(500);
  }
}

void search()
{
  if (distance1 <= 35)
  {
    Serial.println("pushF");
    PUSH();
    delay(500);
  }
  else if (distance2 <= 20)
  {
    Serial.println("pushR");
    RFORWARD();
    delay(100);
  }
  else if (distance3 <= 20)
  {
    Serial.println("pushL");
    LFORWARD();
    delay(100);
  }
  else
  {
    if (scan < 3)
    {
      ROTATE();
    }
    else if (scan <= 5)
    {
      ROTATE();
    }
    if (scan < 5)
    {
      scan += 1;
    }
    else if (scan == 5)
    {
      scan = 0;
    }
  }
}

void setup()
{
  Serial.begin(9600);
  delay(1000);
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(IRR, INPUT);
  pinMode(IRL, INPUT);
  pinMode(IRB, INPUT);
  pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  pinMode(echo3, INPUT);
  attachInterrupt(0, proval, CHANGE);
  attachInterrupt(1, proval, CHANGE);
  attachInterrupt(3, proval, CHANGE);
}

void loop()
{
  // IR
  valIRR = digitalRead(IRR);
  Serial.print("IR phai: ");
  Serial.println(valIRL);
  valIRL = digitalRead(IRL);
  Serial.print("IR trai: ");
  Serial.println(valIRL);
  valIRB = digitalRead(IRB);
  Serial.print("IR sau: ");
  Serial.println(valIRB);

  // SR04
  digitalWrite(trig1, 0);
  delayMicroseconds(2);
  digitalWrite(trig1, 1);
  delayMicroseconds(5);
  digitalWrite(trig1, 0);
  duration1 = pulseIn(echo1, HIGH);
  distance1 = int(duration1 / 2 / 29.412);
  digitalWrite(trig2, 0);
  delayMicroseconds(2);
  digitalWrite(trig2, 1);
  delayMicroseconds(5);
  digitalWrite(trig2, 0);
  duration2 = pulseIn(echo2, HIGH);
  distance2 = int(duration2 / 2 / 29.412);
  digitalWrite(trig3, 0);
  delayMicroseconds(2);
  digitalWrite(trig3, 1);
  delayMicroseconds(5);
  digitalWrite(trig3, 0);
  duration3 = pulseIn(echo3, HIGH);
  distance3 = int(duration3 / 2 / 29.412);

  Serial.print(distance1);
  Serial.println("cm");
  Serial.print(distance2);
  Serial.println("cm");
  Serial.print(distance3);
  Serial.println("cm");
  search();
  delay(1000);
}