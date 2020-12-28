#include <RadioLib.h>

RFM95 radio = new Module(10, 2, 9, 6);

void setup() {
  Serial.begin(9600);
  //Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.begin();
  if (state == ERR_NONE) {
    //Serial.println(F("success!"));
  } else {
    //Serial.print(F("failed, code "));
    //Serial.println(state);
    while (true);
  }
  radio.setDio0Action(setFlag);
  //Serial.print(F("[SX1278] Starting to listen ... "));
  state = radio.startReceive();
  if (state == ERR_NONE) {
    //Serial.println(F("success!"));
  } else {
    /*Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);*/
  }
}

volatile bool receivedFlag = false;
volatile bool enableInterrupt = true;

void setFlag(void) {
  if(!enableInterrupt) {
    return;
  }
  receivedFlag = true;
}

void loop() {
  if(receivedFlag) {
    enableInterrupt = false;
    receivedFlag = false;
    String str;
    uint8_t datain[8];
    int state = radio.readData(str);
    if (state == ERR_NONE) {
      //Serial.println("Position received");  //Receive the position data as a string: lat:x.xxx,lng:x.xxx;
      Serial.println(str);
    } else if (state == ERR_CRC_MISMATCH) {
      //Serial.println(F("[SX1278] CRC error!"));
    } else {
      //Serial.print(F("[SX1278] Failed, code "));
      //Serial.println(state);
    }
    radio.startReceive(8);  //Receive 8 bytes
    enableInterrupt = true;
  }

}
