#include <RadioLib.h>

float lat = 45.2123, lng = -52.3121;
const int siz = 8;
static uint8_t data[siz];  //Save the position into this array

RFM95 radio = new Module(10, 2, 5, 6);

int transmissionState = ERR_NONE;

void setup() {
  //Serial.begin(9600);
  //Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.begin();
  if (state == ERR_NONE) {
    //Serial.println(F("success!"));
  } else {
    //Serial.print(F("failed, code "));
    //Serial.println(state);
    //Serial.print(F("Restart the device"));
    while (true);
  }
  radio.setDio0Action(setFlag);
  //Serial.print(F("[SX1278] Sending first packet ... "));
  sendPosition();
}

volatile bool transmittedFlag = false;
volatile bool enableInterrupt = true;

void setFlag(void) {
  if(!enableInterrupt) {
    return;
  }
  transmittedFlag = true;
}

void loop() {
  
  if(transmittedFlag) {
    enableInterrupt = false;
    transmittedFlag = false;
    if (transmissionState == ERR_NONE) {
      // packet was successfully sent
      //Serial.println(F("transmission finished!"));
    } else {
      //Serial.print(F("failed, code "));
      //Serial.println(transmissionState);

    }
    delay(4000);

    //Serial.print(F("[SX1278] Sending another packet ... "));
    sendPosition();
    enableInterrupt = true;
    lat+=1;
    lng+=1;
  }
}

void sendPosition(){
  long _lat = lat*1000000.0;
  long _lng = lng*1000000.0;
  char dout[40];
  sprintf(dout, "lat:%ld,lng:%ld",_lat,_lng);
  transmissionState = radio.startTransmit(dout,sizeof(dout));
}
