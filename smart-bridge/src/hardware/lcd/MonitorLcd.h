#ifndef __MONITORLCD__
#define __MONITORLCD__

#include <LiquidCrystal_I2C.h>

class MonitorLcd{
    private:
        LiquidCrystal_I2C* lcd;
        int address;
        int rows;
        int cols;
    public:
        MonitorLcd(int address, int rows, int cols);
        void init();
        void displayPreAlarm(double level);
        void displayAlarm(double level, int op_degree);
        void displayON();
        void displayOFF();
};

#endif