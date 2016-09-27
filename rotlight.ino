#include <Encoder.h>
#include <FastLED.h>

#define NUM_LEDS 7
#define D_PIN 6

CRGB leds[NUM_LEDS];

Encoder enc(2, 3);

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, D_PIN>(leds, NUM_LEDS);
}

void loop() {
  long maxLed = enc.read() / 4;
  Serial.println(maxLed);
  
  if (maxLed >= NUM_LEDS) {
    maxLed = NUM_LEDS;
    enc.write(maxLed * 4);
  } else if (maxLed < 0) {
    maxLed = 0;
    enc.write(maxLed);
  }

  FastLED.clear();
  
  for (int i = 0; i < maxLed; i++) {
    leds[i] = CRGB::White;
  }
  
  FastLED.show();
}
    
  
