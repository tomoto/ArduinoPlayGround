#include <WiFiStatusUtil.h>

using namespace tomoto;

String WiFiStatusUtil::toString(wl_status_t status)
{
  switch (status) {
  case WL_IDLE_STATUS: return "Idle";
  case WL_NO_SSID_AVAIL: return "No SSID Available";
  case WL_SCAN_COMPLETED: return "Scan Completed";
  case WL_CONNECTED: return "Connected";
  case WL_CONNECT_FAILED: return "Connect Failed";
  case WL_CONNECTION_LOST: return "Connection Lost";
  case WL_DISCONNECTED: return "Disconnected";
  default: return "Unknown";
  }
}
