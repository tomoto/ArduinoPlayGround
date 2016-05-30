#include <WiFiHTTPClient.h>
#include <ESP8266HTTPClient.h>
#include <Arduino.h>

using namespace tomoto;

void s_processResponse(HTTPClient& http, std::function<bool(int)> consumer)
{
  WiFiClient* stream = http.getStreamPtr();

  while (http.connected()) {
    while (stream->available()) {
      int c = stream->read();
      if (!consumer(c)) return;
    }
    delay(1);
  }
}

int WiFiHTTPClient::get(const char* uri, std::function<bool(int)> consumer)
{
  HTTPClient http;
  
  Serial.printf("HTTP connecting to %s\n", uri);
  http.begin(uri);
  
  int status = http.GET();
  
  if (status > 0) {
    Serial.printf("Status: %d\n", status);
    if (status == HTTP_CODE_OK) {
      s_processResponse(http, consumer);
    }
  } else {
    Serial.printf("HTTP GET failed\n");
  }
  
  http.end();
  Serial.printf("HTTP connection closed\n");
  
  return status;
}
