#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define SERVICE_UUID "3a2cea17-54f8-410f-b9c5-3103e6be8ec0"
#define CHARACTERISTIC_UUID "2cd5f185-c190-4264-8d53-827286b38ca3"

#include "SendSignal.h"
#include "Display.h"
#include "CameraController.h"
#include "PowerManagement.h"

const int BUTTON_PIN = 39;
std::string btCommand;

bool isBtAdvertising = false;

CameraController cc(sendSignal, displayCountDown);

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    btCommand = pCharacteristic->getValue();
  }
};

void setup() {
  slowDownCPU();
  // turnOffWiFi();

  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT);

  initDisplay();

  BLEDevice::init("BTCR-5218");
  BLEServer* pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue("0");
  pService->start();

  BLEAdvertising* pAdv = BLEDevice::getAdvertising();
  pAdv->addServiceUUID(SERVICE_UUID);
  pAdv->setScanResponse(true);
  pAdv->setMinPreferred(0x06);
  pAdv->setMinPreferred(0x12);
  pAdv->start();
  isBtAdvertising = true;
}

void loop() {
  if (isBtAdvertising && millis() >= 60 * 1000) {
    BLEDevice::getAdvertising()->stop();
    isBtAdvertising = false;
    Serial.println("BT advertizing stopped");
  }

  if (!digitalRead(BUTTON_PIN)) {
    if (cc.isBulbRunning()) {
      cc.stop();
    } else {
      cc.release();
    }
    delay(200);
  }

  if (!btCommand.empty()) {
    Serial.printf("BT command: %s\n", btCommand.c_str());
    if (btCommand == "r") {
      cc.release();
    } else if (btCommand == "s") {
      cc.stop();
    } else {
      if (cc.isBulbRunning()) {
        cc.stop();
        cc.setBulbSeconds(atoi(btCommand.c_str()));
      } else {
        cc.setBulbSeconds(atoi(btCommand.c_str()));
        cc.release();
      }
    }
    btCommand.clear();
  }

  cc.process();

  delay(10);
}