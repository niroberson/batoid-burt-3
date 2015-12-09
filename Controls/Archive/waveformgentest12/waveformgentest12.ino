#include <Serial.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> // For the 16ch servo driver

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //Default address 0x40
#define SERVOMIN 150 // 'min' pulse length count (out of 4096) for PWM signal
#define SERVOMAX 600 // 'max' pulse length count (out of 4096) for PWM signal
// ^^^^ Find these experimentally

// Wave constants
double f = 1;
double mid = 90;
double Amax = 20;

const int N = 6; // NUMBER OF SERVOS PER FIN
double xs[] = {3,6,9,12,15,18};
double L = xs[N-1];
double lam = L; //2.0*L/3.0;
double As[2*N];

// Limits of wave constants
double pmin = 60;
double pmax = 120;
double freqmax = 2.0;

// Set up waveform generator with constants Pa, Pb for sin,cos polynomials
double Ck1 = 1, Co1 = 1, Sk1 = 0, So1 = 0;  //Fin 1
double Ck2 = 1, Co2 = 1, Sk2 = 0, So2 = 0;  //Fin 2

double abr[4];      // 0,1: coefs for fin 1;  2,3: coefs for fin 2
double abf[2*N][2]; // 0-(N-1): coefs for fin 1;  N-(2*N-1): coefs for fin 2

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
  
  /*
  coefFreq(f,&abr[0]);  //Fin 1
  coefFreq(f,&abr[2]);  //Fin 2
  
  for (int i = 0; i < N; i++) {
    // Fin 1
    coefLam(lam,xs[i],abf[i]);
    As[i] = Amax*xs[i]/L;
    
    //Fin 2
    coefLam(lam,xs[i],abf[i+N]);
    As[i+N] = Amax*xs[i]/L;
  }*/
  setwave();
}

void loop() {
  // Read input key and change vars accordingly
  /*
  if (Serial.available() > 0) {
    byteRead = Serial.read();
    adjwave(byteRead);
    setwave();
  }*/
  
  Co1 = Ck1; So1 = Sk1;
  Ck1 = abr[0]*Co1 - abr[1]*So1;
  Sk1 = abr[1]*Co1 + abr[0]*So1;
  
  Co2 = Ck2; So2 = Sk2;
  Ck2 = abr[2]*Co2 - abr[3]*So2;
  Sk2 = abr[3]*Co2 + abr[2]*So2;
  
  double pulselen[2*N];
  double yi[2*N]; //JUST FOR DEBUGGING
  
  for (int i = 0; i < N; i++) {
    double pos1 = mid + As[i]*(abf[i][1]*Ck1 - abf[i][0]*Sk1);
    double pos2 = mid - As[i+N]*(abf[i+N][1]*Ck2 - abf[i+N][0]*Sk2); //Mirrored to handle physical layout
  
    if (pos1>pmax) {
      pos1 = pmax;
    } else if (pos1<pmin) {
      pos1 = pmin;
    }
    if (pos2>pmax) {
      pos2 = pmax;
    } else if (pos2<pmin) {
      pos2 = pmin;
    }
    
    pulselen[i] = setpulse(pos1);
    pulselen[i+N] = setpulse(pos2);
    
    yi[i] = pos1;
    yi[i+N]=pos2;
  }
  
  
  for (int servonum = 0; servonum < 2*N; servonum++) {
    pwm.setPWM(servonum,0,pulselen[servonum]);
  }
  
  
  /*
  for (int i = 0; i < N; i++) {
    Serial.print(yi[i]);
    Serial.print("\t");
  }
  Serial.println();
  for (int i = 0; i < N; i++) {
    Serial.print(yi[i+N]);
    Serial.print("\t");
  }
  Serial.println("\n");
  delay(500);
  */
  
  //Fixing Loop Time
  fixTime();
}



double setpulse(double ang) {
  //double pwmrange = SERVOMAX - SERVOMIN;
  //double angrange = 170.0; //Approximate range of servo angle
  double pulse = 2.6789*ang + 127.57;
  
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
  // dt = 10 ms
  double Pa[] = {0.000000641, 0.000000052,-0.001974030, 0.000000082, 0.999999986};
  double Pb[] = {0.000000101,-0.000041791, 0.000000840, 0.062831256, 0.000000098};
  
  // dt = 2 ms
  //double Pa[] = {0.000000001, 0.000000000,-0.000078957, 0.000000000, 1.000000000};
  //double Pb[] = {0.000000000,-0.000000331, 0.000000000, 0.012566370, 0.000000000};
  
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

/*
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
*/

void setwave() {
  coefFreq(f,&abr[0]);  //Fin 1
  coefFreq(f,&abr[2]);  //Fin 2
  
  for (int i = 0; i < N; i++) {
    // Fin 1
    coefLam(lam,xs[i],abf[i]);
    As[i] = Amax*xs[i]/L;
    
    //Fin 2
    coefLam(lam,xs[i],abf[i+N]);
    As[i+N] = Amax*xs[i]/L;
  }
}
