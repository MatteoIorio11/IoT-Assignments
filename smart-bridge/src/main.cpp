#include <Arduino.h>
#include <TimerOne.h>
#include <ArduinoJson.h>
#include "bridge/Bridge.h"
#include "functionalities/json/json-serialization/JsonSerializer.h"
#include "functionalities/waterflow-control-system/State.h"


Bridge* b;

void c(){
  b->getTimer()->changeState();
}

void setup()
{
  Serial.begin(9600);
  b = new Bridge();
  b->init();
  b->getTimer()->getTimerOne()->attachInterrupt(c); 
}


void loop() {
  // put your main code here, to run repeatedly:
  b->getTimer()->waitForTheNextTick();
  b->tick();
  //JsonSerializer::serialize(NORMAL, 100, 10);
  //tickSLS();
  //delay(10);
}