#include <WiFiApp.h>
#include <WiFiClient.h>
#include <WiFiStatusUtil.h>
#include <Arduino.h>

extern "C" {
#include <user_interface.h>
}

const int STARTUP_MODE_PIN = 0;
const int STARTUP_TIMEOUT_MILLIS = 1000;

const int COMM_SPEED = 9600;

using namespace tomoto;

WiFiApp::WiFiApp(const char* appName, const WiFiAppDeepSleepConfig& deepSleepConfig, bool isAPEnabled) :
  m_appName(appName), m_isAPEnabled(isAPEnabled), m_deepSleepConfig(deepSleepConfig)
{
}

void WiFiApp::init()
{
  pinMode(STARTUP_MODE_PIN, INPUT);
  
  wifi_set_sleep_type(LIGHT_SLEEP_T);
  WiFi.mode(WIFI_OFF);
  
  Serial.begin(COMM_SPEED);
  Serial.println();
  Serial.print("Application started. ID=");
  Serial.println(m_appName);
}

void WiFiApp::begin(const char* hostName, const char* apSSID, const char* apPassword, const IPAddress& apIP)
{
  m_hostName = hostName;
  
  m_station.begin(m_hostName.c_str());
  
  waitForEnableAccessPoint();
  
  if (m_isAPEnabled) {
    IPAddress apSubnetMask = IPAddress(255, 255, 255, 0);
    m_accessPoint.begin(apSSID, apPassword, apIP, apSubnetMask, m_hostName.c_str());
  }
  
  waitForNetworkOrDeepSleep(
    m_deepSleepConfig.initialTimeoutMillis,
    m_deepSleepConfig.deepSleepMillis,
    m_deepSleepConfig.shutdownFunc);
  
  m_networkConfiguratorView.init(ws(), &m_networkConfigurator);
}

bool WiFiApp::waitForNetwork(unsigned long timeoutMillis)
{
  if (m_isAPEnabled) return true;
  
  unsigned long startTime = millis();
  while (millis() - startTime < timeoutMillis) {
    if (m_station.isConnected()) return true;
    Serial.print("WiFi waiting for network. Status=");
    Serial.print(WiFiStatusUtil::toString(WiFi.status()));
    Serial.print(", time=");
    Serial.println(millis() - startTime);
    delay(500);
  }
  
  return false;
}

void WiFiApp::waitForNetworkOrDeepSleep(unsigned long timeoutMillis, unsigned long deepSleepMillis, std::function<void(void)> shutdownFunc)
{
  if (!waitForNetwork(timeoutMillis)) {
    Serial.println("Going into sleep...");
    if (shutdownFunc != NULL) shutdownFunc();
    ESP.deepSleep(deepSleepMillis * 1000);
    delay(500); // to ensure going into deep sleep
  }
}

void WiFiApp::waitForEnableAccessPoint()
{
  unsigned long startTime = millis();
  while (!m_isAPEnabled && millis() - startTime < STARTUP_TIMEOUT_MILLIS) {
    m_isAPEnabled |= !digitalRead(STARTUP_MODE_PIN);
  }
}

void WiFiApp::loop()
{
  waitForNetworkOrDeepSleep(
    m_deepSleepConfig.inProcessTimeoutMillis,
    m_deepSleepConfig.deepSleepMillis,
    m_deepSleepConfig.shutdownFunc);
  
  if (m_isAPEnabled) {
    m_accessPoint.loop();
  }
  
  m_station.loop();
  
  ws()->handleClient();
}

void WiFiApp::handleRootDefault()
{
  ws()->send(200, "text/html", "hello, world.");
}

void WiFiApp::beginWS()
{
  ws()->on("/", std::bind(&WiFiApp::handleRootDefault, this));
  ws()->begin();
  Serial.println("Web server started.");
}