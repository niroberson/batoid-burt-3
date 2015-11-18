#include <Serial.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> // For the 16ch servo driver

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //Default address 0x40

// Define the servo parameters
#define N 8
#define SERVOMIN 150
#define SERVOMAX 600

// Degree offset to center all servo motors
double offset[] = {1, 17, 10, 15, 7, 3, -11, -12};

// Define x position of servos
double xs[] = {3, 6, 9, 12, 15, 18, 21, 23};

// Define wave parameters
#define L 23 // Length of the fin
#define Amax 9 // Amplitude (degrees)
#define f 2 // Frequency (Hz)
#define center 90 // Degrees at center line

int Amax[N]  = Amax[iServo] + xs[iServo]*Amax/L;;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);
  centerFin();
}

void loop() {
  undulate();
}

void centerFin() {
  for (int iServo = 0; iServo < N; iServo++ ) {
    Amax[iServo] = center;
    setServoPosition(iServo, Amax[iServo]);
  }
}

void undulate() {
  setServoPosition(iServo, currentPos[iServo]);
}

void setServoPosition(int iServo, double angle) {
  double pulse = convertAngleToPulse(iServo, angle);
  pwm.setPWM(iServo, 0, pulse);  
}

double getServoPosition(int iServo) {
  double pulse = analogRead(iServo);
  double angle = convertPulseToAngle(iServo, pulse);
  return angle;
}

double convertPulseToAngle(int iServo, double pulse) {
  double pwmrange = SERVOMAX - SERVOMIN;
  double angrange = 180.0; //Approximate range of servo angle
  double angle = ((pulse - SERVOMIN) / (pwmrange/angrange)) - offset[iServo];
//  int angle =   map(pulse, SERVOMIN, SERVOMAX, 0, 179);
  return angle;
}

double convertAngleToPulse(int iServo, double angle) {
  double pwmrange = SERVOMAX - SERVOMIN;
  double angrange = 180.0; //Approximate range of servo angle
  double pulse = ((angle + offset[iServo]) * (pwmrange/angrange)) + SERVOMIN;
  return pulse;
}

