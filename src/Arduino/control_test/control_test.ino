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
#define Amax 6 // Amplitude (degrees)
#define f 2 // Frequency (Hz)
#define lam 3 // Wavelength
#define center 90 // Degrees at center line

double start_time = 0;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);
  centerFin();
  start_time = millis();
}

void loop() {
  delay(5000);
  double t = millis() - start_time;
  undulate(t);
}

void centerFin() {
  for (int iServo = 0; iServo < N; iServo++ ) {
    setPosition(iServo, center);
  }
}

void undulate(double t) {
  int y[N];
  // Calculate the new position (degrees) of each servo
  for (int iServo = 0; iServo < N; iServo++ ) {
    int A  = Amax + xs[iServo]*Amax/L;
    Serial.println(A);
    y[iServo] = A*sin(xs[iServo]*2*M_PI/lam + 1/f*t);
  }
  
  // Set the new position of each servo
  for(int iServo = 0; iServo < N; iServo++) {
    setPosition(iServo, y[iServo]);
  }
}

void setPosition(int iServo, double angle) {
  double pulse = convertAngleToPulse(iServo, angle);
  pwm.setPWM(iServo, 0, pulse);  
}

double getPosition(int iServo) {
  double pulse = analogRead(iServo);
  double angle = convertPulseToAngle(iServo, pulse);
  return angle;
}

double convertPulseToAngle(int iServo, double pulse) {
  double pwmrange = SERVOMAX - SERVOMIN;
  double angrange = 180.0; //Approximate range of servo angle
  double angle = ((pulse - SERVOMIN) / (pwmrange/angrange)) - offset[iServo];
  return angle;
}

double convertAngleToPulse(int iServo, double angle) {
  double pwmrange = SERVOMAX - SERVOMIN;
  double angrange = 180.0; //Approximate range of servo angle
  double pulse = ((angle + offset[iServo]) * (pwmrange/angrange)) + SERVOMIN;
  return pulse;
}

