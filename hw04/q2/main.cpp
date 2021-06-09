#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

using namespace std;

Ticker servo_ticker;
Timer t;

PwmOut pin5(D5), pin6(D6);

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx

void lindect(Arguments *in, Reply *out);
RPCFunction linerpc(&lindect, "lindect");

BBCar car(pin5, pin6, servo_ticker);

int main(){
   
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&pc, "r");
   FILE *devout = fdopen(&pc, "w");


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
      printf("%s\r\n", outbuf);
   
   }

}


void lindect(Arguments *in, Reply *out){

   uart.set_baud(9600);

   bool quota = true;

   while(uart.readable() & quota){
   
         char recv[1];
         uart.read(recv, sizeof(recv));
         pc.write(recv, sizeof(recv));

         quota++;

   }

   ThisThread::sleep_for(1s);



}