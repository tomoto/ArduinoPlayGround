#include "util/LedUtil.h"
#include "application.h"

void LedUtil::sendToCloudBegin() {
    // Flash the LED before sending to the cloud
    pinMode(D7, OUTPUT);
    digitalWrite(D7, HIGH);
    delay(100);
}

bool LedUtil::sendToCloudEnd(bool result) {
    // 
    pinMode(D7, OUTPUT);
    digitalWrite(D7, result ? LOW : HIGH);
    return result;
}

void LedUtil::sendToMeshBegin() {
    // Flash the LED before sending to the cloud
    pinMode(D7, OUTPUT);
    digitalWrite(D7, HIGH);
    delay(50);
}

bool LedUtil::sendToMeshEnd(bool result) {
    pinMode(D7, OUTPUT);
    
    /*
    if (!result) {
      for (int i = 0; i < 3; i++) {
        digitalWrite(D7, LOW);
        delay(50);
        digitalWrite(D7, HIGH);
        delay(50);
      }
    }
    */
    
    digitalWrite(D7, LOW);
    return result;
}
