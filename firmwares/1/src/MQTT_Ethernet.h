/***************************************************
  Adafruit MQTT Library Ethernet Example

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Alec Moore
  Derived from the code written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

 
#ifndef MQTT_Ethernet_h
#define MQTT_Ethernet_h

#include <Arduino.h>

class MQTT_Ethernet {
  public:
    MQTT_Ethernet();
    void send_touchState(int tube, int side, boolean touchState);
  private:
    void MQTT_connect();
};
#endif
