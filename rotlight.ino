#include <Encoder.h>
#include <FastLED.h>

#define NUM_LEDS_PER_ROW 4
#define NUM_ROWS 2
#define M_BUTTON_PIN 5
#define D_PIN 6

CRGB leds[NUM_LEDS_PER_ROW * NUM_ROWS];

// Available modes, mode can be change with the Encoder button
enum { M_WIDTH, M_HUE, M_SAT, M_VAL };
int mode = M_VAL;

// Save current encoder value (current setting) for every mode
// Start with white and a 2-wide block
// width, hue, saturation, value
int encval[5] = {2, 0, 0, 0x40};

// Maximum and minimum values for each mode
const int maxval[5] = {NUM_LEDS_PER_ROW, 0xFF, 0xFF, 0xFF};
const int minval[5] = {2, 0, 0, 0};

// Create encoder object
Encoder enc(2, 3);
long curEnc = encval[mode];
long lastCurEnc = encval[mode] - 1;

// Save last button state
int lastButtonState = LOW;
int buttonState = LOW;

// Function prototypes
void render(int width, byte h, byte s, byte v);

// Setup function
void setup() {
  pinMode(M_BUTTON_PIN, INPUT);
  
  FastLED.addLeds<NEOPIXEL, D_PIN>(leds, NUM_LEDS_PER_ROW*NUM_ROWS);
  FastLED.setBrightness(255);

  enc.write(encval[mode] * 4);
}

void loop() {
  buttonState = digitalRead(M_BUTTON_PIN);
  
  if (buttonState == HIGH && buttonState != lastButtonState) {
    if (mode < M_VAL)
      mode++;
    else
      mode = M_WIDTH;

    enc.write(encval[mode] * 4);
  }

  lastButtonState = buttonState;
  
  // Read Encoder value
  curEnc = enc.read() / 4;

  // Limit the value
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

    // Render new state
    render(encval[M_WIDTH], encval[M_HUE], encval[M_SAT], encval[M_VAL]);
    FastLED.show();
  }

  lastCurEnc = curEnc;
}

void render(int width, byte h, byte s, byte v) {
  CHSV color = CHSV(h, s, v);
  
  for (int i = 0; i < NUM_ROWS; i++) {
    int startindex = i * NUM_LEDS_PER_ROW + (NUM_LEDS_PER_ROW - width) / 2;
    fill_solid(&(leds[startindex]), width, color);
  }
}
    
  
