#include <Arduino.h>
#include "logic/logic-smart-light-system/LogicSLS.h"
#include "logic/timer/ServoTimer2.h"
#define PIN_LED 12
#define PIN_PIR 2
#define PIN_PHOTORESISTOR A0
#define PERIOD 100

ServoTimer2 timer;

void setup() {
  Serial.begin(9600);
  initSLS(PIN_PIR, PIN_LED, PIN_PHOTORESISTOR, timer.read());
  /*
  if(PERIOD > 0){
    timer.setupPeriod(PERIOD);
  }else{
    timer.setupPeriod(100);
  }
  */
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  //timer.waitForNextTick();
  tickSLS();
}