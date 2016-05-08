#include <WiFiAppConfigBase.h>
#include <FS.h>
#include <TinyVector.h>

using namespace tomoto;

FS& s_filesystem = SPIFFS;

WiFiAppConfigBase::WiFiAppConfigBase(const char* configName) :
  m_file(String("/conf/") + configName + ".conf"), m_isLoaded(false)
{
}

WiFiAppConfigBase::~WiFiAppConfigBase()
{
}

void WiFiAppConfigBase::ensureLoad()
{
  if (m_isLoaded) return;
  
  s_filesystem.begin();
  
  Serial.print("Opening config file: ");
  Serial.println(m_file);
  
  File f = s_filesystem.open(m_file, "r");
  if (f) {
    TinyVector<char> buf(f.size()+1);
    f.readBytes(buf, buf.size());
    f.close();
    buf[buf.size()] = '\0';
    
    DynamicJsonBuffer jsonBuffer;
    JsonObject& jo = jsonBuffer.parseObject(buf);
    if (jo.success()) {
      Serial.println("Succeeded to parse.");
      jo.printTo(Serial);
      Serial.println();
    } else {
      Serial.println("Failed to parse.");
    }
    read(jo);
  } else {
    Serial.println("Failed to open.");
    read(JsonObject::invalid());
  }
  
  m_isLoaded = true;
}

void WiFiAppConfigBase::save()
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& jo = jsonBuffer.createObject();
  write(jo);
  
  Serial.print("Saving config file: ");
  Serial.println(m_file);
  
  File f = s_filesystem.open(m_file, "w");
  if (f) {
    jo.printTo(f);
    f.close();
    Serial.println("Succeeded to save.");
    jo.printTo(Serial);
    Serial.println();
  } else {
    Serial.println("Failed to save.");
  }
}

String WiFiAppConfigBase::getConfigInJSON()
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& jo = jsonBuffer.createObject();
  write(jo);
  
  String result;
  jo.printTo(result);
  return result;
}

