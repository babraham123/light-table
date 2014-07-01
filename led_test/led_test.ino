//#define FORCE_SOFTWARE_SPI
//#define FORCE_SOFTWARE_PINS
#include "FastSPI_LED2.h"

///////////////////////////////////////////////////////////////////////////////////////////
//
// Move a white dot along the strip of leds.  This program simply shows how to configure the leds,
// and then how to turn a single pixel white and then off, moving down the line of pixels.
// 

// How many leds are in the strip?
#define NUM_LEDS 50
#define DATA_PIN 11
CRGB leds[NUM_LEDS];

void setup() {
  delay(2000);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop() {
  // Move a single white led 
  for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) 
  {
    leds[whiteLed] = CRGB::White;
    // Show the leds (only one of which is set to white, from above)
    FastLED.show();
  
    delay(200);
    leds[whiteLed] = CRGB::Black;
  }
}
