#include<SoftwareSerial.h>

SoftwareSerial ss(4,3); //Rx,Tx
void setup() {
  Serial.begin(9600);
  ss.begin(9600);

}

void loop() {
  if(Serial.available()>0){
    ss.write(Serial.read());
  }
  if(ss.available()>0){
    Serial.write(ss.read());
  }
}
