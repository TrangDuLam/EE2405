#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);
BufferedSerial pc(USBTX, USBRX);
DigitalIn encoder_right(D10);
DigitalIn encoder_left(D9);

Timer t;
Ticker encoder_ticker_right;
Ticker encoder_ticker_left;

void rotation(Arguments *in, Reply *out);
RPCFunction rotationrpc(&rotation, "rott");

BBCar car(pin5, pin6, servo_ticker);

volatile int steps_right, steps_left;
volatile int last_right, last_left;

void encoder_control_left(){
    int value_left = encoder_left;
    if (!last_left && value_left) steps_left++;
    last_left = value_left;
}

void encoder_control_right() {
    int value_right = encoder_right;
    if (!last_right && value_right) steps_right++;
    last_right = value_right;
}


int main() {
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
      RPC::call(buf, outbuf);
   }
}

void rotation(Arguments *in, Reply *out){


   pc.set_baud(9600);

   encoder_ticker_right.attach(&encoder_control_right, .01);
   encoder_ticker_left.attach(&encoder_control_left, .01);

   t.reset();
   t.start();

   ThisThread::sleep_for(1500ms);

   float time = t.read();

   printf("left : %1.3f\r\n", (float)steps_left * 6.5 * 3.14 / 32 / time);
   printf("right : %1.3f\r\n", (float)steps_right * 6.5 * 3.14 / 32 / time);



}