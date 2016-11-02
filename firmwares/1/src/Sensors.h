/* IMPORTANT SIDENOTE
 *  Schematics show that antenna 1 should be connected to pin 25, pin 2 to 26 etc.
 *  In "real life" they are routed to the opposite pins AND the pins are mirrored!
 *  They are mirrored because the antenna 'S4' runs to MTI0 on the chip, S3 to MTI1, S2 to MTI3, S1 to MTI4
 *  So antenna 1 now routes to pin 14, antenna 2 to pin 15, 3 = 16 and 4 = 17.
 */
 
#ifndef Sensors_h
#define Sensors_h

#include <Arduino.h>

class Sensors {
  public:
    Sensors();
    void read(boolean[3][2]); // if not working, try to erase 3
  private:
};
#endif
