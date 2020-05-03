#include <Arduino.h>

#include "FastLEDConfig.h"
#include <M5Atom.h>
#include <WiFi.h>
#include <esp_pm.h>

#include "HTTPHandlers.h"
#include "Display.h"
#include "Storage.h"

#include "Secrets.h"

//
// Global states
//

const unsigned long SLEEP_TIMEOUT = 15000;
const unsigned long BEACON_TIMEOUT = 5000;
const unsigned long WIFI_CONNECTION_TIMEOUT = 15000;

String loginTk;
String loginAccountId;
bool switchState;

static bool sleeping;
static unsigned long nextSleepTime;
static uint8_t lastWiFiState;
static unsigned long initWifiStartedTime;

//
// Setup
//

void initPowerManagement() {
  // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/power_management.html
  // https://github.com/espressif/esp-idf/blob/master/examples/wifi/power_save/main/power_save.c
  esp_pm_config_esp32_t powerConfig;
  powerConfig.max_freq_mhz = 80;
  powerConfig.min_freq_mhz = 20;
  powerConfig.light_sleep_enable = true;
  if (esp_pm_configure(&powerConfig) != ESP_OK) {
    Serial.println("Power management failed");
  }
}

void shutdownWiFi() {
  // https://github.com/espressif/arduino-esp32/issues/2501
  WiFi.persistent(false);
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
}

void initWiFi() {
  WIFI_CONFIG;
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  initWifiStartedTime = millis();
}

void setup() {
  Serial.begin(115200);
  Serial.println("");

  initPowerManagement();

  M5.begin(true, false, true);
  delay(10);
  displayInit();
  
  initStorage();
  loginTk = loadToken();
  loginAccountId = loadAccountId();
  Serial.printf("Token/AccountId from storage: %s/%s\n", loginTk.c_str(), loginAccountId.c_str());

  initWiFi();
  Serial.printf("WiFi Status %d\n", WiFi.status());
  
  // https://github.com/FastLED/FastLED/issues/367
  // WiFi.setSleep(false);
}

void restart() {
  Serial.println("Restarting");
  delay(100);
  shutdownWiFi();
  ESP.restart();
}

void deepSleep() {
  Serial.println("Deep sleep");
  displayOff();
  shutdownWiFi();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_39, 0);
  esp_deep_sleep_start();
}

//
// Main loop
//

#define DELAY_AND_RETURN { delay(100); return; }

void loop() {
  M5.update();

  // While sleeping, wake up at button press or do nothing otherwise
  if (sleeping) {
    if (M5.Btn.wasPressed()) {
      Serial.println("Waking up");
      sleeping = false;
      displayInit(); // turn on display
      lastWiFiState = 0; // enforce state refresh at next loop
    }
    DELAY_AND_RETURN;
  }
  
  // Ensure WiFi is connected
  uint8_t  ws = WiFi.status();
  if (ws != WL_CONNECTED) {
    lastWiFiState = ws;
    if (ws == WL_CONNECT_FAILED || (millis() - initWifiStartedTime) > WIFI_CONNECTION_TIMEOUT) {
      Serial.println("WiFi connection failed");
      shutdownWiFi();
      initWiFi();
      DELAY_AND_RETURN;
    }
    
    Serial.printf("Waiting for WiFi: %d\n", ws);
    DELAY_AND_RETURN;
  }

  // Refresh state when WiFi gets connected or waking up
  if (lastWiFiState != ws) {
    lastWiFiState = ws;
    if (getSwitch()) {
      displaySwitch(switchState);
    }
    nextSleepTime = millis() + SLEEP_TIMEOUT;
  }

  if (M5.Btn.wasPressed()) {
    if (setSwitch(!switchState)) {
      switchState = !switchState;
      displaySwitch(switchState);
    }
    nextSleepTime = millis() + SLEEP_TIMEOUT;
  }

  if (millis() > nextSleepTime) {
    Serial.println("Sleeping");
    displayOff();
    sleeping = true;
//    deepSleep();
  }
  
  DELAY_AND_RETURN;
}
