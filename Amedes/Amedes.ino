#include <WiFiApp.h>
#include <WiFiHTTPClient.h>
#include <HTTPUtil.h>
#include <TinyVector.h>
#include <ArduinoJson.h>

using namespace tomoto;

#define USE_EXTERNAL_WAKE_UP

#ifdef USE_EXTERNAL_WAKE_UP
#define DEEP_SLEEP_MILLIS 0
#define SELF_SHUTDOWN_MILLIS (5 * 60 * 1000L)
#else
#define DEEP_SLEEP_MILLIS (30 * 60 * 1000L)
#endif

#define SHORT_FORECAST_URI "http://www.jma.go.jp/jp/yoho/%s.html"
#define WEEKLY_FORECAST_URI "http://www.jma.go.jp/jp/week/%s.html"

#define REGION_PARAM "region"
#define TEST_MODE_PARAM "testMode"

const int SHOW_FORECAST_SWITCH_PIN = 0;
const int SHOW_FORECAST_SWITCH_ON = LOW;
const int SHOW_FORECAST_SENSOR_PIN = 14;
const int SHOW_FORECAST_SENSOR_ON = HIGH;
const int POWER_MAINTENANCE_PIN = 4;

const long FORECAST_EXPIRATION_MILLIS = 15 * 60 * 1000L;
const unsigned long FORECAST_EXPIRED = -FORECAST_EXPIRATION_MILLIS;

String getConfigInJSON();

void shutdown()
{
  clearDisplay();
}

WiFiApp app("Amedes", WiFiAppDeepSleepConfig(40*1000L, 120*1000L, DEEP_SLEEP_MILLIS, shutdown));

TinyVector<int> shortForecast;
TinyVector<int> weeklyForecast;
unsigned long lastRefreshMillis = FORECAST_EXPIRED;
bool displayForecastTriggered = false;
bool testMode = false;

String renderRootView()
{
  const char* region = getConfigRegion();
  
  return String() +
    "<html>" +
     "<head><title>Amedes</title></head>" +
     "<body>" +
      "<h1>Amedes</h1>" +
      "<ul>"
       "<li>Region: " + region +
        " (Original forecast page: <a href='" + buildURI(SHORT_FORECAST_URI, getConfigRegion()) + "'>Short</a>" +
                               " / <a href='" + buildURI(WEEKLY_FORECAST_URI, getConfigRegion()) + "'>Long</a>)" +
       "<li>Test Mode: " + (testMode ? "On" : "Off") + " (<a href='/testMode/on'>On</a> / <a href='/testMode/off'>Off</a>)" +
       "<li><form action='/refresh' style='margin: 0'>Refresh: <input type='text' name='region' style='width: 60px' value='" + getConfigRegion() + "'><input type='submit'></form>" +
      "</ul>" +
     "</body>" +
    "</html>";
}

void handleRoot()
{
  app.ws()->send(HTTP_CODE_OK, CT_HTML, renderRootView());
}

String renderTestModeResponse()
{
  StaticJsonBuffer<100> jb;
  JsonObject& jo = jb.createObject();

  jo[TEST_MODE_PARAM] = testMode;
  
  String json;
  jo.printTo(json);
  return json;
}

void handleTestMode()
{
  app.ws()->send(HTTP_CODE_OK, CT_JSON, renderTestModeResponse());
}

void handleTestModeOn()
{
  testMode = true;
  handleTestMode();
}

void handleTestModeOff()
{
  testMode = false;
  displayForecastTriggered = false;
  handleTestMode();
}

String buildURI(const char* format, const char* region)
{
  char buf[256];
  sprintf(buf, format, region);
  return String(buf);
}

bool refreshForecast(const char* newRegion = NULL)
{
  const char* region = newRegion ? newRegion : getConfigRegion();
  
  if (getShortForecast(buildURI(SHORT_FORECAST_URI, region).c_str(), shortForecast) != HTTP_CODE_OK ||
      getWeeklyForecast(buildURI(WEEKLY_FORECAST_URI, region).c_str(), weeklyForecast) != HTTP_CODE_OK) {
    showRefreshError();
    return false;
  }

  setConfigRegion(region);
  setConfigForecast(shortForecast, weeklyForecast);

  lastRefreshMillis = millis();

  return true;
}

bool refreshForecastIfExpired()
{
  if (millis() - lastRefreshMillis > FORECAST_EXPIRATION_MILLIS) {
    Serial.println("Forecast Expired");
    return refreshForecast();
  } else {
    Serial.printf("Using previous forecast, %ld seconds left\n", (lastRefreshMillis + FORECAST_EXPIRATION_MILLIS - millis()) / 1000);
    return true;
  }
  
}

inline void triggerDisplayForecast()
{
  displayForecastTriggered = true;
}

void handleRefresh()
{
  bool succeeded = app.ws()->hasArg(REGION_PARAM) ?
    refreshForecast(app.ws()->arg(REGION_PARAM).c_str()) :
    refreshForecast();

  if (succeeded) {
    app.ws()->send(HTTP_CODE_OK, CT_JSON, getConfigInJSON());
    delay(100);
    triggerDisplayForecast();
  } else {
    app.ws()->send(HTTP_CODE_INTERNAL_SERVER_ERROR , CT_TEXT, "Failed to refresh the forecast data");
  }
}

void displayNetworkConnecting(unsigned long elapsedTime)
{
  static bool show = false;
  show = !show;
  showNetworkConnectingSignal(show);
}

void setup() {
  pinMode(POWER_MAINTENANCE_PIN, OUTPUT);
  digitalWrite(POWER_MAINTENANCE_PIN, HIGH);
  
  app.init();
  initDisplay();

  app.setWaitForNetworkCallback(displayNetworkConnecting);
  app.begin("amedes", "AmedesWiFi", "ameame2020", IPAddress(192, 168, 20, 1));

  app.ws()->on("/", handleRoot);
  app.ws()->on("/refresh", handleRefresh);
  app.ws()->on("/testMode", handleTestMode);
  app.ws()->on("/testMode/on", handleTestModeOn);
  app.ws()->on("/testMode/off", handleTestModeOff);
  app.beginWS();

  refreshForecast();
  triggerDisplayForecast();
}

bool showForecastTriggered()
{
  return
    digitalRead(SHOW_FORECAST_SWITCH_PIN) == SHOW_FORECAST_SWITCH_ON ||
    digitalRead(SHOW_FORECAST_SENSOR_PIN) == SHOW_FORECAST_SENSOR_ON;
}

void normalModeLoop()
{
  if (showForecastTriggered()) {
    Serial.println("Show forecast triggerred");
    refreshForecastIfExpired();
    triggerDisplayForecast();
  }

  if (displayForecastTriggered) {
    displayForecastTriggered = false;
    displayForecast(shortForecast, weeklyForecast, [](){ app.loop(); });
  }

#ifdef USE_EXTERNAL_WAKE_UP
  if (millis() - lastRefreshMillis > SELF_SHUTDOWN_MILLIS) {
    app.goToDeepSleep();
  }
#endif
}

void testModeLoop()
{
  showTestSignal(showForecastTriggered());
}

void loop() {
  app.loop();

  if (!testMode) {
    normalModeLoop();
  } else {
    testModeLoop();
  }
  
  delay(100);
}

