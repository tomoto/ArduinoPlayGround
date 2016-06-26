#include <WiFiAppStation.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WINSResponder.h>
#include <WiFiStatusUtil.h>

using namespace tomoto;

static WiFiUDP s_winsUDP;
static WINSResponder s_winsResponder(&s_winsUDP);

static void onStationModeConnected(const WiFiEventStationModeConnected& event)
{
  Serial.print("WiFi Connected. SSID=");
  Serial.println(event.ssid);
}

static void onStationModeDisconnected(const WiFiEventStationModeDisconnected& event)
{
  Serial.print("WiFi Disconnected. Reason code=");
  Serial.println(event.reason);
}

static void onStationModeGotIP(const WiFiEventStationModeGotIP& event)
{
  Serial.print("WiFi Got IP. localIP=");
  Serial.print(event.ip);
  Serial.print(", hostName=");
  Serial.println(WiFi.hostname());

  s_winsResponder.begin(WiFi.hostname().c_str(), event.ip);
  MDNS.begin(WiFi.hostname().c_str(), event.ip);
  Serial.println("WINS and mDNS responder started.");
}

static void onStationModeDHCPTimeout()
{
  Serial.println("WiFi DHCP timed out.");
}

static WiFiEventHandler onStationModeConnectedHandler;
static WiFiEventHandler onStationModeDisconnectedHandler;
static WiFiEventHandler onStationModeGotIPHandler;

bool WiFiAppStation::isConnected()
{
  return WiFi.status() == WL_CONNECTED;
}

void WiFiAppStation::begin(const char* hostName)
{
  WiFi.enableSTA(true);
  onStationModeConnectedHandler = WiFi.onStationModeConnected(onStationModeConnected);
  onStationModeDisconnectedHandler = WiFi.onStationModeDisconnected(onStationModeDisconnected);
  onStationModeGotIPHandler = WiFi.onStationModeGotIP(onStationModeGotIP);
  // WiFi.onStationModeDHCPTimeout(onStationModeDHCPTimeout); // to workaround library bug
  
  WiFi.hostname(hostName);
  WiFi.begin();

  Serial.print("WiFi Connecting. SSID=");
  Serial.println(WiFi.SSID());
}

void WiFiAppStation::loop()
{
  s_winsResponder.processRequest();
}
