#include "mbed.h"

DigitalIn mypin1(PB_2);  //D8
DigitalIn mypin2(PB_1);  //D6
DigitalIn mypin3(PB_0);  //D3


int main(){

    int i = 0, add, minus;

    while(1){

        add = mypin1.read();
        minus = mypin2.read();

        i = i + add;
        i = i - minus;

        if (i < 1){
            printf("%d\r\n", i);
            printf("lowest freq !\r\n");

            i = 0;
        }
        else if (i > 6){
            printf("%d\r\n", i);
            printf("highest freq !\r\n");

            i = 7;
        }
        else{
            printf("%d\r\n", i);
        }
        ThisThread::sleep_for(1618ms);


    }


}