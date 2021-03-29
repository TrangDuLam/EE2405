#include "mbed.h"
#include "uLCD_4DGL.h"

DigitalIn mypin1(PB_2);  //D8
DigitalIn mypin2(PB_1);  //D6
DigitalIn mypin3(PB_0);  //D3
//uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;

AnalogOut aout(PA_4);  //D7

const double peak = 0.91f;

AnalogIn Ain(A0);  //D1

int sample = 128;
int j;
 
float ADCdata[128];

void wave(float freq){

      for(float i=0.0; i<1.0; i+=freq){

            if(i < 0.8){
                  aout = i * peak / 0.8;
            }

            else if (i > 0.8 && i < 1.0){
                  aout = -peak*(i-1)/0.2;
            }

      }


};

int main(){

    int i = 0, add, minus, permit;
    float fqidx;
    

    while(1){

        

        add = mypin1.read();
        minus = mypin2.read();
        

        i = i + add;
        i = i - minus;


        
        if (i <= 0){
            i = 0;
            
            printf("%d\r\n", i);
            /*
            uLCD.printf("lowest freq !\r\n");
            uLCD.text_width(4); //4X size text
            uLCD.text_height(4);
            uLCD.locate(1,2);
            uLCD.color(RED);
            */
        }
        else if (i >= 7){
            i = 7;
            
            printf("%d\r\n", i);
            /*
            uLCD.printf("highest freq !\r\n");
            uLCD.text_width(4); //4X size text
            uLCD.text_height(4);
            uLCD.locate(1,2);
            uLCD.color(RED);
            */
        }
        else{
            
            printf("%d\r\n", i);
            /*
            uLCD.text_width(4); //4X size text
            uLCD.text_height(4);
            uLCD.locate(1,2);
            uLCD.color(RED);
            */
        }
        
        permit = mypin3.read();

        while(permit){
            
            //printf("Output %d\r\n",i);
            /*
            uLCD.text_width(4); //4X size text
            uLCD.text_height(4);
            uLCD.locate(1,2);
            uLCD.color(RED);
            */
            fqidx = 0.1 * i;  //0.0001 = 5Hz
            wave(fqidx);

            
            //using J~~~
            for (j = 0; j < sample; j++){
                ADCdata[j] = Ain;
                ThisThread::sleep_for(1000ms/sample);
              }
            
            for (j = 0; j < sample; j++){
                printf("%f\r\n", ADCdata[j]);
                ThisThread::sleep_for(100ms);
             }
            
            
            if (mypin2.read()){   //cutting point
                permit = 0;
            }
            
            //uLCD.printf("\r\n");
        }

        ThisThread::sleep_for(809ms);


    }


}