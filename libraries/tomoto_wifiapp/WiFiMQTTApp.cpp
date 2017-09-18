#include <WiFiMQTTApp.h>

const unsigned long PING_INTERVAL_MILLIS = 60000;
const unsigned long RECONNECT_INTERVAL_MILLIS = 10000;

using namespace tomoto;

WiFiMQTTApp::WiFiMQTTApp(const char* appName, const WiFiAppDeepSleepConfig &deepSleepConfig, bool isAPEnabledByDefault) :
  WiFiApp(appName, deepSleepConfig, isAPEnabledByDefault),
  m_mqttConfig(appName), m_wifiClient(NULL), m_mqttClient(NULL),
  m_lastPingTimeMillis(0), m_pingIntervalMillis(PING_INTERVAL_MILLIS),
  m_lastConnectionFailedMillis(0), m_reconnectIntervalMillis(RECONNECT_INTERVAL_MILLIS)
{
}

WiFiMQTTApp::~WiFiMQTTApp()
{
  if (m_mqttClient) {
    m_mqttClient->disconnect();
    delete m_mqttClient;
  }
  
  if (m_wifiClient) {
    delete m_wifiClient;
  }
}

void WiFiMQTTApp::init()
{
  WiFiApp::init();
  
  m_mqttConfig->ensureLoad();
  
  if (!isAPEnabled() && m_mqttConfig->enabled) {
    m_wifiClient = new WiFiClient();
    m_mqttClient = new Adafruit_MQTT_Client(m_wifiClient,
      m_mqttConfig->host.c_str(), m_mqttConfig->port, m_mqttConfig->user.c_str(), m_mqttConfig->password.c_str());
  }
}

void WiFiMQTTApp::loop()
{
  WiFiApp::loop();
  
  if (m_mqttClient) {
    connectMQTT();
    
    if (m_mqttClient->connected()) {
      if (millis() - m_lastPingTimeMillis > m_pingIntervalMillis) {
        m_mqttClient->ping();
        m_lastPingTimeMillis = millis();
      }
    }
  }
}

Adafruit_MQTT_Subscribe* WiFiMQTTApp::subscribe(const char* topic)
{
  if (m_mqttClient) {
    Adafruit_MQTT_Subscribe* s = new Adafruit_MQTT_Subscribe(m_mqttClient, topic);
    m_mqttClient->subscribe(s);
    return s;
  } else {
    return NULL;
  }
}

Adafruit_MQTT_Subscribe* WiFiMQTTApp::subscribeDefaultTopic()
{
  return subscribe(m_mqttConfig->topic.c_str());
}

void WiFiMQTTApp::beginMQTT()
{
  if (m_mqttClient) {
    connectMQTT();
  }
}

void WiFiMQTTApp::connectMQTT()
{
  if (m_mqttClient->connected()) {
    return;
  }
  
  if (m_lastConnectionFailedMillis && millis() - m_lastConnectionFailedMillis < m_reconnectIntervalMillis) {
    return;
  }
  
  Serial.print("Connecting to MQTT...");
  int errorCode;
  if ((errorCode = m_mqttClient->connect()) != 0) { // connect will return 0 for connected
    Serial.println(m_mqttClient->connectErrorString(errorCode));
    Serial.println("MQTT Connection Failed");
    m_lastConnectionFailedMillis = millis();
  } else {
    Serial.println("MQTT Connected!");
    m_lastConnectionFailedMillis = 0;
    m_lastPingTimeMillis = millis();
  }
}

Adafruit_MQTT_Subscribe* WiFiMQTTApp::readSubscription(int16_t timeout)
{
  return m_mqttClient ? m_mqttClient->readSubscription(timeout) : NULL;
}

MQTTConfig* WiFiMQTTApp::mqttConfig()
{
  return &m_mqttConfig;
}
