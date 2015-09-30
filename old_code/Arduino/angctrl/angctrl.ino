#include <Serial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN 140
#define SERVOMAX 590

int pinNum = 0;
double ang = 90;
byte byteRead = 0;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);
}

void loop() {
  if (Serial.available() > 0) {
    byteRead = Serial.read();
    switch (byteRead) {
      case 119: // 'w'
        ang += 1;
        break;
      case 115: // 's'
        ang -= 1;
        break;
      
      case 101: // 'e'
        ang += 10;
        break;
      case 100: // 'd'
        ang -= 10;
        break;
    }
    
    //Serial.print(angle);
    //Serial.print("\t\t");
    Serial.println(ang);
  }
  double pulselen = setpulse(ang);
  pwm.setPWM(pinNum,0,pulselen);
}

double setpulse(double angle) {
  //double pwmrange = SERVOMAX - SERVOMIN;
  //double angrange = 170.0; //Approximate range of servo angle
  double pulse = 2.6789*angle + 127.57;
  
  return pulse;
}
