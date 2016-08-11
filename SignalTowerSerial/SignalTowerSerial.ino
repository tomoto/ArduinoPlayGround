#include <ArduinoJson.h>

const int NUMBER_OF_SIGNALS = 4;

enum SignalMode
{
  OFF,
  ON,
  BLINK
};

class SignalState
{
private:
  int m_pin;
  String m_name;
  SignalMode m_mode;
  
public:
  SignalState(int pin, const char* name) :
    m_pin(pin), m_name(name), m_mode(SignalMode::OFF) {}
  
  int pin() const { return m_pin; }
  
  const char* name() const { return m_name.c_str(); }
  
  SignalMode mode() const { return m_mode; }
  void mode(SignalMode m) { m_mode = m; }
  
  void init() const {
    pinMode(m_pin, OUTPUT);
  }
  
  int output() const {
    switch (m_mode) {
    case SignalMode::OFF:
      digitalWrite(m_pin, LOW);
      return 0;
    case SignalMode::ON:
      digitalWrite(m_pin, HIGH);
      return 0;
    case SignalMode::BLINK:
      int phase = millis() % 4000L;
      if (phase < 2000) {
        digitalWrite(m_pin, HIGH);
        return 0;
      } else if (phase < 3023) {
      	analogWrite(m_pin, (3023-phase)>>2);
      	return 10;
      } else {
        digitalWrite(m_pin, LOW);
        return 0;
      }
    }
  }
};

SignalState states[4] = {
  SignalState(5, "blue"),
  SignalState(6, "green"),
  SignalState(9, "yellow"),
  SignalState(10, "red")
};

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < NUMBER_OF_SIGNALS; i++) {
    states[i].init();
  }
}

void processJson(const String& json) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json.c_str());
  
  for (int i = 0; i < NUMBER_OF_SIGNALS; i++) {
    SignalState& s = states[i];
    const char* mode = root.containsKey(s.name()) ? root[s.name()] : root["all"];
    if (mode) {
      if (strcmp(mode, "off") == 0) s.mode(SignalMode::OFF);
      if (strcmp(mode, "on") == 0) s.mode(SignalMode::ON);
      if (strcmp(mode, "blink") == 0) s.mode(SignalMode::BLINK);
    }
  }
}

void loop() {
  while (Serial.available()) {
    String json = Serial.readStringUntil('\n');
    processJson(json);
  }
  
  int minResolution = 100;
  for (int i = 0; i < NUMBER_OF_SIGNALS; i++) {
    int resolution = states[i].output();
    if (resolution > 0 && resolution < minResolution) {
      minResolution = resolution;
    }
  }
  
  delay(minResolution);
}
