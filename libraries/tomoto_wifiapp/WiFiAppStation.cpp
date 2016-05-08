#include <WiFiAppStation.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WINSResponder.h>
#include <WiFiStatusUtil.h>

using namespace tomoto;

static WiFiUDP s_winsUDP;
static WINSResponder s_winsResponder(&s_winsUDP);

static void s_handleWiFiEvent(WiFiEvent_t event)
{
  static WiFiEvent_t ls_lastEvent = WIFI_EVENT_MAX;
  
  if (ls_lastEvent == event) return;
  
  ls_lastEvent = event;
  
  switch (event) {
  case WIFI_EVENT_STAMODE_CONNECTED:
    Serial.print("WiFi Connected. SSID=");
    Serial.println(WiFi.SSID());
    break;
    
  case WIFI_EVENT_STAMODE_GOT_IP:
    Serial.print("WiFi Got IP. localIP=");
    Serial.print(WiFi.localIP());
    Serial.print(", hostName=");
    Serial.println(WiFi.hostname());
    
    s_winsResponder.begin(WiFi.hostname().c_str(), WiFi.localIP());
    MDNS.begin(WiFi.hostname().c_str(), WiFi.localIP());
    Serial.println("WINS and mDNS responder started.");
    break;
    
  case WIFI_EVENT_STAMODE_DHCP_TIMEOUT:
    Serial.println("WiFi DHCP timed out.");
    break;
    
  case WIFI_EVENT_STAMODE_DISCONNECTED:
    Serial.println("WiFi Disconnected.");
    break;
  default:
    // just ignore
    break;
  }
}

bool WiFiAppStation::isConnected()
{
  return WiFi.status() == WL_CONNECTED;
}

void WiFiAppStation::begin(const char* hostName)
{
  WiFi.enableSTA(true);
  WiFi.removeEvent(s_handleWiFiEvent);
  WiFi.onEvent(s_handleWiFiEvent);
  WiFi.hostname(hostName);
  WiFi.begin();
  
  Serial.print("WiFi Connecting. SSID=");
  Serial.println(WiFi.SSID());
}

void WiFiAppStation::loop()
{
  s_winsResponder.processRequest();
}
