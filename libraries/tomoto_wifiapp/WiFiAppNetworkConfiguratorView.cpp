#include <WiFiAppNetworkConfiguratorView.h>
#include <WiFiAppNetworkConfigurator.h>
#include <WiFiAppStation.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <IPAddressUtil.h>
#include <WiFiStatusUtil.h>

#define URL_BASE "/config/network"
#define URL_STATUS URL_BASE
#define URL_CHANGE URL_BASE "/change"
#define HANDLE_FUNC(F) std::bind(&WiFiAppNetworkConfiguratorView::F, this)

using namespace tomoto;

void WiFiAppNetworkConfiguratorView::init(ESP8266WebServer* webServer, WiFiAppNetworkConfigurator* configurator)
{
  m_webServer = webServer;
  m_configurator = configurator;
  
  ws()->on(URL_STATUS, HTTP_GET, HANDLE_FUNC(handleStatusGet));
  ws()->on(URL_CHANGE, HTTP_GET,  HANDLE_FUNC(handleChangeGet));
  ws()->on(URL_CHANGE, HTTP_POST, HANDLE_FUNC(handleChangePost));
}

String WiFiAppNetworkConfiguratorView::renderWiFiOptions()
{
  String result;
  
  TinyVector<WiFiAccessPointInfo> accessPoints;
  m_configurator->scanAccessPoints(&accessPoints);
  
  for (WiFiAccessPointInfo* p = accessPoints.begin(); p != accessPoints.end(); p++) {
    result = result + "<option value='" + p->ssid + "'>" + p->ssid + "</option>";
  }
  
  return result;
}

String WiFiAppNetworkConfiguratorView::renderChangeView()
{
  String wifiOptions = renderWiFiOptions();
  
  return String() +
    "<html>" +
     "<head><title>Network Configuration - Chnage SSID</title></head>" +
     "<body>" +
      "<form method='POST'>" +
       "SSID: <select name='ssid'>" + wifiOptions + "</select> (Current: " + WiFi.SSID() + ")<br>" +
       "Password: <input type='password' name='password'><br>" +
       "<input type='submit'>" +
      "</form>" +
     "</body>" +
    "</html>";
}

String WiFiAppNetworkConfiguratorView::renderStatusView()
{
  return String() +
    "<html>" +
     "<head><title>Network Configuration - Status</title></head>" +
     "<body>" +
      "General Information" +
      "<ul>" +
       "<li>Host name: " + WiFi.hostname() +
      "</ul>" +
      "WiFi Station Information" +
      "<ul>" +
       "<li>Status: " + WiFiStatusUtil::toString(WiFi.status()) +
       "<li>SSID: " + WiFi.SSID() + " <a href='" + URL_CHANGE + "'>Change</a>" +
       "<li>IP address: " + IPAddressUtil::toString(WiFi.localIP()) +
       "<li>Subnet mask: " + IPAddressUtil::toString(WiFi.subnetMask()) +
       "<li>Default gateway: " + IPAddressUtil::toString(WiFi.gatewayIP()) +
       "<li>DNS: " + IPAddressUtil::toString(WiFi.dnsIP()) +
       "<li>MAC address: " + WiFi.macAddress() +
      "</ul>" +
      "WiFi Access Point Information" +
      "<ul>" +
       "<li>AP IP address: " + IPAddressUtil::toString(WiFi.softAPIP()) +
       "<li>AP MAC address: " + WiFi.softAPmacAddress() +
      "</ul>" +
     "</body>" +
    "</html>";
}

void WiFiAppNetworkConfiguratorView::handleStatusGet()
{
  if (WiFi.status() != WL_CONNECTED) {
    ws()->sendHeader("Refresh", "5");
  }
  
  ws()->send(200, "text/html", renderStatusView().c_str());
}

void WiFiAppNetworkConfiguratorView::handleChangeGet()
{
  ws()->send(200, "text/html", renderChangeView().c_str());
}

void WiFiAppNetworkConfiguratorView::handleChangePost()
{
  WiFiAppNetworkConfigurator::SwitchWiFiPayload payload;
  payload.ssid = ws()->arg("ssid");
  payload.password = ws()->arg("password");
  
  ErrorInfo ex;
  m_configurator->switchWiFi(payload, ex);
  if (!ex) {
    ws()->sendHeader("Location", URL_STATUS);
    ws()->send(302);
  } else {
    ws()->send(400, "text/plain", ex.message());
  }
}
