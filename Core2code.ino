//Core2: 
void Core2code( void * pvParameters ){
  //Serial.print("Task2 running on core ");
  //Serial.println(xPortGetCoreID());

  autoMode=1;
  SectMainToAOG = 1;

  for(;;){
   
  currentTime = millis();
  //unsigned int time = currentTime;

  if (currentTime - lastTime >= LOOP_TIME)
  {
    lastTime = currentTime;

    //increase the watchdog - reset in data recv.
    watchdogTimer++;

    //Save previous state
    sectionOld[0] = sectionOut[0];
    sectionOld[1] = sectionOut[1];
    sectionOld[2] = sectionOut[2];
    sectionOld[3] = sectionOut[3];
  
  #if IOMode == 0   // Digital I/O
    sectionOut[0] = 0x00;
    sectionOut[1] = 0x00;
    sectionOut[2] = 0x00;
    
    if (digitalRead(GPSAuto)==1) sectionOut[3] = 0x00;
    if (digitalRead(GPSAuto)==0) sectionOut[3] = 0x80; 
    
    if (digitalRead(Sw1)==0) sectionOut[0] |= 1; else sectionOut[0] &= B11111110;
    if (digitalRead(Sw2)==0) sectionOut[0] |= 2; else sectionOut[0] &= B11111101;
    if (digitalRead(Sw3)==0) sectionOut[0] |= 4; else sectionOut[0] &= B11111011;
    if (digitalRead(Sw4)==0) sectionOut[0] |= 8; else sectionOut[0] &= B11110111;
    if (digitalRead(Sw5)==0) sectionOut[0] |= 16;else sectionOut[0] &= B11101111;
  
  #endif
     
  #if IOMode == 1   //AGOpenGPS     
    sectionOut[0] = relayLo;
    sectionOut[1] = relayHi; 

    sectionOut[0] |= sectionOverride[0];
    SectSWOffToAOG[0] = sectionOverride[0];
    
    if (gotBackSect) {
      //Serial.println(" Override went down ");
      //RelayToAOG[0] = sectionOverrideOld[0];  // Back On again
      //SectMainToAOG = 0;
      sectionOverrideOld[0] = sectionOverride[0];  
      gotBackSect =0;// reset Flag
      delay(20);
     }
    else RelayToAOG[0] = 0;
   #endif
        
  #if (IOMode >= 1) //AOG + Cerea
    //clean out serial buffer
    if (watchdogTimer > 10)
    {
      //DbgSerial.println("No response from AOG or Cerea");
      sectionOut[0] = 0x00; // turn off all Sections
      sectionOut[1] = 0x00;
      sectionOut[2] = 0x00;
      sectionOut[3] = 0x00; // also turn off GPS Mode!
    }
    else sectionOut[3] |= 0x80; // turn on GPS Mode!
  #endif
      
    // compare to previous state
    if (sectionOut[0] != sectionOld[0]) sectChange = true;
    if (sectionOut[1] != sectionOld[1]) sectChange = true;
    if (sectionOut[2] != sectionOld[2]) sectChange = true;
    if (sectionOut[3] != sectionOld[3]) sectChange = true;   
    
    //Serial.println(sectionOut[3],HEX);*/
    //rateAppliedLPMLeft = rateAct_i;
    //rateAppliedLPMRight = rateAct_i;

   #if IOMode == 1   //AGOpenGPS  
      transmit_AOG();
   #endif
  } //end of timed loop

   #if IOMode == 1   //AGOpenGPS 
    receive_AOG();
   #endif  
  //delay(10);  
  } // end of loop core2
} // end of Core2
