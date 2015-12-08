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
//double offset[] = {-12, -11, 3, 7, 15, 10, 17, 1};
// Define x position of servos, here we have to switch the direction
//double xs[] = {23, 21, 18, 15, 12, 9, 6, 3};
double xs[] = {3, 6, 9, 12, 15, 18, 21, 23};

// Define wave parameters
#define L 23 // Length of the fin
#define Amax 30 // Amplitude (degrees)
#define w 20 // Frequency (rad/s)
#define lambda L // Wavelength
#define center 90 // Degrees at center line

// Initialize the time variable
double start_time = 0;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);
  centerFin();
  start_time = millis();
}

void loop() {
  double t = (millis() - start_time)/1000;
  undulate(t);
}

// Center the fin in the setup
void centerFin() {
  for (int iServo = 0; iServo < N; iServo++ ) {
    setPosition(iServo, center);
  }
}

// Undulation function to move each servo according to wave equation
void undulate(double t) {
  double y[N];
  // Calculate the new position (degrees) of each servo
  for (int iServo = 0; iServo < N; iServo++ ) {
    double A  = xs[iServo]*Amax/L;
    double k = 2*M_PI/lambda;
    y[iServo] = A*sin(k*xs[iServo] - w*t);
    setPosition(iServo, center + y[iServo]);
  }
}

// Function to set servo position
void setPosition(int iServo, double angle) {
  double pulse = convertAngleToPulse(iServo, angle);
  pwm.setPWM(iServo, 0, pulse);  
}

// Functio nto convert an angle into a servo pulse
double convertAngleToPulse(int iServo, double angle) {
  double pwmrange = SERVOMAX - SERVOMIN;
  double angrange = 180.0; //Approximate range of servo angle
  double pulse = ((angle + offset[iServo]) * (pwmrange/angrange)) + SERVOMIN;
  return pulse;
}

