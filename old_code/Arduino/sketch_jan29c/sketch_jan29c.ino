#include <Serial.h>
#include <math.h>
#include <Servo.h>

double time = 0;
double told = 0;
double phi = 0;
double mid = 90;
double A = 18;
double pos = mid;
const double pi = M_PI;
double f = 0.5;
double omega = f*(2*pi);
double om = omega;
int servoPin = 9;
Servo servo;
double frac = 0.5;
byte byteRead = 0;

void setup() {
  servo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  
  if (Serial.available() > 0) {
    byteRead = Serial.read();
    switch (byteRead) {
      case 119:  // 'w'
        // Increase amplitude
        frac = frac + 1/A;
        if (frac > 1.0) {frac = 1.0;}
        break;
      case 115:  // 's'
        // Decrease amplitude
        frac = frac - 1/A;
        if (frac < 0.0) {frac = 0.0;}
        break;
      case 100:  // 'd'
        // Increase frequency
        f = f + 0.1;
        if (f > 5.0) {f = 5.0;}
        break;
      case 97:   // 'a'
        // Decrease frequency
        f = f - 0.1;
        if (f < 0.0) {f = 0.0;}  //can it be negative?
        break;    
    }
  }
  
  
  
  told = time;
  time = millis()/1000.0;
  
  // Amplitude Modifications
  double Am = A*frac;
  
  // Frequency Modifications
  om = omega;
  omega = 2*pi*f;
  phi = phi + (om - omega)*told;
  
  pos = mid + Am*sin(omega*time + phi);
  servo.write(pos);
  
  
  Serial.println(Am);
  Serial.println(f);
}
