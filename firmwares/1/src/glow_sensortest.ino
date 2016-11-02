#include "Sensors.h"
#include "Leds.h"
#include "Artnet.h"
#include "MQTT_Ethernet.h"

#include <Ethernet.h>
#include <EthernetClient.h>
#include <Dns.h>
#include <Dhcp.h>

/* -----Configuration::------------ */
/* -------------------------------- */
IPAddress iotIP (10,0,0,$lastOctet);  // TODO: last octet should be teensy index (0 based) + 3
/* -------------------------------- */
/* -NEXT CHANGE IN FILE Artnet.cpp- */


Sensors *sensors;

Leds *leds;

Artnet *artnet;

MQTT_Ethernet *mqtt_ethernet;

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x$lastMinusOneHex, 0x$lastHex};

void setup() { 
  Serial.begin(115200);
  
  Ethernet.begin(mac, iotIP);
  delay(1000); //give the ethernet a second to initialize

  sensors = new Sensors();

  leds = new Leds();

  mqtt_ethernet = new MQTT_Ethernet();

  artnet = new Artnet();
} 

//customisation: edit this if you want for example read and copy only 4 or 6 channels from channel 12 or 48 or whatever.
const int numTubes = 3;
const int numTouchAreas = 2;
const int total_number_of_channels = 1008; //1008 channels (112*3 controllable leds, 112 * 3 * 3)
const int start_address = 0; // 0 if you want to read from channel 1
byte rgb[total_number_of_channels]; // buffer to hold values for all leds

boolean touchData[numTubes][numTouchAreas];
boolean currentTouchState[numTubes][numTouchAreas];
int counter[numTubes][numTouchAreas];       // how many times we have seen new touchState

// the following variable is a long because the time, measured in milliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long time = 0;         // the last time the output pin was sampled
int debounce_count = 2; // number of millis/samples to consider before declaring a debounced input

void loop() {

  if(millis() != time) {  // If we have gone on to the next millisecond
    sensors->read(touchData);
    //Serial.println("tD " + String(touchData[0][0]) + "   cTS" + String(currentTouchState[0][0]) + "   counter" + String(counter[0][0]));
    //Serial.println("cTS" + String(currentTouchState[0][1]));
//    Serial.println("tube = " + String(numTubes));
//    Serial.println("sides = " + String(numTouchAreas));
    
    for (int tube = 0; tube < numTubes; tube ++) {
      for (int side = 0; side < numTouchAreas; side ++) {
//        if(touchData[tube][side] == currentTouchState[tube][side] && counter[tube][side] > 0) {
//          counter[tube][side]--;
//          //Serial.println("no change");
//        }
        if(touchData[tube][side] != currentTouchState[tube][side]) {
          //counter[tube][side]++;
          currentTouchState[tube][side] = touchData[tube][side];
          mqtt_ethernet->send_touchState(tube, side, touchData[tube][side]);
          //Serial.println("change");
        }
        // If the Input has shown the same value for long enough let's switch it
        if(counter[tube][side] >= debounce_count) {
          counter[tube][side] = 0;
          currentTouchState[tube][side] = touchData[tube][side];
          mqtt_ethernet->send_touchState(tube, side, touchData[tube][side]);
        }
      }
    }
    time = millis();
  }

  artnet->receive_artnet(rgb, start_address, total_number_of_channels);

  leds->update(rgb);
}
