#include <WiFiAppNetworkConfigurator.h>
#include <ESP8266WiFi.h>
#include <HardwareSerial.h>

using namespace tomoto;

void WiFiAppNetworkConfigurator::switchWiFi(const SwitchWiFiPayload& payload, ErrorInfo& ex)
{
  validateSwitchWiFiPayload(payload, ex);
  if (ex) return;
  
  const String& ssid = payload.ssid;
  const String& password = payload.password;
  
  WiFi.disconnect();
  WiFi.begin(ssid.c_str(), password.c_str());
  
  Serial.print("WiFi Switching. SSID=");
  Serial.println(ssid);
}

void WiFiAppNetworkConfigurator::validateSwitchWiFiPayload(const SwitchWiFiPayload& payload, ErrorInfo& ex)
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

void WiFiAppNetworkConfigurator::scanAccessPoints(TinyVector<WiFiAccessPointInfo>* outAccessPoints)
{
  // save current SSID and password
  // because scanning networks may clear them out
  String savedSSID = WiFi.SSID();
  String savedPSK = WiFi.psk();
  
  int8_t numberOfAccessPoints = WiFi.scanNetworks();
  outAccessPoints->resize(numberOfAccessPoints);
  for (int i = 0; i < numberOfAccessPoints; i++) {
    WiFiAccessPointInfo* p = outAccessPoints->begin() + i;
    WiFi.getNetworkInfo(i, p->ssid, p->encryptionType, p->rssi, p->bssid, p->ch, p->isHidden);
  }
  
  // restore current SSID and password
  if (savedSSID.length() > 0) {
    WiFi.begin(savedSSID.c_str(), savedPSK.c_str());
  }
}
