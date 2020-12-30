#include <RadioLib.h>
#include<TinyGPS.h>
#include<SoftwareSerial.h>

TinyGPS gps;
SoftwareSerial ss(4,3);

int count = 0;

float flat = 0, flon = 0;
unsigned long age = 0;

bool gpsin = false;               //Tells when new gps data is available
bool receivedPosition = false;    //Position latch, set to true when actual position is read.
bool latch = true;    //Latch for sending the first message just once.


//float lat = 45.2123, lng = -52.3121;
const int siz = 8;
static uint8_t data[siz];  //Save the position into this array

RFM95 radio = new Module(10, 2, 5, 6);

int transmissionState = ERR_NONE;

void setup() {
  //Serial.begin(9600);
  ss.begin(9600);
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

volatile bool transmittedFlag = true;
volatile bool enableInterrupt = true;

void setFlag(void) {
  if(!enableInterrupt) {
    return;
  }
  transmittedFlag = true;
}

///////////////////////////////////





void loop() {
  while(ss.available()>0){
    char c = ss.read();
    if(gps.encode(c)){
      gpsin = true;
    }
  }
  if(gpsin){
    gps.f_get_position(&flat, &flon, &age);
    receivedPosition = true;
  }
  gpsin = false;
  
  if(transmittedFlag) {
    enableInterrupt = false;
    transmittedFlag = false;
    if(receivedPosition){}
    if (transmissionState == ERR_NONE){}    //No error at transmit 
    else {}
    delay(10);
    if(count >=99 && receivedPosition){
      sendPosition();
      count = 0;
    }else if(!transmittedFlag){
      //setFlag();    //simulate a send event when timer has not finished yet
    }
    enableInterrupt = true;
  }
  if(count >=99){
      transmittedFlag = true;
  }
  count++;
  delay(200);
}

void sendPosition(){
  if(receivedPosition){
    long _lat = flat*100000000, _lng = flon*100000000;
    char dout[40];
    sprintf(dout, "lat:%ld,lng:%ld",_lat,_lng);
    transmissionState = radio.startTransmit(dout,sizeof(dout));
  }else{
    if(latch){
      transmissionState = radio.startTransmit("t");
      latch = false;
    }
  }
}
