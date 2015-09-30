/*
  Basic servo mapping for 6 servos (assumes 1.5 waves, equally spaced)
*/

#include <math.h>
#include <Serial.h>
#include <Servo.h>

const double pi = M_PI;
int baudrate = 9600;

float L = 1;
float numwav = 1.5;
float lam = L/numwav; //wavelength
double Amax = 60;
double om = 0.5 *(2*pi);
double midpos = 90;

const int numservos = 6;
Servo servos[numservos];
//Servo s1; Servo s2; Servo s3; Servo s4; Servo s5; Servo s6;
int spins[] = {2, 3, 4, 5, 6, 7};

float xpins[] = {L/6, L/3, L/2, 2*L/3, 5*L/6, L};
/*
double Apins[numservos];
double fpins[numservos];
*/
double ypins[numservos];
double pinpos[numservos];

void setup(){
  //Set PWM pins to output?
  for (int i=0; i < numservos; i++){
    pinMode(spins[i], OUTPUT);
    servos[i].attach(spins[i]);
  }
  //Attach servos
  /*
  s1.attach(spins[0]); s2.attach(spins[1]); s3.attach(spins[2]);
  s4.attach(spins[3]); s5.attach(spins[4]); s6.attach(spins[5]);
  */
  
  /*
  //Set amplitude and phase for each pin
  for (int i=0; i < numservos; i++) {
    Apins[i] = amp(xpins[i]);
    fpins[i] = phase(xpins[i]);
  }*/
  
  
  //Initialize Serial Monitor (for debugging)
  Serial.begin(baudrate);
}

void loop(){
  //Get system time
  double time = millis()/1000;
  
  //Find pin position for each pin at the current time
  for (int i=0; i < numservos; i++) {
    // pinpos[i] = midpos + Apins[i]*sin(om*time + fpins[i]);
    pinpos[i] = midpos + y(xpins[i],time);
    servos[i].write(pinpos[i]);
  }
  
  //Write pin positions to servos
  /*
  s1.write(pinpos[0]); s2.write(pinpos[1]); s3.write(pinpos[2]);
  s4.write(pinpos[3]); s5.write(pinpos[4]); s6.write(pinpos[5]);
  */
}

/*
//Determine the phase of a pin based on rad position and wavelength
double phase(float x){
  double phi = 2*pi*x/lam;
  return phi;
}
//Determine the maximum amplitude of a pin based on rad position and rad length
double amp(float x){
  double A = Amax*x/L;
  return A;
}*/

double y(float x,double t) {
  double y = (Amax*x/L) * sin((2*pi/lam)*x - om*t);
  return y;
}
