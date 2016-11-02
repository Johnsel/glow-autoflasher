#ifndef Artnet_h
#define Artnet_h

#include <Arduino.h>

class Artnet {
  public:
    Artnet();
    void receive_artnet (byte*, int start_address, int number_of_channels);
  private:
    
};
#endif

