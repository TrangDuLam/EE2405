#include "mbed.h"

DigitalIn mypin1(PB_2);  //D8
DigitalIn mypin2(PB_1);  //D6
DigitalIn mypin3(PB_0);  //D3


int main(){

    int i = 0, add, minus, permit;

    while(1){

        add = mypin1.read();
        minus = mypin2.read();
        

        i = i + add;
        i = i - minus;


        
        if (i <= 0){
            i = 0;
            printf("%d\r\n", i);
            printf("lowest freq !\r\n");
        }
        else if (i >= 7){
            i = 7;
            printf("%d\r\n", i);
            printf("highest freq !\r\n");
        }
        else{
            printf("%d\r\n", i);
        }
        
        permit = mypin3.read();

        while(permit){
            printf("Output %d\r\n",i);
            ThisThread::sleep_for(1s);
            permit = 0;
        }
    
        ThisThread::sleep_for(809ms);


    }


}