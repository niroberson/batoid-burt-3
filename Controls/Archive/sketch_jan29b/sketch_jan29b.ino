/* Sweep */

#include <math.h>
#include <Serial.h>
#include <Servo.h>

double time = 0;
double told = 0;
double phi = 0;
double mid = 90;
double A = 60.0;
double pos = mid;
const double pi = M_PI;
double omega = 0.5*(2*pi);
double om = omega/2;
int servoPin = 9;
Servo servo;

double frac = 0.5;


void setup() {
  servo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  told = time;
  time = millis()/1000.0;
  
  //int reading = analogRead(potPin); // 0 to 1023 (2^10)
  //double frac = reading/1023.0;
  
  double Am = A*frac;
  
  pos = mid+Am*sin(omega*time);
  servo.write(pos);
  
  Serial.println(Am);
}
