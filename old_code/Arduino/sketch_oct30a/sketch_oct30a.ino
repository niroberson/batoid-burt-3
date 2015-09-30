/* Sweep */

#include <math.h>
#include <Serial.h>
#include <Servo.h>

double time = 0;
double told = 0;
double phi = 0;
double A = 50.0;
double pos = A;
const double pi = M_PI;
double omega = 0.5*(2*pi);
double om = omega/2;
int servoPin = 9;
Servo servo;

int potPin = 0;
int angle = 0;


void setup() {
  servo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  told = time;
  time = millis()/1000.0;
  int reading = analogRead(potPin); // 0 to 1023 (2^10)
  
  double omold = om;
  om = omega*reading/1023.0;
  
  phi = phi + (om-omold)*told;
  pos = A*(1+sin(om*time + phi));
  servo.write(pos);
  
  Serial.println(phi);
  
  /*int reading = analogRead(potPin); // 0 to 1023 (2^10)
  int angle = reading / 6;          // 0 to 180-ish
  servo.write(angle);
  */
  
  /*
  // scan from 0 to 180 degrees
  for(angle = 0; angle < 180; angle++) {
    servo.write(angle);
    delay(15);
  }
  //now scan back from 180 to 0 degrees
  for(angle = 180; angle > 0; angle--) {
    servo.write(angle);
    delay(15);
  }
  */
}
