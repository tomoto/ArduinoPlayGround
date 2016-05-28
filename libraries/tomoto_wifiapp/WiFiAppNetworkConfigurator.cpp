#include <WiFiAppNetworkConfigurator.h>
#include <ESP8266WiFi.h>
#include <HardwareSerial.h>
#include <WiFiStationConfiguration.h>

using namespace tomoto;

void WiFiAppNetworkConfigurator::switchWiFi(const SwitchWiFiPayload& payload, ErrorInfo& ex)
{
  validateSwitchWiFiPayload(payload, ex);
  if (ex) return;
  
  const String& ssid = payload.ssid;
  const String& password = payload.password;
  
  WiFi.disconnect();
  WiFi.persistent(true);
  WiFi.begin(ssid.c_str(), password.c_str());
  WiFi.persistent(false);
  
  Serial.print("WiFi Switching. SSID=");
  Serial.println(ssid);
}

void WiFiAppNetworkConfigurator::validateSwitchWiFiPayload(const SwitchWiFiPayload& payload, ErrorInfo& ex) const
{
  const String& ssid = payload.ssid;
  if (ssid.length() == 0 || ssid.length() > 32) {
    ex.add("SSID is invalid.");
  }
  
  const String& password = payload.password;
  if (password.length() > 0) {
    if (password.length() < 8 || password.length() > 63) {
      ex.add("Password is invalid");
    }
  }
}

void WiFiAppNetworkConfigurator::scanAccessPoints()
{
  int8_t numberOfAccessPoints = WiFi.scanNetworks();
  m_scannedAccessPoints.resize(numberOfAccessPoints);
  for (int i = 0; i < numberOfAccessPoints; i++) {
    WiFiAccessPointInfo* p = m_scannedAccessPoints.begin() + i;
    WiFi.getNetworkInfo(i, p->ssid, p->encryptionType, p->rssi, p->bssid, p->ch, p->isHidden);
  }
}

bool WiFiAppNetworkConfigurator::isConnecting() const
{
  return WiFi.SSID().length() > 0;
}

void WiFiAppNetworkConfigurator::reconnect()
{
  WiFiStationConfiguration config(true);
  WiFi.begin(config.ssid(), config.password());
}

void WiFiAppNetworkConfigurator::disconnect()
{
  WiFi.disconnect();
}

