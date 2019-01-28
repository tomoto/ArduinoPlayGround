#include <Adafruit_NeoPixel.h>

const int SOLENOID_PIN = 2;
const int NUM_PIXELS = 24;
const int NEOPIXEL_PIN = 6;
Adafruit_NeoPixel npx(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRBW + NEO_KHZ800);

static float squ(float a) { return a * a; }

class Display {
private:
  uint32_t colors[NUM_PIXELS];
  int ptr;
  float current;
  
public:
  Display() : ptr(0), current(0) {
    for (int i = 0; i < NUM_PIXELS; i++) {
      colors[i] = 0;
    }
  }

  void begin() {
    npx.begin();
    npx.setBrightness(64);
    npx.clear();
  }
  
  void next(float x) {
    current = (x + current) / 2.0;
    float r = 1.0 - current;
    
    colors[ptr] = npx.Color(
      0  + int(r * 200),
      20,
      30 + int(r * 20),
      0 + int(squ(r) * 50 ));
      
    for (int i = 0; i < NUM_PIXELS; i++) {
      npx.setPixelColor((i + ptr) % NUM_PIXELS, colors[i]);
    }
    npx.show();

    ptr = (ptr + 1) % NUM_PIXELS;
  }
};

class Yuragi {
private:
  float x;

public:
  Yuragi() : x(0) {
  }
  
  float next() {
    if (x < 0.5) {
      x = x + 2 * squ(x);
    } else {
      x = x - 2 * squ(1.0 - x);
    }

    if (x < 0.05 || x > 0.995) {
      x = random(10, 90) / 100.0;
    }

    return x;
  }

  float current() {
    return x;
  }
};

class Control {
private:
  Yuragi yuragi;
  Display display;
  float amp;
  int freq;
  int ratio;
  int phase;

public:
  Control() : amp(0.2), freq(12), ratio(3), phase(-1) {
  }

  int getDelay() {
    return int((1000.0 / freq / ratio) * (1.0 + (yuragi.current() - 0.5) * amp * 2));
  }
  
  int getOut() {
    return phase == 0;
  }

  void begin() {
    display.begin();
  }

  void next() {
    if (++phase % ratio == 0) {
      phase = 0;
      yuragi.next();
    }
    display.next(yuragi.current());
  }
} control;

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  
  pinMode(SOLENOID_PIN, OUTPUT);
  control.begin();
}

void loop() {
  control.next();
  digitalWrite(SOLENOID_PIN, control.getOut());
  digitalWrite(13, control.getOut());

  delay(control.getDelay());
}
