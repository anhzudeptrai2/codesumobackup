#include <Arduino.h>
int RPWM1 = 7;
int LPWM1 = 6;
int RPWM = 8;
int LPWM = 9;
int IRR = 2;
int IRL = 3;
int IRB = 21;  // chua test
int trig1 = 4; // SR trướcttttt
int echo1 = 5;
int trig2 = 12; // SR phải
int echo2 = 13;
int trig3 = 10; // SR trái
int echo3 = 11;
int valIRR;
int valIRL;
int valIRB;
#define button_K6 A0 // reset xe ngung hoat dong
#define button_K7 A1 // delay 3s chay default
#define button_K8 A2 // tactics ...
unsigned long previousSR_millis = 0;
unsigned long duration1, duration2, duration3;
int distance1;
int distance2;
int distance3;
int lastdistance1 = 0;
int lastdistance2 = 0;
int lastdistance3 = 0;
boolean firstRun = true; // co chay lan dau tien

void BACKWARD()
{
  analogWrite(LPWM, 0);
  analogWrite(LPWM1, 255);
  analogWrite(RPWM, 255);
  analogWrite(RPWM1, 0);
}

void STOP()
{
  analogWrite(LPWM, 0);
  analogWrite(RPWM, 0);
  analogWrite(LPWM1, 0);
  analogWrite(RPWM1, 0);
}
// void THOATHIEM()
// {
//   analogWrite(LPWM, 0);
//   analogWrite(RPWM, 255);
//   analogWrite(LPWM1, 0);
//   analogWrite(RPWM1, 255);
// }
void ROTATER()
{
  analogWrite(LPWM, 175);
  analogWrite(RPWM, 0);
  analogWrite(LPWM1, 175);
  analogWrite(RPWM1, 0);
  delay(350);
}
void ROTATEL()
{
  analogWrite(LPWM, 0);
  analogWrite(RPWM, 175);
  analogWrite(LPWM1, 0);
  analogWrite(RPWM1, 175);
  delay(350);
}
void RBACKWARD()
{
  analogWrite(LPWM, 0);
  analogWrite(RPWM, 255);
  analogWrite(LPWM1, 225);
  analogWrite(RPWM1, 0);
}
void LBACKWARD()
{
  analogWrite(LPWM, 0);
  analogWrite(LPWM1, 255);
  analogWrite(RPWM, 225);
  analogWrite(RPWM1, 0);
}
void RFORWARD()
{
  analogWrite(LPWM, 255);
  analogWrite(RPWM, 0);
  analogWrite(LPWM1, 0);
  analogWrite(RPWM1, 225);
}
void LFORWARD()
{
  analogWrite(LPWM, 225);
  analogWrite(RPWM, 0);
  analogWrite(LPWM1, 0);
  analogWrite(RPWM1, 255);
}
void PUSH()
{
  analogWrite(LPWM, 255);
  analogWrite(RPWM, 0);
  analogWrite(LPWM1, 0);
  analogWrite(RPWM1, 255);
}
void blind_runRight()
{
  unsigned long currnentSR_millis = millis();
  if (currnentSR_millis - previousSR_millis >= 2000)
  {
    analogWrite(LPWM, 255);
    analogWrite(RPWM, 0);
    analogWrite(LPWM1, 0);
    analogWrite(RPWM1, 225);
  }
}

void proval()
{
  valIRR = digitalRead(IRR);
  valIRL = digitalRead(IRL);
  valIRB = digitalRead(IRB);

  if (valIRR == 0)
  {
    Serial.println("Lui trai");
    // delay(100);
    RBACKWARD();
  }
  else if (valIRL == 0)
  {
    Serial.println("Lui phai");
    // delay(100);
    LBACKWARD();
  }
  else if (valIRL == 0 && valIRR == 0)
  {
    Serial.println("Thoathiem");
    BACKWARD();
  }
  else if (valIRB == 0)
  {
    Serial.println("Tien");
    // delay(100);
    PUSH();
  }
}
int filterValF()
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
void tactic_default()
{

  if (filterValF() <= 40)
  {
    Serial.println("pushF");
    PUSH();
  }
  else if (filterValR() <= 30)
  {
    Serial.println("pushR");
    ROTATER();
  }
  else if (filterValL() <= 40)
  {
    Serial.println("pushL");
    ROTATEL();
  }
  else if (filterValF() <= 45 && filterValR() <= 30)
  {
    Serial.println("chay ne diem mu phai");
    RFORWARD();
  }
  else if (filterValF() <= 45 && filterValL() <= 30)
  {
    Serial.println("chay ne diem mu trai");
    LFORWARD();
  }

  else
  {
    ROTATEL();
  }
  // }
}
void tactic_attack()
{
  blind_runRight();
  Serial.println("chay mu");
  if (filterValF() <= 3)
  {
    Serial.println("Lock");
    PUSH();
  }
  else
  {
    Serial.println("Tiep tuc tim kiem");
    tactic_default();
  }
}
void tactic_defence()
{
  unsigned long currnentSR_millis = millis();
  if (currnentSR_millis - previousSR_millis > 500)
  {
    //   analogWrite(LPWM, 255);
    //   analogWrite(RPWM, 0);
    //   analogWrite(LPWM1, 200);
    //   analogWrite(RPWM1, 0);
    //   previousSR_millis = currnentSR_millis;
    //   search();
    // }
    if (filterValR() <= 20)
    {
      Serial.println("ne");
      RFORWARD();
    }
    else if (filterValL() <= 20)
    {
      Serial.println("ne");
      LBACKWARD();
    }
    else
    {
      if (filterValF() <= 40)
      {
        PUSH();
        Serial.println("PushF");
      }
      else
      {
        ROTATER();
      }
    }
  }
}

int button_state = 0;
void Button_Control()
{
  int val_K6 = digitalRead(button_K6);
  int val_K7 = digitalRead(button_K7);
  int val_K8 = digitalRead(button_K8);

  if (val_K6 == 0)
  {
    button_state = 6;
  }
  else if (val_K7 == 0)
  {
    button_state = 7;
  }
  else if (val_K8 == 0)
  {
    button_state = 8;
  }
}

void setup()
{
  // while (1);
  Serial.begin(9600);
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
  // nut bam
  pinMode(button_K6, INPUT_PULLUP);
  pinMode(button_K7, INPUT_PULLUP);
  pinMode(button_K8, INPUT_PULLUP);
  // ngat
  attachInterrupt(0, proval, CHANGE);
  attachInterrupt(1, proval, CHANGE);
  attachInterrupt(2, proval, CHANGE); // chua test
}

void loop()
{

  Button_Control();
  if (button_state == 6)
  {
    if (firstRun)
    {
      delay(2900);
      firstRun = false;
    }
    tactic_attack();
    Serial.println("treo");
  }
  else if (button_state == 7)
  {
    if (firstRun)
    {
      delay(2900);
      firstRun = false; //
    }
    tactic_default();
    Serial.println("chay mac dich");
  }
  else if (button_state == 8)
  {
    if (firstRun)
    {
      delay(2900);
      firstRun = false;
    }
    tactic_defence();
    Serial.println("Chay trien thuat phong ngu");
  }
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
  filterValL();
  filterValR();
  filterValF();
  Serial.print(distance1);
  Serial.println("cm");
  Serial.print(distance2);
  Serial.println("cm");
  Serial.print(distance3);
  Serial.println("cm");
  // delay(1000);
}
