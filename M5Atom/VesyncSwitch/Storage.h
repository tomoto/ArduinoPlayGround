#include <Arduino.h>

void initStorage();
String loadToken();
String loadAccountId();
void storeToken(const char* token);
void storeAccountId(const char* accountId);
