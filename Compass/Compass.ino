#include <Wire.h>
#include <HMC5883L.h>
#include <MPU6050.h>

MPU6050 mpu;
HMC5883L compass;

const int LED_PIN = 13;

// Fix according to your hardware
const int16_t MAG_X_OFFSET = 155;
const int16_t MAG_Y_OFFSET = -275;
const int16_t MAG_Z_OFFSET = -139;

float mx, my, mz;
float gx, gy, gz;
float angle;

void setupCompass()
{  
  compass.setGain(HMC5883L_GAIN_1370);
  compass.setMode(HMC5883L_MODE_CONTINUOUS);
}

void setupMpu()
{
  mpu.initialize();

  // Fix according to your hardware
  mpu.setXGyroOffset(-15);
  mpu.setYGyroOffset(6);
  mpu.setZGyroOffset(33);
  mpu.setZAccelOffset(850);
}

void readCompass()
{ 
  int16_t mxRaw = compass.getHeadingX() - MAG_X_OFFSET;
  int16_t myRaw = compass.getHeadingY() - MAG_Y_OFFSET;
  int16_t mzRaw = compass.getHeadingZ() - MAG_Z_OFFSET;

  if (abs(mxRaw) < 1000 && abs(myRaw) < 1000 && abs(mzRaw) < 1000) {
    // Tweak the axes to match MPU
    mx = myRaw;
    my = -mxRaw;
    mz = mzRaw;
  }
}

void readMpu()
{
  int16_t gxRaw, gyRaw, gzRaw;
  mpu.getAcceleration(&gxRaw, &gyRaw, &gzRaw);

  gx = gxRaw;
  gy = gyRaw;
  gz = gzRaw;
}

void calcAngle()
{
  // rotate on xy plane
  float gx1 = gx / sqrt(gx*gx+gy*gy);
  float gy1 = gy / sqrt(gx*gx+gy*gy);
  float mx1 = mx * gx1 + my * gy1;
  float my1 = mx * gy1 - my * gx1;
  float mz1 = mz;

  // rotate on xz plane
  float gx2 = sqrt(gx*gx+gy*gy) / sqrt(gx*gx+gy*gy+gz*gz);
  float gz2 = gz / sqrt(gx*gx+gy*gy+gz*gz);
  float mx2 = mx1 * gx2 + mz1 * gz2;
  float my2 = my1;
  float mz2 = mx1 * gz2 - mz1 * gx2;

  angle = atan2(mz2, my2) / M_PI * 180 + 180;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(LED_PIN, OUTPUT);

  setupCompass();
  setupMpu();
}

static unsigned long phaseStartMillis = 0;
static unsigned long lastSerialOutMillis = 0;
static const unsigned long SERIAL_OUT_INTERVAL = 100;

void loop() {
  readCompass();
  readMpu();
  calcAngle();

  float d = angle<=90 ? angle : angle<=180 ? 180-angle : angle<=270 ? angle-180 : 360-angle;

  int t = millis() - phaseStartMillis;
  if (t > d * (1000/90)) {
    phaseStartMillis = millis();
    t = 0;
  }

  digitalWrite(LED_PIN, t<=20);

  if (millis() - lastSerialOutMillis > SERIAL_OUT_INTERVAL) {
    lastSerialOutMillis = millis();
    Serial.print(angle);
    Serial.print(",");
    Serial.print(mx);
    Serial.print(",");
    Serial.print(my);
    Serial.print(",");
    Serial.print(mz);
    Serial.println();
  }
}

