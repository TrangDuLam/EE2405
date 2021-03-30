#include "mbed.h"

AnalogOut aout(PA_4);  //D7
AnalogIn Ain(A0);  //D1

const double peak = 0.91f;
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

      float fqidx = 0.0001 * 3;

      while(1){
            
            //using J~~~
            for (j = 0; j < sample; j++){
                wave(fqidx);
                ADCdata[j] = Ain;
                ThisThread::sleep_for(10ms/sample);
              }
            
            for (j = 0; j < sample; j++){
                printf("%f\r\n", ADCdata[j]);
                ThisThread::sleep_for(1ms);
             }
            }
    }



//0.001 => 50Hz
