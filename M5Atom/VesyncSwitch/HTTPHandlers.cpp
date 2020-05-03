#include "HTTPHandlers.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "Display.h"
#include "Storage.h"

#include "Secrets.h"

#define VESYNC_URL "https://smartapi.vesync.com"
#define LOGIN_URL VESYNC_URL "/vold/user/login"
#define SET_SWITCH_URL VESYNC_URL "/v1/wifi-switch-1.3/%s/status/%s"
#define GET_SWITCH_URL VESYNC_URL "/v1/device/%s/detail"

const char* LOGIN_PAYLOAD = "{ \"account\": \"" VESYNC_USER "\", \"devToken\": \"\", \"password\": \"" VESYNC_HASH "\"}";

StaticJsonDocument<1024> json;

class CleanHttpClient : public HTTPClient {
public:
  CleanHttpClient() {}
  ~CleanHttpClient() { end(); }
};

static void setLoginHeaders(HTTPClient& http) {
  http.addHeader("tk", loginTk);
  http.addHeader("accountid", loginAccountId);
}

static bool login() {
  Serial.println("Login");
  CleanHttpClient http;
  http.begin(LOGIN_URL);
  int httpCode = http.POST(String(LOGIN_PAYLOAD));
  Serial.printf("Status: %d\n", httpCode);
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    DeserializationError error = deserializeJson(json, payload);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return false;
    }
    loginTk = json["tk"].as<const char*>();
    loginAccountId = json["accountID"].as<const char*>();
    storeToken(loginTk.c_str());
    storeAccountId(loginAccountId.c_str());
    Serial.println(loginTk);
    Serial.println(loginAccountId);
    Serial.println("Success");
    return true;
  } else {
    Serial.println("Network Error");
    return false;
  }
}

static bool validateToken() {
  return (loginTk != "" && loginAccountId != "") || login();
}

static void invalidateToken() {
  loginTk = "";
  loginAccountId = "";
}

bool setSwitch(bool state, int retry /* = 0 */) {
  if (!validateToken()) {
    Serial.println("Failed to validate token");
    displayError();
    return false;
  }
  
  Serial.printf("Set Switch %d\n", state);
  CleanHttpClient http;
  char url[256];
  sprintf(url, SET_SWITCH_URL, DEVICE_ID, (state ? "on" : "off"));
  http.begin(url);
  setLoginHeaders(http);
  int httpCode = http.PUT("");
  Serial.printf("Status: %d\n", httpCode);
  if (httpCode == HTTP_CODE_OK) {
    Serial.println("OK");
    return true;
  } else if (httpCode == HTTP_CODE_BAD_REQUEST) {
    Serial.println("Parameter invalid");
    if (retry == 0) {
      invalidateToken();
      return setSwitch(state, retry + 1);
    }
  }
  
  Serial.println("Error");
  displayError();
  return false;
}

bool getSwitch(int retry /* = 0 */) {
  if (!validateToken()) {
    Serial.println("Failed to validate token");
    displayError();
    return false;
  }
  
  Serial.println("Get Switch");
  CleanHttpClient http;
  char url[256];
  sprintf(url, GET_SWITCH_URL, DEVICE_ID);
  http.begin(url);
  setLoginHeaders(http);
  int httpCode = http.GET();
  Serial.printf("Status: %d\n", httpCode);
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    DeserializationError error = deserializeJson(json, payload);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      displayError();
      return false;
    }
    switchState = (strcmp(json["deviceStatus"].as<const char*>(), "on") == 0);
    Serial.println("OK");
    return true;
  } else if (httpCode == HTTP_CODE_BAD_REQUEST) {
    Serial.println("Parameter invalid");
    if (retry == 0) {
      invalidateToken();
      return getSwitch(retry + 1);
    }
  }
  
  Serial.println("Error");
  displayError();
  return false;
}
