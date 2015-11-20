#include <math.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> // For the 16ch servo driver

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //Default address 0x40
#define SERVOMIN 150 // 'min' pulse length count (out of 4096) for PWM signal
#define SERVOMAX 600 // 'max' pulse length count (out of 4096) for PWM signal
// ^^^^ Find these experimentally

int servonum = 0;

double time = 0;
double told = 0;
double mid = 90;
double A = 9.0;
double omega = 0.5*(2*M_PI);

void setup() {
  pwm.begin();
  pwm.setPWMFreq(60); // Analog servos run at ~60 Hz updates
}

void loop() {
  told = time;
  time = millis()/1000.0;
  
  double pos = mid + A*sin(omega*time);
  double pulselen = setpulse(pos);
  
  pwm.setPWM(servonum,0,pulselen);
}

double setpulse(double ang) {
  double pwmrange = SERVOMAX - SERVOMIN;
  double angrange = 180.0; //Approximate range of servo angle
  double pulse = ang * pwmrange/angrange + SERVOMIN;
  
  return pulse;
}
