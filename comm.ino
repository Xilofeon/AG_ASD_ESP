void transmit_AOG(){
  //Send to agopenGPS, 5Hz per second
  
  //Send to AOG
  AOG[5] = (uint8_t)SectMainToAOG;
  AOG[9] = (uint8_t)RelayToAOG[0];
  AOG[10] = (uint8_t)SectSWOffToAOG[0];
  AOG[11] = (uint8_t)RelayToAOG[1];
  AOG[12] = (uint8_t)SectSWOffToAOG[1];
  
  //add the checksum
  int16_t CK_A = 0;
  for (uint8_t i = 2; i < sizeof(AOG)-1; i++)
  {
    CK_A = (CK_A + AOG[i]);
  }
  AOG[sizeof(AOG)-1] = CK_A;
  
  SC_Serial.write(AOG, sizeof(AOG));
  SC_Serial.flush();   // flush out buffer
}

//******************************************************************
void receive_AOG(){
  // Serial Receive
  
  //clean out serial buffer to prevent buffer overflow:
  if (serialResetTimer++ > 20) {
    while (SC_Serial.available() > 0) SC_Serial.read();
    serialResetTimer = 0;
    //noNewTone();
  }
  
  //Do we have a match with 0x8081?
  if (SC_Serial.available() > 4 && !isHeaderFound && !isPGNFound)
  {
    uint8_t temp = SC_Serial.read();
    if (tempHeader == 0x80 && temp == 0x81)
    {
      isHeaderFound = true;
      tempHeader = 0;
    }
    else
    {
      tempHeader = temp;     //save for next time
      return;
    }
  }

  //Find Source, PGN, and Length
  if (SC_Serial.available() > 2 && isHeaderFound && !isPGNFound)
  {
    SC_Serial.read(); //The 7F or less
    pgn = SC_Serial.read();
    dataLength = SC_Serial.read();
    isPGNFound = true;
  }

  //The data package
  if (SC_Serial.available() > dataLength && isHeaderFound && isPGNFound)
  {
    if (pgn == 239) // EF Machine Data
    {
      SC_Serial.read(); //5
      groundSpeed = SC_Serial.read() >> 2; //6
      SC_Serial.read(); //7
      SC_Serial.read(); //8
      SC_Serial.read(); //9
      SC_Serial.read(); //10

      relayLo = SC_Serial.read();          // read relay control from AgOpenGPS
      relayHi = SC_Serial.read();

      //Bit 13 CRC
      SC_Serial.read();

      //reset watchdog
      watchdogTimer = 0;
      
      //Reset serial Watchdog
      serialResetTimer = 0;
      
      //reset for next pgn sentence
      isHeaderFound = isPGNFound = false;
      pgn = dataLength = 0;
    }
    else if (pgn == 200) // Hello from AgIO
    {
      SC_Serial.read(); //Version
      SC_Serial.read();
      
      if (SC_Serial.read())
      {
        relayLo -= 255;
        relayHi -= 255;
        watchdogTimer = 0;
      }
    
      //crc
      SC_Serial.read();
      
      helloFromMachine[5] = relayLo;
      helloFromMachine[6] = relayHi;
      
      SC_Serial.write(helloFromMachine, sizeof(helloFromMachine));
      SC_Serial.flush();   // flush out buffer
      
      //reset for next pgn sentence
      isHeaderFound = isPGNFound = false;
      pgn = dataLength = 0;
    }
    else { //reset for next pgn sentence
      isHeaderFound = isPGNFound = false;
      pgn = dataLength = 0;
    }
  }
}
