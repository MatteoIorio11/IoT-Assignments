#include "Led.h"
#include "Arduino.h"

Led::Led(int pin){
    this->pin = pin;
    this->state = OFF;
    pinMode(this->pin, OUTPUT);
}

void Led::ledOn(){
    digitalWrite(this->pin, HIGH);
}

void Led::ledOff(){
    digitalWrite(this->pin, LOW);
}