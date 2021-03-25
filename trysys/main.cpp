#include "mbed.h"

DigitalIn mypin1(PB_2);  //D8
DigitalIn mypin2(PB_1);  //D6

DigitalOut myled2(LED3);
DigitalOut myled1(LED1);


int main(){

    while(1) {

        myled1.write(mypin1.read());
        ThisThread::sleep_for(250ms);

        myled2.write(mypin2.read());
        ThisThread::sleep_for(250ms);

        } 


    }


