#include "mbed.h"
 
DigitalOut myLED(LED1);
DigitalOut myLED2(LED3);

void Led(DigitalOut &ledpin,int time);

  int main()
  {

   myLED = 0;
   myLED2 = 1;

    while (true)
    {
      Led(myLED2, 3);
      myLED = 1;
      myLED2 = 0;
      ThisThread::sleep_for(1s);
      Led(myLED, 2);
      myLED = 0;
      myLED2=1;
      ThisThread::sleep_for(1s);
    }
   }
