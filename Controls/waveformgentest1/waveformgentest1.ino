#include <math.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> // For the 16ch servo driver

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //Default address 0x40
#define SERVOMIN 150 // 'min' pulse length count (out of 4096) for PWM signal
#define SERVOMAX 600 // 'max' pulse length count (out of 4096) for PWM signal
// ^^^^ Find these experimentally

int servonum = 0;

// Wave constants
double f = 0.5;
double mid = 90;
double A = 9.0;
double time = 0;

// Set up waveform generator with constants Pa, Pb for sin,cos polynomials
double Pa[] = {0.000000641, 0.000000052,-0.001974030, 0.000000082, 0.999999986};
double Pb[] = {0.000000101,-0.000041791, 0.000000840, 0.062831256, 0.000000098};
double Ck = 1, Co = 1;
double Sk = 0, So = 0;

double alphar = Pa[4]+f*(Pa[3]+f*(Pa[2]+f*(Pa[1]+f*Pa[0])));  //These will have to go in loop for var
double betar  = Pb[4]+f*(Pb[3]+f*(Pb[2]+f*(Pb[1]+f*Pa[0])));  //frequency

// Set up fixed time constants
int STD_LOOP_TIME = 10; //This should be dt, NOT dt-1
int lastLoopTime = STD_LOOP_TIME;
int lastLoopUsefulTime = STD_LOOP_TIME;
unsigned long loopStartTime = 0;



void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60); // Analog servos run at ~60 Hz updates
  
}

void loop() {
  Co = Ck;
  So = Sk;
  Ck = alphar*Co - betar*So;
  Sk = betar*Co + alphar*So;
  
  double pos = mid + A*Sk;
  double pulselen = setpulse(pos);
  pwm.setPWM(servonum,0,pulselen);
  
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
