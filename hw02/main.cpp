#include "mbed.h"
#include "uLCD_4DGL.h"

DigitalIn mypin1(PB_2);  //D8
DigitalIn mypin2(PB_1);  //D6
DigitalIn mypin3(PB_0);  //D3
uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;

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
            
            uLCD.text_width(4); //4X size text
            uLCD.text_height(4);
            uLCD.locate(0, 0);
            uLCD.printf("%d\r\n", i);
            uLCD.locate(0, 3);
            uLCD.printf("N");
            uLCD.locate(1, 3);
            uLCD.printf("O");
            uLCD.locate(2, 3);
            uLCD.printf("N");

            /*
            uLCD.text_width(1); //4X size text
            uLCD.text_height(1);
            uLCD.locate(7, 7);
            uLCD.printf("lowest freq !\r\n");
            */
            uLCD.color(RED);
            
        }
        else if (i >= 7){
            i = 7;
            uLCD.text_width(4); //4X size text
            uLCD.text_height(4);
            uLCD.locate(3, 1);
            uLCD.printf("%d\r\n", i);
            uLCD.locate(0, 3);
            uLCD.printf("H");
            uLCD.locate(1, 3);
            uLCD.printf("I");
            uLCD.locate(2, 3);
            uLCD.printf("G");
            uLCD.locate(3, 3);
            uLCD.printf("H");

            
            /*
            uLCD.text_width(1); //4X size text
            uLCD.text_height(1);
            uLCD.locate(7, 7);
            uLCD.printf("highest freq !\r\n");
            */
            
            uLCD.color(RED);
            
        }
        else{
    
            if (i > 0 && i < 4){
                uLCD.text_width(4); //4X size text
                uLCD.text_height(4);
                uLCD.locate(i,0);
                uLCD.printf("%d\r\n", i);
                uLCD.color(RED);
            }

            

            
            else if (i > 3 && i < 7){
                
                uLCD.text_width(4); //4X size text
                uLCD.text_height(4);
                uLCD.locate(i - 4, 1);
                uLCD.printf("%d\r\n", i);
                uLCD.color(RED);

            }
            
            
        }
        
        permit = mypin3.read();

        while(permit){
            permit = 1;
            int row, col;

            row = i % 4;
            col = i / 4;
            
            uLCD.cls();
            uLCD.text_width(4); //4X size text
            uLCD.text_height(4);
            uLCD.locate(row , col);
            uLCD.printf("%d\r\n", i);
            uLCD.locate(2, 2);
            uLCD.printf("O");
            uLCD.locate(3, 2);
            uLCD.printf("N");
            
            
            
            uLCD.color(RED);
            
            fqidx = 0.0001 * i;  //0.0001 = 5Hz
            

            
            //using J~~~
            for (j = 0; j < sample; j++){
                wave(fqidx);
                ADCdata[j] = Ain;
                ThisThread::sleep_for(1000ms/sample);
              }
            
            for (j = 0; j < sample; j++){
                printf("%f\r\n", ADCdata[j]);
                ThisThread::sleep_for(100ms);
             }
            
            
            if (mypin2.read()){   //cutting point
                break;
            }
            
            //uLCD.printf("\r\n");
        }


        

        ThisThread::sleep_for(809ms);
        uLCD.cls();

    }


}
