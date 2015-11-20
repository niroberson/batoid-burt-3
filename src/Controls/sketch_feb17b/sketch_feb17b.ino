#include <Serial.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> // For the 16ch servo driver

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //Default address 0x40
#define SERVOMIN 150 // 'min' pulse length count (out of 4096) for PWM signal
#define SERVOMAX 600 // 'max' pulse length count (out of 4096) for PWM signal
// ^^^^ Find these experimentally

int STD_LOOP_TIME = 9;
int lastLoopTime = STD_LOOP_TIME;
int lastLoopUsefulTime = STD_LOOP_TIME;
unsigned long loopStartTime = 0;

const double pi = M_PI;
short N = 6;
double xs[] = {1,2,3,4,5,6};
double L = xs[N];
double Alim[] = {81,99};  //degrees
double flim = 5;  //Hz

double time = 0;
double told = 0;
double phi = 0;
byte byteRead = 0;  //Used for input

double mid = 90;  //Consider making this an array
double Ao = 5;  //max deg around the midline (Ao*x/L*sin...)
double Ay = 0;  //amp diff from ave (Ao)
double fo = 0.5;  // ave freq (btwn fins), Hz
double fy = 0;    // freq diff from ave (fo), Hz
double th1 = 0;  //fin 1, deg diff from nominal midline
double th2 = 0;  //fin 2, deg diff from nominal midline
double lam = 0.9*L;  //
double lamy = 0;

double pos = mid;
double omega = 2*pi*(fo);
double omold = omega;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

void loop() {
  // Read input key and change vars accordingly
  if (Serial.available() > 0) {
    byteRead = Serial.read();
    adjwave(byteRead);
  }
    
  
  // Calculate servo positions and move the servos
  
  
  
  // Fixing Loop Time
  lastLoopUsefulTime = millis() - loopStartTime;
  if(lastLoopUsefulTime<STD_LOOP_TIME) {
    delay(STD_LOOP_TIME - lastLoopUsefulTime);
  }
  lastLoopTime = millis() - loopStartTime;
  loopStartTime = millis();
}

void adjwave(byte key) {
  switch (key) {
    // FREQUENCY CHANGES
    case 119:  // 'w'
      // Increase average frequency
      fo = fo + 0.1;
      break;
    case 115:  // 's'
      // Decrease average frequency
      fo = fo - 0.1;
      break;
    case 100:  // 'd'
      // Increase frequency on left fin (to turn right)
      fy = fy + 0.1;
      break;
    case 97:   // 'a'
      // Increase frequency on right fin (to turn left)
      fy = fy - 0.1;
      break;
      
    // AMPLITUDE CHANGES
    case 114: // 'r'
      Ao = Ao + 1;
      break;
    case 102: // 'f'
      Ao = Ao - 1;
      break;
    case 116: // 't'
      Ay = Ay + 1;
      break;
    case 103: // 'g'
      Ay = Ay - 1;
      break;
    
    // MEAN ANGLE CHANGES
    case 117: // 'u'
      th1 = th1 - 1; // Note that right fin = fin 1
      th2 = th2 - 1;
      break;
    case 106: // 'j'
      th1 = th1 + 1;
      th2 = th2 + 1;
      break;
    case 107: // 'k'
      th1 = th1 + 1;
      th2 = th2 - 1;
      break;
    case 104: // 'h'
      th1 = th1 - 1;
      th2 = th2 + 1;
      break;
    
    // WAVELENGTH CHANGES
    case 111: // 'o'
      lam = lam + xs[1]/(2*L);
      break;
    case 108: // 'l'
      lam = lam - xs[1]/(2*L);
      break;
    case 112: // 'p'
      lamy = lamy - xs[1]/(2*L);
      break;
    case 59: // ';'
      lamy = lamy + xs[1]/(2*L);
      break;
  }
}

