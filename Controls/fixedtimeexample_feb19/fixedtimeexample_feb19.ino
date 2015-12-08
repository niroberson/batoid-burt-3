// From: http://www.x-firm.com/?page_id=187

#include <Serial.h>

int STD_LOOP_TIME = 9;
int lastLoopTime = STD_LOOP_TIME;
int lastLoopUsefulTime = STD_LOOP_TIME;
unsigned long loopStartTime = 0;

void setup {
  Serial.begin(9600);
}

void loop() {
  //various code stuffs
  serialOut_timing();
  
  lastLoopUsefulTime = millis() - loopStartTime;
  if(lastLoopUsefulTime<STD_LOOP_TIME) {
    delay(STD_LOOP_TIME - lastLoopUsefulTime);
  }
  lastLoopTime = millis() - loopStartTime;
  loopStartTime = millis();
}

//from http://www.uchobby.com/index.php/2012/01/21/replacing-delay-in-arduino-sketches-istime-to-the-rescue/
//  -> could get rid of the delay
//  ----> do stuff until "isTime(#,#)" is true
boolean isTime(unsigned long *timeMark, int timeInterval) {
  boolean result = false;
  unsigned long timeCurrent = millis();
  int timeElapsed = timeCurrent - *timeMark;
  
  if(timeElapsed >= timeInterval) {
    *timeMark = timeCurrent;
    result = true;
  }
  return(result);
}


void serialOut_timing() {
  static int skip=0;
  
  if(skip++==5) {
    skip = 0;
    Serial.print(lastLoopUsefulTime); Serial.print(",");
    Serial.print(lastLoopTime); Serial.print("\\n");
  }
}
