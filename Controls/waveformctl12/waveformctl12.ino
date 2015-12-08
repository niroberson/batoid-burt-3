#include <Serial.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> // For the 16ch servo driver

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //Default address 0x40
#define SERVOMIN 150 // 'min' pulse length count (out of 4096) for PWM signal
#define SERVOMAX 600 // 'max' pulse length count (out of 4096) for PWM signal
// ^^^^ Find these experimentally


// Wave Constant Parameters
const int N = 6; // NUMBER OF SERVOS PER FIN
double xs[] = {3,6,9,12,15,18};
double L = xs[N-1];
double As[2*N];
double mid = 90;

// Wave Variables
double fm = 0.5;  //mean frequency of 2 fins
double fe = 0;    //freq difference from mean of fins (- is left ^, + is right ^)
double lam = 2.0*L/3.0; //mean wavelength of 2 fins
double lame = 0;  //wavelength difference from mean of fins (- is left ^, + is right ^)
double Am = 9;  //mean maximum amplitude of 2 fins (back-heavy)
double Ae = 0;  //diff from mean of max amplitude
double thm = 0;  //Angle difference from midpt - mean of two fins
double the = 0;  //Angle difference from midpt - diff between fins

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
  
  setwave();
}

void loop() {
  // Read input key and change vars accordingly
  if (Serial.available() > 0) {
    byteRead = Serial.read();
    adjwave(byteRead);
    
  }
  
  Co1 = Ck1; So1 = Sk1;
  Ck1 = abr[0]*Co1 - abr[1]*So1;
  Sk1 = abr[1]*Co1 + abr[0]*So1;
  
  Co2 = Ck2; So2 = Sk2;
  Ck2 = abr[2]*Co2 - abr[3]*So2;
  Sk2 = abr[3]*Co2 + abr[2]*So2;
  
  double pulselen[2*N];
  double yi[2*N]; //JUST FOR DEBUGGING
  
  for (int i = 0; i < N; i++) {
    double pos1 = (mid+thm+the) + As[i]*(abf[i][1]*Ck1 - abf[i][0]*Sk1);
    double pos2 = (mid-thm+the) - As[i+N]*(abf[i+N][1]*Ck2 - abf[i+N][0]*Sk2); //Mirrored to handle physical layout
  
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
  double thinc = 1;
  
  switch (key) {
    // FREQUENCY CHANGES
    case 119:  // 'w'
      // Increase mean frequency
      fm += finc;
      if (fm+abs(fe)>freqmax) {fm = freqmax-abs(fe);}
      break;
    case 115:  // 's'
      // Decrease mean frequency
      fm -= finc;
      if (fm-abs(fe)<-freqmax) {fm = -freqmax+abs(fe);}
      break;
    case 100:  // 'd'
      // Increase diff frequency
      fe += finc;
      if (abs(fm+fe)>freqmax) {fe = sign(fe)*(freqmax-abs(fm));}
      break;
    case 97:   // 'a'
      // Decrease diff frequency
      fe -= finc;
      if (abs(fm-fe)>freqmax) {fe = sign(fe)*(freqmax-abs(fm));}
      break;
      
    // AMPLITUDE CHANGES
    case 114:  // 'r'
      // Increase mean amplitude
      Am += Ainc;
      break;
    case 102:  // 'f'
      // Decrease mean amplitude
      Am -= Ainc;
      break;
    case 116:  // 't'
      // Increase diff amplitude
      Ae += Ainc;
      break;
    case 103:   // 'g'
      // Decrease diff amplitude
      Ae -= Ainc;
      break;
    
    // WAVELENGTH CHANGES
    case 117:  // 'u'
      // Increase mean wavelength
      lam += laminc;
      break;
    case 106:   // 'j'
      // Decrease mean wavelength
      lam -= laminc;
      break;
    case 107:  // 'k'
      // Increase diff wavelength
      lame += laminc;
      break;
    case 104:   // 'h'
      // Decrease diff wavelength
      lame -= laminc;
      break;
      
    // MEAN ANGLE CHANGES
    case 111:  // 'o'
      // Up both fins
      thm += thinc;
      break;
    case 108:   // 'l'
      // Down both fins
      thm -= thinc;
      break;
    case 112:  // 'p'
      // Up Fin 1, Down Fin 2
      the += thinc;
      break;
    case 59:   // ';'
      // Down Fin 1, Up Fin 2
      the -= thinc;
      break;
  }
  setwave();
}

// VVVV - Consider switching to be diff funcs for freq, lam, and amp
void setwave() {
  coefFreq(fm+fe,&abr[0]);  //Fin 1
  coefFreq(fm-fe,&abr[2]);  //Fin 2
  
  for (int i = 0; i < N; i++) {
    // Fin 1
    coefLam(lam+lame,xs[i],abf[i]);
    As[i] = (Am+Ae)*xs[i]/L;
    
    //Fin 2
    coefLam(lam-lame,xs[i],abf[i+N]);
    As[i+N] = (Am-Ae)*xs[i]/L;
  }
}

int sign(double x) {
  return double((x>0) - (x<0));
}
