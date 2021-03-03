#include "mbed.h"

void Led(DigitalOut &ledpin,int time)
  {
    for (int i = 0; i < time; ++i) 
      {                     //blink for 10 times
         ledpin = !ledpin; // toggle led
         ThisThread::sleep_for(100ms);
      }
  }
