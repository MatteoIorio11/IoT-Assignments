#include <Arduino.h>


#define EASY 0.5
#define NORMAL 1
#define MEDIUM 1.5
#define HARD 2

class TimerController{
    private :
        float timer_t1 = 0;
        float timer_t2 = 0;
        float timer_t3 = 0;
        float factor = 0;
    
    public:
        TimerController(int input){
            int difficulty = map(input, 0, 1023, 1, 4);
            switch (difficulty)
            {
                case 1:
                    this->factor = EASY; 
                    break;
                case 2:
                    this->factor = NORMAL;
                    break;
                case 3:
                    this->factor = MEDIUM;
                    break;
                case 4:
                    this->factor = HARD;
                    break;
            }
        }
        float ledsOff(){
            // manage the timer t1 : the leds L1…L4 are turned off for some random time T1 
            this->timer_t1 = ((rand() % 10) + 1);
            return this->timer_t1;
        }
        float showPattern(){
            //manage the timer t2 : the leds are then turned on according to some  random pattern, for some time T2 and then turned off again 
            this->timer_t2 =  ((rand() % 10) + 1);
            return this->timer_t2;
        }
        float beginGame(){
            //manage the timer t3 : the player has max T3 time for recreating the pattern by pressing the buttons T1…T4 
            this->timer_t3 =  ((rand() % 10) + 1);
            return this->timer_t3;
        }
        void reduceTimers(){
            this->timer_t2 = this->timer_t2 - this->factor > 0 ? this->timer_t2 - this->factor : 0;
            this->timer_t3 = this->timer_t3 - this->factor > 0 ? this->timer_t3 - this->factor : 0;
        }
};