#include "mbed.h"
#include "uLCD_4DGL.h"

DigitalIn mypin1(PB_2);  //D8
DigitalIn mypin2(PB_1);  //D6
DigitalIn mypin3(PB_0);  //D3
uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;
AnalogOut aout(PA_4);  //D7



const double peak = 0.91f;





void wave(float sec) {

    for(float i = 0.0; i < 1.0; i = i + 0.004){   //1/240


        if (i < sec/3){
            aout = peak*3*sec*i;
        }

        else if (i > sec/3 && i < 1 - sec/3){
            aout = peak;
        }

        else{
            aout = -peak*(i-1)*3*sec;

        }
    }





}


void display(int i){
      
    if (i == 0){
        uLCD.text_width(4); //4X size text
        uLCD.text_height(4);
        uLCD.locate(0, 0);
        uLCD.printf("1");
        uLCD.locate(1, 0);
        uLCD.printf("/");
        uLCD.locate(2, 0);
        uLCD.printf("8");
    }

    if (i == 1){
        uLCD.text_width(4); //4X size text
        uLCD.text_height(4);
        uLCD.locate(0, 1);
        uLCD.printf("1");
        uLCD.locate(1, 1);
        uLCD.printf("/");
        uLCD.locate(2, 1);
        uLCD.printf("4");
    }

    if (i == 2){
        uLCD.text_width(4); //4X size text
        uLCD.text_height(4);
        uLCD.locate(0, 2);
        uLCD.printf("1");
        uLCD.locate(1, 2);
        uLCD.printf("/");
        uLCD.locate(2, 2);
        uLCD.printf("2");
    }

    if (i == 3){
        uLCD.text_width(4); //4X size text
        uLCD.text_height(4);
        uLCD.locate(0, 3);
        uLCD.printf("1");
    }





}





int main(){


    float selection[4] = {0.125, 0.24, 0.5, 1};
    int i = 0, add, minus;


    while(1){
        
        
        add = mypin1.read();
        minus = mypin2.read();

        i = i + add;
        i = i - minus;


        if (i <= 0){
            i = 0;
            display(i);
        }
        else if (i >= 3){
            i = 3;
            display(i);
        }
        else{
            display(i);
        }



        ThisThread::sleep_for(809ms);

        uLCD.cls();
    }



}