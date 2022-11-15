#include "WaterflowContolSystem.h"
#include "hardware/sonar/Sonar.h"
#include "hardware/led/GreenLed.h"
#include "hardware/led/RedLed.h"
#include "Arduino.h"



WaterflowControlSystem::WaterflowControlSystem(int sonar_echoPin, int sonar_trigPin, int red_pin_led, int green_pin_led, int address, int rows, int cols){
    this->sonar_echoPin = sonar_echoPin;
    this->sonar_trigPin = sonar_trigPin;
    this->red_pin_led = red_pin_led;
    this->green_pin_led = green_pin_led;
    this->address = address;
    this->rows = rows;
    this->cols = cols;
    this->state = NORMAL;
}

void WaterflowControlSystem::init(){
    this->sonar = new Sonar(this->sonar_echoPin, this->sonar_trigPin);
    this->greenLed = new GreenLed(this->green_pin_led);
    this->redLed = new RedLed(this->red_pin_led); 
    this->lcd = new MonitorLcd(address, rows, cols);
}

void WaterflowControlSystem::turnOnGreenLed(){
    this->greenLed->ledOn();
}

void WaterflowControlSystem::turnOffGreenLed(){
    this->greenLed->ledOff();
}

void WaterflowControlSystem::turnOnRedLed(){
    this->redLed->ledOn();
}

void WaterflowControlSystem::turnOffRedLed(){
    this->redLed->ledOff();
}

void WaterflowControlSystem::RedLedBlink(){
    this->redLed->blink();
}

void WaterflowControlSystem::refreshWaterState(){
    WaterState tmpState;
    if(this->getWaterLevel() > MINIMUM_SONAR_DISTANCE && this->getWaterLevel() <= WL1_BOUND){
        tmpState = NORMAL;
    }else if(this->getWaterLevel() > WL1_BOUND && this->getWaterLevel() <= WL2_BOUND){
        tmpState = PRE_ALARM;
    }else if(this->getWaterLevel() > WL2_BOUND){
        tmpState = ALARM;
    }
    if(this->state != tmpState){
        this->state = tmpState;
        this->refresh();
    }
}

void WaterflowControlSystem::refresh(){
    switch (this->state)
    {
    case NORMAL:
        this->turnOnGreenLed();
        this->turnOffRedLed();
        this->turnOffDisplay();
        break;
    case PRE_ALARM:
        this->turnOnDisplay();
        this->turnOffGreenLed();
        this->RedLedBlink();
        this->displayPreAlarm(this->getWaterLevel());
        break;
    case ALARM:
        this->turnOnDisplay();
        //Secondo me non e' da mettere qui ma va bene
        this->turnOffGreenLed();
        this->turnOnRedLed();
        break;
    }
}

void WaterflowControlSystem::displayAlarm(double level, int op_degree){
    this->lcd->displayAlarm(level, op_degree);
}

void WaterflowControlSystem::displayPreAlarm(double level){
    this->lcd->displayPreAlarm(level);
}

void WaterflowControlSystem::turnOnDisplay(){
    this->lcd->displayON();
}

void WaterflowControlSystem::turnOffDisplay(){
    this->lcd->displayOFF();
}

double WaterflowControlSystem::getWaterLevel(){
    return this->sonar->readValue();
}

WaterState WaterflowControlSystem::getState(){
    return this->state;
}

Sonar WaterflowControlSystem::getSonar(){
    return *this->sonar;
}

RedLed WaterflowControlSystem::getRedLed(){
    return *this->redLed;
}

GreenLed WaterflowControlSystem::getGreenLed(){
    return *this->greenLed;
}

int WaterflowControlSystem::getSonarEchoPin(){
    return this->sonar->getEchoPin();
}

int WaterflowControlSystem::getSonarTrigPin(){
    return this->sonar->getTrigPin();
}

int WaterflowControlSystem::getRedLedPin(){
    return this->redLed->getPin();
}

int WaterflowControlSystem::getGreenLedPin(){
    return this->greenLed->getPin();
}

