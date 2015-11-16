#include <Serial.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> // For the 16ch servo driver

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //Default address 0x40
#define SERVOMIN 150 // 'min' pulse length count (out of 4096) for PWM signal
#define SERVOMAX 600 // 'max' pulse length count (out of 4096) for PWM signal
// ^^^^ Find these experimentally

// Wave constants
double f = 0.5;
double mid = 90;
double Amax = 9;

const int N = 8;
double xs[] = {3,6,9,12,15,18,21,23};
double L = xs[N-1];
double lam = 2.0*L/3.0;
double As[N];

// Limits of wave constants
double pmin = 60;
double pmax = 120;
double freqmax = 2.0;

// Set up waveform generator with constants Pa, Pb for sin,cos polynomials
double Ck = 1, Co = 1;
double Sk = 0, So = 0;

double abr[2];
double abf[N][2];

// Set up fixed time constants
int STD_LOOP_TIME = 10;
int lastLoopTime = STD_LOOP_TIME;
int lastLoopUsefulTime = STD_LOOP_TIME;
unsigned long loopStartTime = 0;

// Set up var to read key presses
byte byteRead = 0;



void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60); // Analog servos run at ~60 Hz updates
  
  coefFreq(f,abr);
  for (int i = 0; i < N; i++) {
    coefLam(lam,xs[i],abf[i]);
    As[i] = Amax*xs[i]/L;
  }
}

void loop() {
  // Read input key and change vars accordingly
  if (Serial.available() > 0) {
    byteRead = Serial.read();
    adjwave(byteRead);
    
    coefFreq(f,abr);
    for (int i=0; i<N; i++) {
      coefLam(lam,xs[i],abf[i]);
      As[i] = Amax*xs[i]/L;
    }
  }
  
  Co = Ck;
  So = Sk;
  Ck = abr[0]*Co - abr[1]*So;
  Sk = abr[1]*Co + abr[0]*So;
  
  double pulselen[N];
  double yi[N]; //JUST FOR DEBUGGING
  
  for (int i = 0; i < N; i++) {
    double pos = mid + As[i]*(abf[i][1]*Ck - abf[i][0]*Sk);
  
    if (pos>pmax) {
      pos = pmax;
    } else if (pos<pmin) {
      pos = pmin;
    }
    
    pulselen[i] = setpulse(pos);
    yi[i] = pos;
  }
  
  
  for (int servonum = 0; servonum < N; servonum++) {
    pwm.setPWM(servonum,0,pulselen[servonum]);
  }
  
  
  //Fixing Loop Time
  fixTime();
}



double setpulse(double ang) {
  double pwmrange = SERVOMAX - SERVOMIN;
  double angrange = 180.0; //Approximate range of servo angle
  double pulse = ang * pwmrange/angrange + SERVOMIN;
  
  return pulse;
}

void fixTime() {
  lastLoopUsefulTime = millis() - loopStartTime;
  if(lastLoopUsefulTime<STD_LOOP_TIME) {
    delay(STD_LOOP_TIME - lastLoopUsefulTime);
  }
  lastLoopTime = millis() - loopStartTime;
  loopStartTime = millis();
}

void coefFreq(double freq, double coeff[]) {
  double Pa[] = {0.000000641, 0.000000052,-0.001974030, 0.000000082, 0.999999986};
  double Pb[] = {0.000000101,-0.000041791, 0.000000840, 0.062831256, 0.000000098};
  
  coeff[0] = Pa[4]+freq*(Pa[3]+freq*(Pa[2]+freq*(Pa[1]+freq*Pa[0])));  //These will have to go in loop for var
  coeff[1] = Pb[4]+freq*(Pb[3]+freq*(Pb[2]+freq*(Pb[1]+freq*Pb[0])));  //frequency
}

void coefLam(double lambda, double x, double coeff[]) {
  double Pa[] = {60.709408575, -182.128225725, 166.173260485, -28.799478096, -15.731910650, -0.223054590, 1.002616693};
  double Pb[] = {-54.848410591, 137.121026478, -98.700216010, 10.929297536, 5.472509859, 0.012896364};
  
  double xlam = x/lambda;
  while (xlam > 1) {xlam-=1.0;}
  
  coeff[0] = Pa[6]+xlam*(Pa[5]+xlam*(Pa[4]+xlam*(Pa[3]+xlam*(Pa[2]+xlam*(Pa[1]+xlam*Pa[0])))));
  coeff[1] = Pb[5]+xlam*(Pb[4]+xlam*(Pb[3]+xlam*(Pb[2]+xlam*(Pb[1]+xlam*Pb[0]))));
}


void adjwave(byte key) {
  double finc = 0.1;
  double Ainc = 1;
  double laminc = 1;
  
  switch (key) {
    // FREQUENCY CHANGES
    case 119:  // 'w'
      // Increase frequency
      f = f + finc;
      if (f>freqmax) {f = freqmax;}
      break;
    case 115:  // 's'
      // Decrease frequency
      f = f - finc;
      if (f<-freqmax) {f = -freqmax;}
      break;
    case 100:  // 'd'
      // Increase amplitude
      Amax = Amax + Ainc;
      break;
    case 97:   // 'a'
      // Decrease amplitude
      Amax = Amax - Ainc;
      break;
    case 117:  // 'u'
      // Increase wavelength
      lam = lam + laminc;
      break;
    case 106:   // 'j'
      // Decrease amplitude
      lam = lam - laminc;
      if (lam<L/N) {lam = L/N;}
      break;
  }
}
