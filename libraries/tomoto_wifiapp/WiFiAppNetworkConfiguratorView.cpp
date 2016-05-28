#include <WiFiAppNetworkConfiguratorView.h>
#include <WiFiAppNetworkConfigurator.h>
#include <WiFiAppStation.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <IPAddressUtil.h>
#include <WiFiStatusUtil.h>
#include <WiFiStationConfiguration.h>

#define URL_BASE "/config/network"
#define URL_STATUS URL_BASE
#define URL_CHANGE URL_BASE "/change"
#define URL_SCAN_AND_CHANGE URL_BASE "/scanAndChange"
#define URL_RECONNECT URL_BASE "/reconnect"
#define URL_DISCONNECT URL_BASE "/disconnect"
#define HANDLE_FUNC(F) std::bind(&WiFiAppNetworkConfiguratorView::F, this)

using namespace tomoto;

void WiFiAppNetworkConfiguratorView::init(ESP8266WebServer* webServer, WiFiAppNetworkConfigurator* configurator)
{
  m_webServer = webServer;
  m_configurator = configurator;
  
  ws()->on(URL_STATUS, HTTP_GET, HANDLE_FUNC(handleStatus));
  ws()->on(URL_CHANGE, HTTP_GET,  HANDLE_FUNC(handleChangeGet));
  ws()->on(URL_CHANGE, HTTP_POST, HANDLE_FUNC(handleChangePost));
  ws()->on(URL_SCAN_AND_CHANGE, HTTP_GET, HANDLE_FUNC(handleScanAndChange));
  ws()->on(URL_RECONNECT, HTTP_GET, HANDLE_FUNC(handleReconnect));
  ws()->on(URL_DISCONNECT, HTTP_GET, HANDLE_FUNC(handleDisconnect));
}

String WiFiAppNetworkConfiguratorView::renderWiFiOptions()
{
  String result;
  
  const TinyVector<WiFiAccessPointInfo> &accessPoints = m_configurator->getScannedAccessPoints();
  
  for (const WiFiAccessPointInfo* p = accessPoints.begin(); p != accessPoints.end(); p++) {
    result = result + "<option value='" + p->ssid + "'>" + p->ssid + "</option>";
  }
  
  return result;
}

String WiFiAppNetworkConfiguratorView::renderChangeView()
{
  WiFiStationConfiguration config(true);
  
  String wifiOptions = renderWiFiOptions();
  
  return String() +
    "<html>" +
     "<head><title>Network Configuration - Chnage SSID</title></head>" +
     "<body>" +
      "<form method='POST'>" +
       "SSID: <select name='ssid'>" + wifiOptions + "</select> (Current: " + config.ssid() + ")<br>" +
       "Password: <input type='password' name='password'><br>" +
       "<input type='submit'>" +
      "</form>" +
     "</body>" +
    "</html>";
}

String WiFiAppNetworkConfiguratorView::renderStatusView()
{
  WiFiStationConfiguration config(true);
  
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
        (m_configurator->isConnecting() ?
         " <a href='" URL_DISCONNECT "'>Disconnect</a>" :
         " <a href='" URL_RECONNECT "'>Reconnect</a>") +
       "<li>SSID: " + config.ssid() +
        " <a href='" + URL_SCAN_AND_CHANGE + "'>Change</a>" +
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

String WiFiAppNetworkConfiguratorView::renderScanAndChangeView()
{
  return String() +
    "<html>" +
      "<head>" +
        "<meta http-equiv='refresh' content='3;url=" + URL_CHANGE + "'/>" +
      "</head>" +
      "<body>" +
        "Scanning for the networks..." +
      "</body>" +
    "</html>";
}

String WiFiAppNetworkConfiguratorView::renderWaitForStatusView()
{
  return String() +
    "<html>" +
      "<head>" +
        "<meta http-equiv='refresh' content='5;url=" + URL_STATUS + "'/>" +
      "</head>" +
      "<body>" +
        "Waiting for refreshing the network..." +
      "</body>" +
    "</html>";
}

void WiFiAppNetworkConfiguratorView::handleStatus()
{
  if (WiFi.status() != WL_CONNECTED && m_configurator->isConnecting()) {
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
  m_configurator->validateSwitchWiFiPayload(payload, ex);
  
  if (!ex) {
    ws()->send(200, "text/html", renderWaitForStatusView());
    delay(100);
    m_configurator->switchWiFi(payload, ex);
  } else {
    ws()->send(400, "text/plain", ex.message());
  }
}

void WiFiAppNetworkConfiguratorView::handleScanAndChange()
{
  ws()->send(200, "text/html", renderScanAndChangeView());
  delay(100);
  m_configurator->scanAccessPoints();
}

void WiFiAppNetworkConfiguratorView::handleReconnect()
{
  ws()->send(200, "text/html", renderWaitForStatusView());
  delay(100);
  m_configurator->reconnect();
}

void WiFiAppNetworkConfiguratorView::handleDisconnect()
{
  ws()->send(200, "text/html", renderWaitForStatusView());
  delay(100);
  m_configurator->disconnect();
}
