/*
  This is an example sketch, it simply prints out the id to the serial monitor.
*/


#include <gwiot9741.h>
gwiot9741 rfid(2,3);
void setup() {
  Serial.begin(9600);
}
void loop() {
  if(rfid.newCardAvailable()){
    Serial.println(rfid.getID(), HEX);
  }

}
