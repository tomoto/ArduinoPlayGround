#ifndef ServoUtil_h
#define ServoUtil_h

#include <Servo.h>

namespace tomoto {

class ServoProfile
{
private:
  int m_minMicroseconds;
  int m_rangeMicroseconds;

public:
  ServoProfile(int minMicroseconds, int maxMicroseconds);
  int degreesToMicroseconds(float degrees, bool reverse = false) const;

public:
  static const ServoProfile* DEFAULT_PROFILE;
};

class ServoUtil
{
private:
  const ServoProfile* m_profile;
  Servo m_servo;
  int m_pin;
  bool m_reverse;
  float m_currentDegrees;
  
  bool m_moving;
  float m_initialDegrees;
  float m_targetDegrees;
  float m_currentSpeed;
  float m_targetSpeed;
  float m_acceleration;
  
public:
  ServoUtil(const ServoProfile* profile = NULL);
  void setup(int pin, bool reverse = false);
  void attach();
  void detach();
  void directMoveTo(float degrees);
  void initiateSmoothMoveTo(float targetDegrees, float targetSpeed, float acceleration);
  bool processSmoothMove();
  void smoothMoveTo(float targetDegrees, float targetSpeed, float acceleration);
  float currentDegrees() { return m_currentDegrees; }
};

}

#endif
