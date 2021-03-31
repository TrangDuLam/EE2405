// uLCD-144-G2 basic text demo program for uLCD-4GL LCD driver library
//
#include "mbed.h"
#include "uLCD_4DGL.h"
 
uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;
 
int main()
 {    
    

    // basic printf demo = 16 by 18 characters on screen
    uLCD.color(BLUE);
    uLCD.printf("\n106061121\n"); //Default Green on black text
  
    uLCD.text_width(5); //4X size text
    uLCD.text_height(5);
    uLCD.color(GREEN);    
    for (int i=30; i>=0; --i) {
      uLCD.locate(1,2);
      uLCD.printf("%2d",i);
      ThisThread::sleep_for(5ms);
     }

    uLCD.background_color(0xFFFFFF);
 
 }
