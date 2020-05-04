// e.g. curl https://api.particle.io/v1/devices/events
// -d name=signaltower/set -d data=red=1 -d private=true -d access_token=...

enum SignalState {
  SS_OFF, SS_ON, SS_BLINK, SS_BREATH, SS_MAX
};

struct SignalDesc {
  const pin_t pin;
  const char* const name;
  SignalState state;
} signals[] = {
  { D3, "green", SS_OFF },
  { D5, "yellow", SS_OFF },
  { D6, "red", SS_OFF },
  { 0, NULL, SS_OFF }
};

#define FOREACH_IN_SIGNALS(s) for (SignalDesc* s = signals; s->name; s++)

int processSetFunc(const char* data) {
  const int BUFSIZE = 64;
  char buf[BUFSIZE];
  strncpy(buf, data, BUFSIZE);
  buf[BUFSIZE-1] = '\0';
  
  char* delimiter = strchr(buf, '=');
  if (delimiter) {
      *delimiter = '\0';
  }
  const char* name = buf;
  bool applyToAll = strcmp(buf, "all") == 0;
  const char* value = delimiter ? delimiter+1 : "1";
  
  SignalState newState = (atoi(value) < SS_MAX) ? SignalState(atoi(value)) : SS_ON;
  
  bool success = false;
  FOREACH_IN_SIGNALS(s) {
    if (applyToAll || strcmp(s->name, name) == 0) {
      s->state = newState;
      success = true;
    }
  }
  
  return success ? 0 : -1;
}

void processSetEvent(const char* event, const char* data) {
  processSetFunc(data);
}

void setup() {
  FOREACH_IN_SIGNALS(s) {
    pinMode(s->pin, OUTPUT);
  }
  
  Particle.function("set", processSetFunc);
  Particle.subscribe("signaltower/set", processSetEvent, MY_DEVICES);
}

void loop() {
  FOREACH_IN_SIGNALS(s) {
    pin_t p = s->pin;
    switch (s->state) {
    case SS_OFF:
      analogWrite(p, 0);
      break;
    case SS_ON:
      analogWrite(p, 255);
      break;
    case SS_BLINK:
      analogWrite(p, (millis() % 1000L < 500) ? 255 : 0);
      break;
    case SS_BREATH:
      int phase = millis() % 4000L;
      if (phase < 1200) {
        analogWrite(p, 255);
      } else if (phase < 1200+256*8-1) {
        int a = 1200+256*8-1-phase;
        analogWrite(p, a / 8);
      } else {
        analogWrite(p, 0);
      }
      break;
    }
  }
  
  delay(10);
}
