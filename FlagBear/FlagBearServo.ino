#include "FlagBear.h"
#include <ServoUtil.h>

const float WAVE_DEGREES = 15;
const int WAVE_WAIT_MILLIS = 100;
const float OVERSHOOT_DEGREES = 10;
const int OVERSHOOT_WAIT_MILLIS = 100;
const int RIGHT_SERVO_PIN = 12;
const int LEFT_SERVO_PIN = 13;

const float ACCELERATION[] = { 0.0002, 0.0005, 0.001};
const float TARGET_SPEED[] = { 2, 5, 10 };

ServoUtil servo[2];

void initServo()
{
  Serial.println("Initializing servo...");
  
  servo[RIGHT_SERVO_INDEX].setup(RIGHT_SERVO_PIN, true);
  servo[LEFT_SERVO_INDEX].setup(LEFT_SERVO_PIN, false);

  for (int i = 0; i < 2; i++) servo[i].directMoveTo(getConfigDownDegrees(i));
  delay(500);
  for (int i = 0; i < 2; i++) quiet(i);
}

void shutdownServo()
{
  for (int i = 0; i < 2; i++) moveDown(i, DEFAULT_SPEED);
}

void moveToInternal(int index, int speed, float degrees)
{
  servo[index].attach();
  servo[index].smoothMoveTo(degrees, TARGET_SPEED[speed], ACCELERATION[speed]);
}

void quiet(int index)
{
  servo[index].detach();
}

void moveTo(int index, int speed, float degrees) {
  moveToInternal(index, speed, degrees);
  quiet(index);
}

void moveUp(int index, int speed)
{
  float upDegrees = getConfigUpDegrees(index);
  moveToInternal(index, speed, upDegrees + OVERSHOOT_DEGREES);
  delay(OVERSHOOT_WAIT_MILLIS);
  moveToInternal(index, speed, upDegrees);
  quiet(index);
}

void moveDown(int index, int speed)
{
  moveToInternal(index, speed, getConfigDownDegrees(index));
  quiet(index);
}

void waveHand(int index, int speed, int count)
{
  float upDegrees = getConfigUpDegrees(index);
  moveToInternal(index, speed, upDegrees + WAVE_DEGREES);
  delay(WAVE_WAIT_MILLIS);
  for (int i = 0; i < count; i++) {
    moveToInternal(index, speed, upDegrees - WAVE_DEGREES);
    delay(WAVE_WAIT_MILLIS);
    moveToInternal(index, speed, upDegrees + WAVE_DEGREES);
    delay(WAVE_WAIT_MILLIS);
  }
  moveToInternal(index, speed, upDegrees);
  quiet(index);
}

float getHand(int index)
{
  return servo[index].currentDegrees();
}

