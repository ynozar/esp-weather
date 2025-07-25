#include "settings.h"


time_t now;
struct tm timeinfo;
Preferences preferences;

int brightness_level = 3;
int levels[] = {0,1,25,100,200};

// Timezone (Central Time in this example)
const char* tz = "CST6CDT,M3.2.0/2,M11.1.0/2";

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if (attempts > 20) {
      delay(60000);
      esp_deep_sleep_start();
    }
  }
  Serial.println("\nWiFi connected");
}

void initTime() {
  configTzTime(tz, "pool.ntp.org");
  delay(1000);
  time(&now);
  localtime_r(&now, &timeinfo);
}
