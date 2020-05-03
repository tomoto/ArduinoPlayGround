#include "Storage.h"
#include <FS.h>
#include <SPIFFS.h>

#define TOKEN_FILE "/token.dat"
#define ACCOUNT_ID_FILE "/accountId.dat"

void initStorage() {
  if (SPIFFS.begin(true)) {
    Serial.println("SPIFFS mounted");
  } else {
    Serial.println("SPIFFS mount failed");
  }
}

static String readFile(fs::FS& fs, const char* path) {
  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.printf("Failed to open file '%s' to read\n", path);
    return "";
  }
  String result;
  while (file.available()) {
    result += char(file.read());
  }
  file.close();
  return result;
}

String loadToken() {
  return readFile(SPIFFS, TOKEN_FILE);
}

String loadAccountId() {
  return readFile(SPIFFS, ACCOUNT_ID_FILE);
}

static void writeFile(fs::FS& fs, const char* path, const char* data) {
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.printf("Failed to open file '%s' to write\n", path);
    return;
  }
  if (!file.print(data)) {
    Serial.printf("Failed to write to file '%s\n", path);
    file.close();
    fs.remove(path);
    return;
  }
  file.close();
}

void storeToken(const char* token) {
  writeFile(SPIFFS, TOKEN_FILE, token);
}

void storeAccountId(const char* accountId) {
  writeFile(SPIFFS, ACCOUNT_ID_FILE, accountId);
}
