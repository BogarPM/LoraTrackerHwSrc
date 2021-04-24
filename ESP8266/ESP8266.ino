#include<ESP8266WiFiMulti.h>

WiFiClient wcl;
ESP8266WiFiMulti wifiMulti;


const char* address1 = "192.168.1.1";    //Observation point server address //// Dev server address.
const char* address2 = "192.168.43.2";   //Observation point server address //// "Prodction" server address

uint16_t port = 3001;                  //Observation point server port.

void setup() {
  Serial.begin(9600); //Data will be received from the default HardwareSerial object
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP("TROYANO","8363269005");
  wifiMulti.addAP("SUDO","1122334455");
  //WiFi.begin("TROYANO","8363269005");
  while(wifiMulti.run(5000) != WL_CONNECTED) {
    //Don't send anything to the Arduino Hardware Serial port
    delay(500);
  }
  if (wcl.connect(address1, port)) {
  }
}

void loop() {
  if (Serial.available() > 0) {
    String txt = Serial.readStringUntil('\n');
    if(wcl.connected()){
      if(!txt.equals("\n")){
        wcl.print(txt);
      }
    }
  }

  if(!wcl.connected()){
    wcl.connect(address1, port);
    delay(1000);
  }
  delay(100);
}
