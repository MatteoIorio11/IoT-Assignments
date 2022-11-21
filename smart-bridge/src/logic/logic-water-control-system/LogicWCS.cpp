#include "functionalities/motor-control/MotorControl.h"
#include "functionalities/waterflow-control-system/WaterflowContolSystem.h"
#include "LogicWCS.h"
#include "logic/logic-smart-light-system/LogicSLS.h"
#include <EnableInterrupt.h>


MotorControl* mc;
WaterflowControlSystem* wcs;
Timer* timer;
int timer_blinking = 0;

void buttonHandler(){
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    // If interrupts come faster than 200ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > 500) 
    {
        last_interrupt_time = interrupt_time;
        Serial.println("pressed");
        switch(mc->getState())
        {
            case OFF:
                break;
            case AUTOMATIC:
                mc->manual();
                break;
            case MANUAL:
                mc->off();
                break;
        }
    }
}

void initWCS(Timer* t, int pin_servo, int pin_pot, int pin_button, int sonar_echoPin, int sonar_trigPin, int red_pin_led, int green_pin_led, int address, int rows, int cols){
    timer = t;
    mc = new MotorControl(pin_servo, pin_pot, pin_button);
    wcs = new WaterflowControlSystem(sonar_echoPin, sonar_trigPin, red_pin_led, green_pin_led, address, rows, cols);
    mc->init();
    wcs->init();
    enableInterrupt(mc->getButton().getPin(), buttonHandler, HIGH);
}

void blink(){
    if(timer_blinking >= BLINK_2SEC(PREALARM_STATE_SAMPLING)){
        wcs->RedLedBlink();
        timer_blinking = 0;
    }else{
        timer_blinking++;
    }
}

void automatic(){
    mc->automatic();
}

void tickWCS(){
    wcs->refreshWaterState(timer);
    switch (wcs->getState())
    {
        case SHUT:
            break;
        case NORMAL:
            if(isInAlarmState()){
                resetStatus();
            }        
            tickSLS(NORMAL_STATE_SAMPLING);
            JsonSerializer::serialize(NORMAL, wcs->getLastLevelDetected(), ANGLE_NOT_SET);
            break;
        case PRE_ALARM:
            if(isInAlarmState()){
                resetStatus();
            }   
            tickSLS(PREALARM_STATE_SAMPLING);
            blink();
            wcs->displayPreAlarm(wcs->getLastLevelDetected());
            JsonSerializer::serialize(PRE_ALARM, wcs->getLastLevelDetected(), ANGLE_NOT_SET);
            break;
        case ALARM:
            float level = wcs->getLastLevelDetected();
            wcs->displayAlarm(level, mc->getServoMotor().getAngle());
            if(!isInAlarmState()){
                //If the Smart light system is not in the alarm state It must be setted
                setAlarm();
            }
            switch (mc->getState())
            {
                case OFF:
                    mc->closeValve();
                    wcs->refreshWaterState(timer);
                    if(wcs->getState() == ALARM){
                        mc->automatic();
                    }
                    break;
                case AUTOMATIC:
                    if(level < WL2_BOUND){
                        /// Because maybe I am not in the ALARM State so I do not have to turn on the
                        mc->automaticControl(MINIMUM_SONAR_DISTANCE, WL2_BOUND, level);
                        JsonSerializer::serialize(ALARM, level, mc->getServoMotor().getAngle());
                    }
                    break;
                case MANUAL:
                    mc->manualControl();
                    JsonSerializer::serialize(ALARM, level, mc->getServoMotor().getAngle());
                    break;
            }
            break;
    }
}