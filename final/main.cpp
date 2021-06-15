#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

using namespace std;

Ticker servo_ticker;
Timer t;

PwmOut pin5(D5), pin6(D6);
BufferedSerial pc(USBTX, USBRX);
BufferedSerial xbee(D1, D0); //tx, rx
DigitalInOut ping(D11);

void ultrasound(Arguments *in, Reply *out);
RPCFunction ultrarpc(&ultrasound, "ultra");
//void mvision(Arguments *in, Reply *out);
//RPCFunction mvisionrpc(&mvision, "mv");

BBCar car(pin5, pin6, servo_ticker);


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

void ultrasound(Arguments *in, Reply *out){

   float val;
   pc.set_baud(9600);


   ping.output();
   ping = 0; wait_us(200);
   ping = 1; wait_us(5);
   ping = 0; wait_us(5);

   ping.input();
   while(ping.read() == 0);
   t.start();
   while(ping.read() == 1);
   val = t.read();
   
   printf("\n Ping = %lf \r\n", val*17700.4f);
   t.stop();
   t.reset();

   ThisThread::sleep_for(1s);



}

/*
void mvision(Arguments *in, Reply *out){

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
*/
