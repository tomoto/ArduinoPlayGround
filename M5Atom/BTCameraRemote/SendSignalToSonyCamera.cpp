#include "SendSignal.h"
// https://github.com/shirriff/Arduino-IRremote.git
#include <IRremote.h>
#include "Display.h"

static IRsend irsend;

void sendSignal() {
  pinMode(IR_SEND_PIN, OUTPUT);
  
  const unsigned long SHOOT_COMMAND = 0xb4b8f;
  Serial.println("Send shoot begin");
  displaySendSignal(true);
  for (int i = 0; i < 3; i++) {
    irsend.sendSony(SHOOT_COMMAND, 20);
    delay(40);
  }
  displaySendSignal(false);
  Serial.println("Send shoot end");
}
