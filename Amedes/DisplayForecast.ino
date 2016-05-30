#include <TinyVector.h>
#include <Adafruit_NeoPixel.h>

const int OUTPUT_PIN = 12;
Adafruit_NeoPixel strip(8, OUTPUT_PIN);

const int BRIGHTNESS = 255;
const int L = 20; // normal brightness
const int S = 50; // signal brightness

static int maxItem(const int* a, int begin, int end)
{
  int result = a[begin];
  for (int i = begin+1; i < end; i++) {
    if (a[i] > result) result = a[i];
  }
  return result;
}

static uint32_t percipToColor(int p, double b)
{
  const int R = 255;
  const int G = 80;
  const int B = 200;
  double r = p / 100.0;
  return (r >= 0 && r <= 1) ? strip.Color(r*R*b, (1-r)*G*b, (1-r)*B*b) : strip.Color(L*b, L*b, L*b);
}

static void setPixelPercip(int i, int p, double b)
{
  strip.setPixelColor(i, percipToColor(p, b));
}

static double em(double x, double c = 2.0)
{
  return exp(-(x*x)/(c*c));
}

static void show(long delayMillis)
{
  strip.show();
  delay(delayMillis);
}

static void showRainbow()
{
  for (int i = 0; i < 8; i++) strip.setPixelColor(i, em(i)*L, em(i-8.0/3)*L, em(i-8.0/3*2)*L);
  strip.show();
}

static void showNetworkConnectingSignal(bool show)
{
  strip.clear();
  if (show) strip.setPixelColor(0, 0, S, 0);
  strip.show();
}

static void showTestSignal(bool show)
{
  strip.clear();
  if (show) strip.setPixelColor(0, 0, 0, S);
  strip.show();
}

static void blinkSignal(uint32_t c)
{
  strip.clear();
  for (int k = 0; k < 5; k++) {
    strip.setPixelColor(0, c);
    show(200);
    strip.setPixelColor(0, 0);
    show(100);
  }
  strip.clear();
}

void showRefreshError()
{
  blinkSignal(strip.Color(S, 0, 0));
}

void showDataStructureError()
{
  blinkSignal(strip.Color(S, S, 0));
}

void initDisplay()
{
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  showRainbow();
}

void clearDisplay()
{
  strip.clear();
  strip.show();
}

void displayForecast(const TinyVector<int>& shortForecast, const TinyVector<int>& weeklyForecast, std::function<void(void)> yieldFunc)
{
  if (shortForecast.size() != 8 || weeklyForecast.size() != 10) {
    showDataStructureError();
    return;
  }
  
  int percips[16];
  for (int i = 0; i < 8; i++) percips[i] = shortForecast[i];
  percips[8] = maxItem(shortForecast, 0, 4);
  percips[9] = maxItem(shortForecast, 4, 8);
  for (int i = 0; i < 6; i++) percips[i+10] = weeklyForecast[i+4];

  int currentTime = 0;
  while (percips[currentTime] < 0) currentTime++;

  // Starting up
  for (int k = 0; k < 3; k++) {
    for (double j = -4; j <= 12; j += 0.1) {
      for (int i = 0; i < 8; i++) setPixelPercip(i, percips[i], em(i-j));
      show(10);
      yieldFunc();
    }
  }

  // Flash current
  for (int k = 0; k < 5; k++) {
    for (int i = 0; i < 8; i++) setPixelPercip(i, percips[i], 0.2);
    for (double j = currentTime-4; j <= currentTime+4; j += 0.05) {
      setPixelPercip(currentTime, percips[currentTime], 0.2f+em(currentTime-j, 1.0)*0.8);
      show(10);
      yieldFunc();
    }
  }

  // Smooth scroll 
  for (int j = 0; j < 8; j++) {
    for (double m = 0; m < 1.0; m += 0.1) {
      for (int i = 0; i < 8; i++) {
        int p = i + j;
        setPixelPercip(i, (p>=currentTime && p<15) ? (percips[p]*(1.0-m)+percips[p+1]*m) : percips[p], 0.2+em(j+m-4.0)*0.8);
      }
      show(10);
      yieldFunc();
    }
  }

  // Wave
  for (int k = 0; k < 4; k++) {
    for (double j = -4; j <= 12; j += 0.05) {
      for (int i = 0; i < 8; i++) setPixelPercip(i, percips[i+8], 0.2+em(i-j, 1.0)*0.8);
      show(10);
      yieldFunc();
    }
  }

  // Fade out
  for (double j = -2.5; j <= 4.5; j += 0.1) {
    for (int i = 0; i < 8; i++) {
      setPixelPercip(i, percips[i+8], em(j));
    }
    show(10);
    yieldFunc();
  }

  strip.clear();
  strip.show();
}

