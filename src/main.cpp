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
unsigned long previousSR_millis = 0;
int lastdistance1 = 0;
int lastdistance2 = 0;
int lastdistance3 = 0;

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
void THOATHIEM()
{
  analogWrite(LPWM, 0);
  analogWrite(RPWM, 255);
  analogWrite(LPWM1, 0);
  analogWrite(RPWM1, 255);
}

void ROTATER()
{
  analogWrite(LPWM, 125);
  analogWrite(RPWM, 0);
  analogWrite(LPWM1, 0);
  analogWrite(RPWM1, 125);
  delay(500);
}
void ROTATEL()
{
  analogWrite(LPWM, 0);
  analogWrite(RPWM, 125);
  analogWrite(LPWM1, 125);
  analogWrite(RPWM1, 0);
  delay(500);
}
// void RBACKWARD()
// {
//   analogWrite(RPWM, 0);
//   analogWrite(LPWM, 100);
//   analogWrite(RPWM1, 0);
//   analogWrite(LPWM1, 50);
// }
// void LBACKWARD()
// {
//   analogWrite(RPWM, 0);
//   analogWrite(RPWM1, 0);
//   analogWrite(LPWM, 50);
//   analogWrite(LPWM1, 100);
// }
// void RFORWARD()
// {
//   analogWrite(RPWM, 20);
//   analogWrite(RPWM1, 0);
//   analogWrite(LPWM, 120);
//   analogWrite(LPWM1, 0);
// }
// void LFORWARD()
// {
//   analogWrite(RPWM, 0);
//   analogWrite(RPWM1, 20);
//   analogWrite(LPWM, 0);
//   analogWrite(LPWM1, 120);
// }
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
    // delay(100);
    BACKWARD();
  }
  else if (valIRR == 0)
  {
    // delay(100);
    BACKWARD();
  }
  else if ( valIRL == 0 && valIRR == 0)
  {
    THOATHIEM();
  }
  else if (valIRB == 0)
  {
    // delay(100);
    FORWARD();
  }
}
int filterValueF()
{
  if (abs(distance1 - lastdistance1) > 50)
  {
    lastdistance1 = distance1;
    return min(lastdistance1, 50);
  }
  else
  {
    return min(distance1, 50);
  }
}
int filterValR()
{
  if (abs(distance2 - lastdistance2) > 25)
  {
    lastdistance2 = distance2;
    return min(lastdistance2, 50);
  }
  else
  {
    return min(distance2, 50);
  }
}
int filterValL()
{
  if (abs(distance3 - lastdistance3) > 25)
  {
    lastdistance3 = distance3;
    return min(lastdistance3, 50);
  }
  else
  {
    return min(distance3, 50);
  }
}

void search()
{
  // unsigned long currnentSR_millis = millis();
  // if (currnentSR_millis - previousSR_millis > 200)
  // {
  //   previousSR_millis = currnentSR_millis;
  if (filterValueF() <= 40)
  {
    Serial.println("pushF");
    PUSH();
  }
  else if (filterValR() <= 20)
  {
    Serial.println("pushR");
    ROTATER();
  }
  else if (filterValL() <= 20)
  {
    Serial.println("pushL");
    ROTATEL();
  }
  else
  {
    // if (scan < 3)
    // {
    //   ROTATER();
    // }
    // else if (scan <= 5)
    // {
    //   ROTATER();
    // }
    // if (scan < 5)
    // {
    //   scan += 1;
    // }
    // else if (scan == 5)
    // {
    //   scan = 0;
    // }
    analogWrite(LPWM, 120);
    analogWrite(RPWM, 0);
    analogWrite(LPWM1, 0);
    analogWrite(RPWM1, 120);
  }
  // }
}

void setup()
{
  Serial.begin(9600);
  delay(1000);
  // DC
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(RPWM1, OUTPUT);
  pinMode(LPWM1, OUTPUT);
  // IR
  pinMode(IRR, INPUT);
  pinMode(IRL, INPUT);
  pinMode(IRB, INPUT);
  // SR 
  pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  pinMode(echo3, INPUT);
  // ngat
  attachInterrupt(0, proval, CHANGE);
  attachInterrupt(1, proval, CHANGE);
  attachInterrupt(3, proval, CHANGE);
}

void loop()
{
  filterValueF();
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

  // SR04 truoc
  digitalWrite(trig1, 0);
  delayMicroseconds(2);
  digitalWrite(trig1, 1);
  delayMicroseconds(5);
  digitalWrite(trig1, 0);
  duration1 = pulseIn(echo1, HIGH);
  distance1 = int(duration1 / 2 / 29.412);
  // SR04 phai
  digitalWrite(trig2, 0);
  delayMicroseconds(2);
  digitalWrite(trig2, 1);
  delayMicroseconds(5);
  digitalWrite(trig2, 0);
  duration2 = pulseIn(echo2, HIGH);
  distance2 = int(duration2 / 2 / 29.412);
  // SR04 trai
  digitalWrite(trig3, 0);
  delayMicroseconds(2);
  digitalWrite(trig3, 1);
  delayMicroseconds(5);
  digitalWrite(trig3, 0);
  duration3 = pulseIn(echo3, HIGH);
  distance3 = int(duration3 / 2 / 29.412);

  // Serial.print(distance1);
  // Serial.println("cm");
  // Serial.print(distance2);
  // Serial.println("cm");
  // Serial.print(distance3);
  // Serial.println("cm");
  search();
}
