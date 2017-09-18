#include <WiFiAppNetworkConfiguratorView.h>
#include <WiFiAppNetworkConfigurator.h>
#include <WiFiAppStation.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <IPAddressUtil.h>
#include <WiFiStatusUtil.h>
#include <WiFiStationConfiguration.h>
#include <MQTTConfig.h>
#include <HTMLUtil.h>

#define URL_BASE "/config/network"
#define URL_STATUS URL_BASE
#define URL_CHANGE URL_BASE "/change"
#define URL_SCAN_AND_CHANGE URL_BASE "/scanAndChange"
#define URL_RECONNECT URL_BASE "/reconnect"
#define URL_DISCONNECT URL_BASE "/disconnect"
#define URL_MQTT URL_BASE "/mqtt"
#define HANDLE_FUNC(F) std::bind(&WiFiAppNetworkConfiguratorView::F, this)

using namespace tomoto;

void WiFiAppNetworkConfiguratorView::init(ESP8266WebServer* webServer, WiFiAppNetworkConfigurator* configurator, MQTTConfig* mqttConfig)
{
  m_webServer = webServer;
  m_configurator = configurator;
  m_mqttConfig = mqttConfig;
  
  ws()->on(URL_STATUS, HTTP_GET, HANDLE_FUNC(handleStatus));
  ws()->on(URL_CHANGE, HTTP_GET,  HANDLE_FUNC(handleChangeGet));
  ws()->on(URL_CHANGE, HTTP_POST, HANDLE_FUNC(handleChangePost));
  ws()->on(URL_SCAN_AND_CHANGE, HTTP_GET, HANDLE_FUNC(handleScanAndChange));
  ws()->on(URL_RECONNECT, HTTP_GET, HANDLE_FUNC(handleReconnect));
  ws()->on(URL_DISCONNECT, HTTP_GET, HANDLE_FUNC(handleDisconnect));
  
  if (m_mqttConfig) {
    ws()->on(URL_MQTT, HTTP_POST, HANDLE_FUNC(handleMqttPost));
  }
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
  
  String contents = String() +
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
      "</ul>";
  
  if (m_mqttConfig) {
    contents += String() +
      "MQTT Configuration" +
      "<form method='POST' action='" URL_MQTT "'>" +
        m_mqttConfig->ex.message() + 
        "<ul>" +
         "<li>Enabled: <input type='checkbox' value='1' name='enabled' " + HTMLUtil::renderChecked(m_mqttConfig->enabled) + ">" +
         "<li>Host: <input type='text' name='host' value='" + m_mqttConfig->host + "'>" +
         "<li>Port: <input type='text' name='port' value='" + m_mqttConfig->port + "'>" +
         "<li>Topic: <input type='text' name='topic' value='" + m_mqttConfig->topic + "'>" +
         "<li>User: <input type='text' name='user' value='" + m_mqttConfig->user + "'>" +
         "<li>Password: <input type='text' name='password' value='" + m_mqttConfig->password + "'>" +
         "<li><input type='submit' value='Update MQTT Configuration (Reboot required)'>" +
        "</ul>" +
      "</form>" +
      "MQTT Status Information" +
      "<ul>" +
        "<li>TODO</li>" +
      "</ul>";
  }
     
  contents += String() +
      "</body>" +
    "</html>";
  
  return contents;
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

void WiFiAppNetworkConfiguratorView::handleMqttPost()
{
  m_mqttConfig->enabled = ws()->arg("enabled").toInt();
  m_mqttConfig->host = ws()->arg("host");
  m_mqttConfig->port = ws()->arg("port").toInt();
  m_mqttConfig->topic = ws()->arg("topic");
  m_mqttConfig->user = ws()->arg("user");
  m_mqttConfig->password = ws()->arg("password");
  
  if (m_mqttConfig->validate()) {
    m_mqttConfig->save();
  }
  
  ws()->sendHeader("Location", URL_STATUS, true);
  ws()->send(302, "text/plain", "");
}
