#include<ESP8266WiFi.h>

WiFiClient wcl;

const char* address = "192.168.1.65";    //Observation point server address.
uint16_t port = 3001;                  //Observation point server port.

void setup() {
  Serial.begin(9600); //Data will be received from the default HardwareSerial object
  WiFi.mode(WIFI_STA);
  WiFi.begin("INFINITUM13A1_2.4","aS7DP89W8m");
  if (WiFi.status() != WL_CONNECTED) {
    //Don't send anything to the Arduino Hardware Serial port
    delay(500);
  }
  if (wcl.connect(address, port)) {
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
    wcl.connect(address, port);
    delay(1000);
  }
  delay(100);
}
