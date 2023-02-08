 //loop time variables in microseconds
  const unsigned int LOOP_TIME = 100; //10hz 
  unsigned int lastTime = LOOP_TIME;
  unsigned int currentTime = LOOP_TIME;
  uint8_t count = 0;
  uint8_t watchdogTimer = 0;
  uint8_t serialResetTimer = 0;   //if serial buffer is getting full, empty it

  uint8_t SectMainToAOG = 0;  // output the Switches to AOG
  uint8_t SectSWOffToAOG[]={0,0};
  uint8_t RelayToAOG[]={0,0};
  bool autoMode=0,autoModeold=0;
  
 //Parsing PGN
  bool isPGNFound = false, isHeaderFound = false;
  uint8_t pgn = 0, dataLength = 0;
  int16_t tempHeader = 0;
  
  //hello from AgIO
  uint8_t helloFromMachine[] = { 128, 129, 123, 123, 2, 1, 1, 71 };
  uint8_t AOG[] = {0x80, 0x81, 0x7B, 0xEA, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0xCC };
 
 //bit 0 is section 0
  uint8_t relayHi = 0, relayLo = 0;
  //float rateSetPointLeft = 0.0;
  //float rateSetPointRight = 0.0;

 //the ISR counter
  /*volatile unsigned long pulseCountLeft = 0, pulseDurationLeft;
  volatile unsigned long pulseCountRight = 0, pulseDurationRight;
 //Actual Applied rates
  int rateAppliedLPMLeft = 0;
  int rateAppliedLPMRight = 0;*/
  float groundSpeed = 0; //speed from AgOpenGPS is multiplied by 4

 //used to calculate delivered volume
  /*unsigned long accumulatedCountsLeft = 0;
  unsigned long accumulatedCountsRight = 0;
  float pulseAverageLeft = 0;
  float pulseAverageRight = 0;
  float flowmeterCalFactorLeft = 500;
  float flowmeterCalFactorRight = 500;*/
