#include "M5Atom.h"

bool IMU6886Flag = false;

void setup()
{
  M5.begin(true, false, true);
  
  if (M5.IMU.Init() != 0)
    IMU6886Flag = false;
  else
    IMU6886Flag = true;
}

const float SCALE = 0.07;
const float OFFSET = 2.5;
const float BOUNDARY = OFFSET - 0.01;
const float BOUNCE = 0.8;
const float ACC_MIN = 0.1;
const float DECAY = 0.95;
const float TRANSIT = 0.01;

float ax, ay, az;
float dx, dy;
float x, y;
float t;

float clip(float x, float a, float b) {
  return min(max(x, a), b);
}

float alternate(float t) {
  return clip(sin(t) * 5.0, -0.5f, 0.5f) + 0.5f;
}

void loop()
{
  if (M5.Btn.wasPressed()) {
    x = y = dx = dy = 0;
  }

  if (IMU6886Flag == true)
  {
    M5.IMU.getAccelData(&ax, &ay, &az);
    if (fabs(ax) + fabs(ay) > ACC_MIN) dx += ax * SCALE, dy += ay * SCALE;
    x += dx, y += dy;
    dx *= DECAY, dy *= DECAY;

    if (x >=  BOUNDARY) x =  BOUNDARY, dx *= -BOUNCE;
    if (x <= -BOUNDARY) x = -BOUNDARY, dx *= -BOUNCE;
    if (y >=  BOUNDARY) y =  BOUNDARY, dy *= -BOUNCE;
    if (y <= -BOUNDARY) y = -BOUNDARY, dy *= -BOUNCE;

    float p = min(fabs(dx) + fabs(dy), 1.0);
    float q = alternate(t);
    int r = int(0xff * p);
    int b = int(0xff * (1.0f - p) * q);
    int g = int(0x80 * (1.0f - p) * (1.0 - q));
    t += TRANSIT;
    
    M5.dis.clear();
    M5.dis.drawpix(x + OFFSET, y + OFFSET, (g << 16) | (r << 8) | b);
    // Serial.printf("%.2f,%.2f,%.2f,%.2f,%.2f\r\n", p, dx, dy, ax, ay);
    // Serial.printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n", x, y, dx, dy, ax, ay);
  }
  delay(10);
  M5.update();
}
