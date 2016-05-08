#include <ServoUtil.h>

using namespace tomoto;

static ServoProfile s_defaultServoProfile(544, 2400);
const ServoProfile* ServoProfile::DEFAULT_PROFILE = &s_defaultServoProfile;

ServoProfile::ServoProfile(int minMicroseconds, int maxMicroseconds) :
  m_minMicroseconds(minMicroseconds),
  m_rangeMicroseconds(maxMicroseconds - minMicroseconds)
{
}

int ServoProfile::degreesToMicroseconds(float degrees, bool reverse) const
{
  const float MAX_DEGREES = 180.0;
  if (reverse) degrees = MAX_DEGREES - degrees;
  if (degrees < 0) degrees = 0;
  if (degrees > MAX_DEGREES) degrees = MAX_DEGREES;
  
  return m_minMicroseconds + (int)(m_rangeMicroseconds * (degrees / MAX_DEGREES));
};

ServoUtil::ServoUtil(const ServoProfile* profile) :
  m_profile(profile ? profile : &s_defaultServoProfile),
  m_pin(0),
  m_reverse(false),
  m_currentDegrees(0),
  m_moving(false)
{
}

void ServoUtil::setup(int pin, bool reverse)
{
  m_reverse = reverse;
  m_pin = pin;
  attach();
}

void ServoUtil::attach()
{
  m_servo.attach(m_pin);
}

void ServoUtil::detach()
{
  m_servo.detach();
}

void ServoUtil::directMoveTo(float degrees)
{
  m_currentDegrees = degrees;
  int microseconds = m_profile->degreesToMicroseconds(degrees, m_reverse);
  m_servo.writeMicroseconds(microseconds);
}

void ServoUtil::initiateSmoothMoveTo(float targetDegrees, float targetSpeed, float acceleration)
{
  if (m_currentDegrees == targetDegrees) {
    m_moving = false;
  } else {
    m_moving = true;
    m_initialDegrees = m_currentDegrees;
    m_targetDegrees = targetDegrees;
    m_currentSpeed = 0;
    m_targetSpeed = targetSpeed;
    m_acceleration = acceleration;
  }
}

bool ServoUtil::processSmoothMove()
{
  if (!m_moving) {
    return false;
  }
  
  int moveDirection = m_initialDegrees < m_targetDegrees ? 1 : -1;
  int accelerationDirection = (m_currentDegrees < (m_initialDegrees + m_targetDegrees) / 2) ? 1 : -1;
  
  m_currentSpeed += m_acceleration * accelerationDirection;
  
  float clippedSpead = (m_currentSpeed > m_targetSpeed)  ? m_targetSpeed :
                       (m_currentSpeed < -m_targetSpeed) ? -m_targetSpeed :
                       m_currentSpeed;
  
  directMoveTo(m_currentDegrees + clippedSpead);
  
  if (m_currentSpeed * moveDirection <= 0) {
    m_moving = false;
  }
  
  return m_moving;
}

void ServoUtil::smoothMoveTo(float targetDegrees, float targetSpeed, float acceleration)
{
  initiateSmoothMoveTo(targetDegrees, targetSpeed, acceleration);
  while (processSmoothMove()) {
    delay(1);
  }
}
