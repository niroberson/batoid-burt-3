#include <Serial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN 140
#define SERVOMAX 590

int pinNum = 0;
int pulselen = 300;
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
        pulselen += 1;
        break;
      case 115: // 's'
        pulselen -= 1;
        break;
      
      case 101: // 'e'
        pulselen += 10;
        break;
      case 100: // 'd'
        pulselen -= 10;
        break;
      
      case 114: // 'r'
        pulselen += 100;
        break;
      case 102: // 'f'
        pulselen -= 100;
        break;
    }
    
    double angle = getang(pulselen);
    
    //Serial.print(angle);
    //Serial.print("\t\t");
    Serial.println(pulselen);
  }
  
  pwm.setPWM(pinNum,0,pulselen);
}

double getang(double pulse) {
  double pwmrange = SERVOMAX - SERVOMIN;
  double angrange = 170.0; //Approximate range of servo angle
  double ang = angrange*(pulse - SERVOMIN)/pwmrange;
  
  return ang;
}
