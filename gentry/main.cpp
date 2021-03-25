#include "mbed.h"

AnalogOut aout(PA_4);

const double peak = 0.91f;

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

      float fqidx = 0.001 * 3;

      while(1){
            wave(fqidx);
            }
    }



//0.001 => 50Hz