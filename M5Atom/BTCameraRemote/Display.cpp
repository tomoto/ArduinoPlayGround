#include "Display.h"

#include <FastLED.h>

const int NUM_LEDS = 25;
const int LED_PIN = 27;
static CRGB leds[NUM_LEDS];

static CRGB correct(CRGB c) { return CRGB(c.g, c.r, c.b); }

void initDisplay() {
  FastLED.addLeds<WS2812, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(4);
  delay(100);
  displayClear();
}

static CRGB countDownColors1[] = {0x800000, 0x000080};
static CRGB countDownColors10[] = {0x808000, 0x008080};

void displayCountDown(int remainingMillis, bool shutterOpen) {
  int type = shutterOpen ? 0 : 1;
  displayClear(false);

  if (remainingMillis > 0) {
    remainingMillis--;

    // 1
    {
      CRGB c = correct(countDownColors1[type]);
      int count = (remainingMillis / 1000) % 10 + 1;
      for (int i = 0; i < count; i++) {
        leds[24 - i] = c;
      }
    }

    // 10
    {
      CRGB c = correct(countDownColors10[type]);
      int count = min(remainingMillis / 10000, 10);
      for (int i = 0; i < count; i++) {
        leds[14 - i] = c;
      }
    }
  }

  FastLED.show();
}

void displaySendSignal(bool value) {
  CRGB c = correct(CRGB(value ? 0x008000 : 0));
  for (int i = 0; i < 5; i++) {
    leds[i] = c;
  }
  FastLED.show();
}

void displayClear(bool show) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = 0;
  }
  if (show) {
    FastLED.show();
  }
}
