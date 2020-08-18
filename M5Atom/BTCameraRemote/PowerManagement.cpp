#include "PowerManagement.h"

#include <Arduino.h>
#include <esp_pm.h>
#include <esp_wifi.h>

void slowDownCPU() {
  // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/power_management.html
  // https://github.com/espressif/esp-idf/blob/master/examples/wifi/power_save/main/power_save.c
  esp_pm_config_esp32_t powerConfig;
  powerConfig.max_freq_mhz = 80;
  powerConfig.min_freq_mhz = 40;
  powerConfig.light_sleep_enable = true;
  if (esp_pm_configure(&powerConfig) != ESP_OK) {
    Serial.println("Power management failed");
  }
}

void turnOffWiFi() {
  esp_wifi_stop();
}

void turnOffBT() {
  btStop();
}