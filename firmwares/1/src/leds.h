#ifndef Leds_h
#define Leds_h

#include <Arduino.h>

class Leds {
  public:
    Leds();
    void update(byte* artnetValues); 
  private:
};
#endif

