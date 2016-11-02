#include "Arduino.h"
#include "Leds.h"

/*OctoWS2811 Notes:
 All 8 are the same length (112 WS2812B leds)
 
 Required Connections
 --------------------
 pin 2:  LED Strip #1    Master tube strip 1
 pin 14: LED strip #2    Master tube strip 2
 pin 7:  LED strip #3    Slave1 tube strip 1
 pin 8:  LED strip #4    Slave1 tube strip 2
 pin 6:  LED strip #5    Slave2 tube strip 1
 pin 20: LED strip #6    Slave2 tube strip 2
 pin 21: LED strip #7    NOT USED
 pin 5:  LED strip #8    NOT USED
 pin 15 & 16 - Connect together, but do not use
 pin 4 - Do not use
 pin 3 - Do not use as PWM.  Normal use is ok.*/

#define USE_OCTOWS2811
#include<OctoWS2811.h>
#include<FastLED.h>

#define numLedsPerStrip 112
#define numStrips 8
#define numLeds numLedsPerStrip * numStrips

CRGB ledArray[numLeds];

Leds::Leds() {
  LEDS.addLeds<OCTOWS2811>(ledArray, numLeds);

  FastLED.setMaxPowerInVoltsAndMilliamps(5,5700);
}

void Leds::update(byte* artnetValues) {
    for (int w = 0; w < numLedsPerStrip; w += 1) {
      ledArray[w] = ledArray[w + numLedsPerStrip]  = CRGB(artnetValues[w*3],    artnetValues[w*3+1],    artnetValues[w*3+2]);
    }
    for (int w = (numLedsPerStrip * 2) - 1; w < numLedsPerStrip * 4; w += 1) {
      ledArray[w] = ledArray[w + numLedsPerStrip] = CRGB(artnetValues[w*3],    artnetValues[w*3+1],    artnetValues[w*3+2]);
    }
    for (int w = (numLedsPerStrip * 2) - 1; w < numLedsPerStrip * 6; w += 1) {
      ledArray[w] = ledArray[w + numLedsPerStrip] = CRGB(artnetValues[w*3],    artnetValues[w*3+1],    artnetValues[w*3+2]);
    }
    FastLED.show();
}
