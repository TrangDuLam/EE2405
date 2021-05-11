#include "mbed.h"
#include "mbed_rpc.h"

#include "MQTTNetwork.h"
#include "MQTTmbed.h"
#include "MQTTClient.h"
#include "stm32l475e_iot01_accelero.h"

#include "uLCD_4DGL.h"

#include "accelerometer_handler.h"
#include "config.h"
#include "magic_wand_model_data.h"
 
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include "mbed_events.h"
using namespace std::chrono;


// GLOBAL VARIABLES
WiFiInterface *wifi;
//InterruptIn btn2(USER_BUTTON);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;
//InterruptIn btn3(SW3);
volatile int message_num = 0;
volatile int arrivedcount = 0;
volatile bool closed = false;

const char* topic = "Mbed";

int16_t pDataXYZ[3] = {0};

Thread mqtt_thread(osPriorityHigh);
EventQueue mqtt_queue;

// RPC function group
RpcDigitalOut myled1(LED1,"myled1");
RpcDigitalOut myled2(LED2,"myled2");
RpcDigitalOut myled3(LED3,"myled3");


uLCD_4DGL uLCD(D1, D0, D2);

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalIn btn(USER_BUTTON);
EventQueue printfQueue;
EventQueue eventQueue;
// low priority thread for calling printf()
Thread printfThread(osPriorityLow);
// normal priority thread for other events
Thread eventThread(osPriorityLow);
int mode = 0;
int selectGes = -1;
int demoGes = -1;

char uLCD_gesture[3][10] = {"30degree", "45degree", "60degree"};

BufferedSerial pc(USBTX, USBRX);
void funcO(Arguments *in, Reply *out);
RPCFunction rpc01(&funcO, "funcO"); //string cmp user's input
void funcT(Arguments *in, Reply *out);
RPCFunction rpc02(&funcT, "funcT"); //RPCFunction name cannot be the same

void GestureControl();
void AngleDetect();


//tensorflow declaration
bool should_clear_buffer = false;
bool got_data = false;
 
// The gesture index of the prediction
int gesture_index;

// Set up logging.
static tflite::MicroErrorReporter micro_error_reporter;
tflite::ErrorReporter* error_reporter = &micro_error_reporter;

// Create an area of memory to use for input, output, and intermediate arrays.
// The size of this will depend on the model you're using, and may need to be
// determined by experimentation.
constexpr int kTensorArenaSize = 60 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
// Return the result of the last prediction
int PredictGesture(float* output);
TfLiteTensor* model_input;
  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  const tflite::Model* model = tflite::GetModel(g_magic_wand_model_data);

  static tflite::MicroOpResolver<6> micro_op_resolver;
// Build an interpreter to run the model with
  static tflite::MicroInterpreter static_interpreter(

      model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);

  tflite::MicroInterpreter* interpreter = &static_interpreter;



//MQTT part
void messageArrived(MQTT::MessageData& md) {
      MQTT::Message &message = md.message;
      char msg[300];
      sprintf(msg, "Message arrived: QoS%d, retained %d, dup %d, packetID %d\r\n", message.qos, message.retained, message.dup, message.id);
      printf(msg);
      ThisThread::sleep_for(1000ms);
      char payload[300];
      sprintf(payload, "Payload %.*s\r\n", message.payloadlen, (char*)message.payload);
      printf(payload);
      ++arrivedcount;
  }

void publish_message(MQTT::Client<MQTTNetwork, Countdown>* client) {
      message_num++;
      MQTT::Message message;
      char buff[100];
      BSP_ACCELERO_AccGetXYZ(pDataXYZ);
      sprintf(buff, "Qos0 %d, %d, %d", pDataXYZ[0], pDataXYZ[1], pDataXYZ[2]);
      message.qos = MQTT::QOS0;
      message.retained = false;
      message.dup = false;
      message.payload = (void*) buff;
      message.payloadlen = strlen(buff) + 1;
      int rc = client->publish(topic, message);

}

void close_mqtt() {
      closed = true;
}




//uLCD group
void uLCD_illu_anagle(){

  uLCD.cls();
  uLCD.color(0x00ff00); //otherwise, default is green
  uLCD.text_width(2); //4X size text
  uLCD.text_height(2);
  printf("angle detection\r\n");

  if (pDataXYZ[2] > 855){
        uLCD.locate(2, 2);
        uLCD.printf("\n%d\n","< 30");
      }

      else if (pDataXYZ[2] > 699 && pDataXYZ[2] < 855){
        sprintf(buff, "30 ~ 45");
      }

      else if(pDataXYZ[2] > 494 && pDataXYZ[2] < 699){
        sprintf(buff, "45~60");
      }

      else {
        sprintf(buff, "> 60");
      }
  
  
  }
}

void uLCD_illu_gesture(){

  uLCD.color(0x00ff00); //otherwise, default is green
  uLCD.text_width(2); //4X size text
  uLCD.text_height(2);
  printf("enter uLCD_illu_gesture\r\n");
  
  uLCD.locate(2, 1); // i specify rows
  uLCD.printf("\ncomfirm?\n");

  uLCD.locate(2, 3); // i specify rows
  if (demoGes==0)
    uLCD.printf("\n15\n");
  else if (demoGes==1)
    uLCD.printf("\n60\n");
}


 
int main() {

   BSP_ACCELERO_Init();

   wifi = WiFiInterface::get_default_instance();

   if (!wifi) {
              printf("ERROR: No WiFiInterface found.\r\n");
              return -1;
   }


   printf("\nConnecting to %s...\r\n", MBED_CONF_APP_WIFI_SSID);
   int ret = wifi->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
   if (ret != 0) {
              printf("\nConnection error: %d\r\n", ret);
              return -1;
   }


   NetworkInterface* net = wifi;
   MQTTNetwork mqttNetwork(net);
   MQTT::Client<MQTTNetwork, Countdown> client(mqttNetwork);

   //TODO: revise host to your IP
   const char* host = "172.20.10.2";
   printf("Connecting to TCP network...\r\n");

   SocketAddress sockAddr;
   sockAddr.set_ip_address(host);
   sockAddr.set_port(1883);

   printf("address is %s/%d\r\n", (sockAddr.get_ip_address() ? sockAddr.get_ip_address() : "None"),  (sockAddr.get_port() ? sockAddr.get_port() : 0) ); //check setting

   int rc = mqttNetwork.connect(sockAddr);//(host, 1883);
   if (rc != 0) {
              printf("Connection error.");
              return -1;
   }

   printf("Successfully connected!\r\n");

   MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
   data.MQTTVersion = 3;
   data.clientID.cstring = "Mbed";

   if ((rc = client.connect(data)) != 0){
              printf("Fail to connect MQTT\r\n");
   }
   if (client.subscribe(topic, MQTT::QOS0, messageArrived) != 0){
              printf("Fail to subscribe\r\n");
   }

   mqtt_thread.start(callback(&mqtt_queue, &EventQueue::dispatch_forever));
   mqtt_queue.call_every(500ms, publish_message, &client);
   //btn3.rise(&close_mqtt);

   int num = 0;
   while (num != 5) {
              client.yield(100);
              ++num;
   }

   while (1) {
              if (closed) break;
              client.yield(500);
              ThisThread::sleep_for(500ms);
   }

   printf("Ready to close MQTT Network......\n");

   if ((rc = client.unsubscribe(topic)) != 0) {
              printf("Failed: rc from unsubscribe was %d\n", rc);
   }

   if ((rc = client.disconnect()) != 0) {
      printf("Failed: rc from disconnect was %d\n", rc);
   }

   mqttNetwork.disconnect();
   printf("Successfully closed!\n");

   return 0;

 

}

int PredictGesture(float* output) {
     // How many times the most recent gesture has been matched in a row
     static int continuous_count = 0;
     // The result of the last prediction
     static int last_predict = -1;
 
     // Find whichever output has a probability > 0.8 (they sum to 1)
     int this_predict = -1;
     for (int i = 0; i < label_num; i++) {
        if (output[i] > 0.8) this_predict = i;
     }
 
     // No gesture was detected above the threshold
     if (this_predict == -1) {
        continuous_count = 0;
        last_predict = label_num;
        return label_num;
     }
 
     if (last_predict == this_predict) {
        continuous_count += 1;
      } else {
        continuous_count = 0;
      }
      last_predict = this_predict;
 
      // If we haven't yet had enough consecutive matches for this gesture,
      // report a negative result
     if (continuous_count < config.consecutiveInferenceThresholds[this_predict]) {
        return label_num;
      }
      // Otherwise, we've seen a positive result, so clear all our variables
      // and report it
      continuous_count = 0;
      last_predict = -1;
 
       return this_predict;
}

void GestureControl(){
    
    led2 = led3 = 1; //start
    printf("Gesture queue\r\n");
    demoGes = -1; //init
    uLCD.background_color(0x000000); //white
    uLCD.cls();

    while(mode){
      
      if (!btn){
        if (demoGes == 0 || demoGes == 1){
          selectGes = demoGes;
          break;
        }
      }

      // Attempt to read new data from the accelerometer
      got_data = ReadAccelerometer(error_reporter, model_input->data.f,

                                  input_length, should_clear_buffer);


      // If there was no new data,

      // don't try to clear the buffer again and wait until next time

      if (!got_data) {

        should_clear_buffer = false;

        continue;

      }


      // Run inference, and report any error

      TfLiteStatus invoke_status = interpreter->Invoke();

      if (invoke_status != kTfLiteOk) {

        error_reporter->Report("Invoke failed on index: %d\n", begin_index);

        continue;

      }


      // Analyze the results to obtain a prediction

      gesture_index = PredictGesture(interpreter->output(0)->data.f);
      if (gesture_index != 2){
          demoGes = gesture_index;
          uLCD_illu_gesture();
      }

      //printf("demoGes: %d\r\n", demoGes); // show this during demo in classroom



      // Clear the buffer next time we read data

      should_clear_buffer = gesture_index < label_num;


      // Produce an output

      if (gesture_index < label_num) {

        error_reporter->Report(config.output_message[gesture_index]);
  
      }
    }

    led2 = LED3 = 0; //end
    printf("end gesture\r\n");


}

void AngleDetect(){

     led1 = 1;
     led3 = 0;

    uLCD.background_color(0x000000); //white
    uLCD.cls();
     
    BSP_ACCELERO_Init();
    led2 = 1; //start of tilt mode
    led3 = 0;
    BSP_ACCELERO_AccGetXYZ(pDataXYZ);


    led1 = 0;
    led3 = 1;

}

void funcO(Arguments *in, Reply *out){

    mode = in->getArg<double>();
    eventQueue.call(GestureControl);
    eventThread.start(callback(&eventQueue, &EventQueue::dispatch_forever));

}

void funcT (Arguments *in, Reply *out)   {

    mode = in->getArg<double>();
    printfQueue.call(AngleDetect);
    printfThread.start(callback(&printfQueue, &EventQueue::dispatch_forever));
}