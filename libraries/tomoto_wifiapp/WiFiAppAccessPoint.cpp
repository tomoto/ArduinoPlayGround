#include <WiFiAppAccessPoint.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>

using namespace tomoto;

const int DNS_PORT = 53;

static DNSServer s_dnsServer;

void WiFiAppAccessPoint::begin(const char* apSSID, const char* apPassword, const IPAddress& apIP, const IPAddress& apSubnetMask, const char* hostName)
{
  WiFi.softAPConfig(apIP, apIP, apSubnetMask);
  WiFi.softAP(apSSID, apPassword);
  WiFi.enableAP(true);
  
  Serial.print("WiFi AP configured. SSID=");
  Serial.print(apSSID);
  Serial.print(", IP=");
  Serial.println(apIP);
  
  s_dnsServer.start(DNS_PORT, hostName, apIP);
  
  Serial.print("DNS server started. hostName=");
  Serial.println(hostName);
}

void WiFiAppAccessPoint::loop()
{
  s_dnsServer.processNextRequest();
}
