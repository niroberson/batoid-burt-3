#include <Serial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN 140
#define SERVOMAX 590

int pinNum = 0;
double startpt = 75;
double stoppt = 105;
double angspd = 200; // # deg / s
double incr = 10;    // hit # points between start and stop

byte byteRead = 0;
boolean flag = false;
boolean restart = false;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);
  pwm.setPWM(pinNum,0,setpulse(90));
}

void loop() {
  
  if (Serial.available() > 0) {
    byteRead = Serial.read();
    switch (byteRead) {
      // SET START/END POINTS
      case 119: // 'w'
        startpt -= 5;
        stoppt += 5;
        break;
      case 115: // 's'
        startpt += 5;
        stoppt -= 5;
        break;
      
      // SET ANGULAR SPEED
      case 101: // 'e'
        angspd += 10;
        break;
      case 100: // 'd'
        angspd -= 10;
        break;
      case 114: // 'r'
        angspd += 100;
        break;
      case 102: // 'f'
        angspd -= 100;
        break;
      
      // Start test
      case 106: // 'j'
        flag = true;
        break;
      // Go back to start point
      case 107: // 'k'
        restart = true;
        break;
    }    
    Serial.print(startpt);
    Serial.print("\t\t");
    Serial.print(stoppt);
    Serial.print("\t\t--\t\t");
    Serial.println(angspd);
  }
  
  // Start test - from startpt to stoppt at angspd
  
  if (flag) {
    /*double ang = startpt;
    double dt = (stoppt-startpt)/angspd /incr * 1000; // milliseconds
    for (int i = 0; i < incr; i++) {
      double starttime = millis();
      
      ang += angspd*dt;
      pwm.setPWM(pinNum,0,setpulse(ang));
      
      delay((millis()-starttime)-dt);
    }*/
    double pulselen = setpulse(stoppt);
    pwm.setPWM(pinNum,0,pulselen);
    
    flag = false;
  }
  // Go back to startpt
  if (restart) {
    double pulselen = setpulse(startpt);
    pwm.setPWM(pinNum,0,pulselen);
    restart = false;
  }
}

double setpulse(double ang) {
  //double pwmrange = SERVOMAX - SERVOMIN;
  //double angrange = 170.0; //Approximate range of servo angle
  double pulse = 2.6789*ang + 127.57;
  
  return pulse;
}
