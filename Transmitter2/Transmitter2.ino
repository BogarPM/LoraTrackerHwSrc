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
  Serial.begin(9600);
  ss.begin(4800);
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
  //Serial.println("Loop");
  if(ss.available()>0){
    while(ss.available()>0){    //Read gps
      char c = ss.read();
      if(gps.encode(c)){
        gpsin = true;
      }
    }
    transmissionState = radio.startTransmit("r");
  }
  
  if(gpsin){
    gps.f_get_position(&flat, &flon, &age);
    Serial.println("GpsReceived");
    receivedPosition = true;
    sendPosition();
  }
  gpsin = false;
  
  if(transmittedFlag) {
    enableInterrupt = false;
    transmittedFlag = false;
    delay(10);
    if(receivedPosition){
      sendPosition();
    }
    count = 0;
    enableInterrupt = true;
  }
  if(count >=99){
      transmittedFlag = true;
      enableInterrupt = false;
      sendPosition();
      Serial.println("100 Ticks Reached");
      enableInterrupt = true;
  }
  count++;
  delay(200);
}

void sendPosition(){
  if(receivedPosition){
    long _lat = flat*100000000, _lng = flon*100000000;
    char dout[40];
    Serial.println("Sending position");
    sprintf(dout, "lat:%ld,lng:%ld",_lat,_lng);
    transmissionState = radio.startTransmit(dout,sizeof(dout));
    Serial.print("Lat: ");
    Serial.print(flat);
    Serial.print("    ");
    Serial.print("Long: ");
    Serial.print(flon);
  }else{
    if(latch){
      transmissionState = radio.startTransmit("t");
      latch = false;
    }else{
      Serial.println("send not received");
      transmissionState = radio.startTransmit("NR");
    }
  }
}
