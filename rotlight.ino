#include <Encoder.h>
#include <FastLED.h>

// Remove for final version
//#define DEBUG 1

#define NUM_LEDS 7
#define M_BUTTON_PIN 5
#define D_PIN 6

CRGB leds[NUM_LEDS];

// Available modes, mode can be change with the Encoder button
enum { M_HWIDTH, M_POS, M_RED, M_GREEN, M_BLUE };
int mode = M_HWIDTH;

// Save current encoder value (current setting) for every mode
int encval[6] = {0,NUM_LEDS/2,31,31,31};

// Maximum and minimum values for each mode
const int maxval[6] = {(NUM_LEDS/2) + 1, NUM_LEDS - 1, 31, 31, 31};
const int minval[6] = {0, 0, 0, 0, 0};

// Create encoder object
Encoder enc(2, 3);
long curEnc = 0;
long lastCurEnc = 1;

// Save last button state
int lastButtonState = LOW;
int buttonState = LOW;

// Function prototypes
void render(int start, int end, int r, int g, int b);

// Setup function
void setup() {
  pinMode(M_BUTTON_PIN, INPUT);
  pinMode(13, OUTPUT);
  
  FastLED.addLeds<NEOPIXEL, D_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
}

void loop() {
  buttonState = digitalRead(M_BUTTON_PIN);
  
  if (buttonState == HIGH && buttonState != lastButtonState) {
    if (mode != M_BLUE)
      mode++;
    else
      mode = M_HWIDTH;

    enc.write(encval[mode] * 4);
  }

  lastButtonState = buttonState;
  
  // Read Encoder value
  curEnc = enc.read() / 4;
  
  if (curEnc > maxval[mode]) {
    curEnc = maxval[mode];
    enc.write(curEnc * 4);    
  } else if (curEnc < minval[mode]) {
    curEnc = minval[mode];
    enc.write(curEnc * 4);    
  }

  encval[mode] = curEnc;

  if (lastCurEnc != curEnc) {
    FastLED.clear();

    // If width isn't 0, render new
    if (encval[M_HWIDTH])
      render(encval[M_POS] - encval[M_HWIDTH] + 1, encval[M_POS] + encval[M_HWIDTH] - 1, encval[M_RED] << 3, encval[M_GREEN] << 3, encval[M_BLUE] << 3);

    FastLED.show();
  }

  lastCurEnc = curEnc;
}

void render(int start, int end, int r, int g, int b) {
  for (int i = start; i <= end; i++) {
    if (i >= 0 && i < NUM_LEDS) {
      leds[i].red = r;
      leds[i].green = g;
      leds[i].blue = b;
    }
  }
}
    
  
