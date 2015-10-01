#include <math.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> // For the 16ch servo driver

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //Default address 0x40
#define SERVOMIN 150 // 'min' pulse length count (out of 4096) for PWM signal
#define SERVOMAX 600 // 'max' pulse length count (out of 4096) for PWM signal
// ^^^^ Find these experimentally

int servonum = 1;

const int N = 3;
const double xs[] = {1,2,3};
const double L = xs[N];

double f = 0.5;
double mid = 90;
double A[] = {6.0, 7.5, 9.0};
double phi[] = {0, M_PI/4, M_PI/2};

double time = 0;
double omega = 2 * M_PI * f;
double pos[N];

void setup() {
  // To speed up more, can go into 'fast 400khz I2C mode' - see pwmtest example (TWBR->12)
  pwm.begin();
  pwm.setPWMFreq(60); // Analog servos run at ~60 Hz updates; max is 1600 Hz
}

void loop() {
  time = millis()/1000.0;
  
  for (int i = 0; i < N; i++) {
    pos[i] = mid + A[i]*sin(omega*time + phi[i]);
    pwm.setPWM(servonum,0,setpulse(pos[i]));
  }
}

double setpulse(double ang) {
  double pwmrange = SERVOMAX - SERVOMIN;
  double angrange = 180.0; //Approximate range of servo angle
  double pulse = ang * pwmrange/angrange;
  
  return pulse;
}
