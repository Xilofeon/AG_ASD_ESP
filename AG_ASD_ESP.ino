TaskHandle_t Core1;
TaskHandle_t Core2;
// ESP32 ASD Host by Coffeetrac
// Release: V1.00
// 10.01.2019 W.Eder
//  

 //##########################################################################################################
  //### Setup Zone ###########################################################################################
  // Just now,  only default values, will be overwritten by the Web config site
  //##########################################################################################################

  #define IOMode 1          // 0 = I/O from Portpins 19 18 12 13
                            // 1 = I/O from AGOpenGPS 

  //##########################################################################################################
  //### End of Setup Zone ####################################################################################
  //##########################################################################################################

// IO pins --------------------------------
#define RX1     6 
#define TX1     7 

/*#define GPSAuto   34
#define Sw1       19  // Section 1 (left)
#define Sw2       18  // Section 2 (right)
#define Sw3       14  // 
#define Sw4       27
#define Sw5        5
*/

//#include <Wire.h>
#include "var.h"

#if IOMode == 1   //AGOpenGPS   
  #define Baudrate 38400
#endif

// ASD Host variables
  int i=0, in = 0;  
  const char escMask = 0x10;
  const char startByte = 0x02;
  const char stopByte = 0x04;
  uint8_t ASDBuffer[20], c=0;
  String CereaBuf;
  uint8_t prevbyte=0;
  bool newSentence = false, newSet =false;
  uint8_t sectionOut[4] = {0x00,0x00,0x00,0x80};
  uint8_t sectionSent[4] = {0,0,0,0x80};
  uint8_t sectionOverride[4] = {0,0,0,0};
  uint8_t sectionOverrideOld[4] = {0,0,0,0};
  uint8_t sectionOld[4] = {0,0,0,0x80};
  uint8_t sectionReturn[4] = {0,0,0,0x80};
  uint8_t sectionRetOld[4] = {0,0,0,0x80};
  unsigned long Sections=0;
  uint8_t rateAct[3] ={0,0,0};
  int  rateAct_i = 0;
  
  byte toolID[2] = {0x05, 0x00};

// Program flow
  unsigned long lastInitTime;
  unsigned long lastInitResponse;
  unsigned long lastReqTime;
  unsigned long lastSectReqTime;
  unsigned long lastSectTime;
  
  int steps =0;
  bool gotInit = false;
  bool sectChange = false;
  bool firstInit = true;
  bool gotBackSect =false;
// Instances ------------------------------

#define SC_Serial Serial
//#define DbgSerial Serial2

// Setup procedure ------------------------
void setup() {
  SC_Serial.begin(Baudrate);                        // AGOPENGPS or Cerea
  Serial1.begin (19200, SERIAL_8N1, RX1, TX1); // ASD Connector to ASD Machine Terminal
  
/*   pinMode(GPSAuto, INPUT_PULLUP);             // GND=Unconnected = GPSControl ON // 3,3V = GPSControl OFF
   pinMode(Sw1, INPUT_PULLUP);
   pinMode(Sw2, INPUT_PULLUP); 
   pinMode(Sw3, INPUT_PULLUP);
   pinMode(Sw4, INPUT_PULLUP); */
//------------------------------------------------------------------------------------------------------------  
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Core1code,   /* Task function. */
                    "Core1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Core1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Core2code,   /* Task function. */
                    "Core2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Core2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
  delay(500); 
}

void loop() {}
