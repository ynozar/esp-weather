#include "settings.h"
#include "display.h"
#include "weather.h"

void IRAM_ATTR AdjustBrightness(); //Boot Button Interrupt

void setup() {
  Serial.begin(115200);
  pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BOOT_BUTTON_PIN), AdjustBrightness, FALLING);
  
  initDisplay();
  initWiFi();
  initTime();

  getWeather();
}

void loop() {
  time(&now);
  localtime_r(&now, &timeinfo);

  displayTime();

  if (timeinfo.tm_min % WEATHER_INTERVAL == 0) {
    getWeather();
  }

  int sleepFor = max(60 - timeinfo.tm_sec, 1);
  delay(sleepFor * 1000);
}

void IRAM_ATTR AdjustBrightness() {
  if (brightness == 0) brightness = 1;
  else brightness += 50;
  if (brightness > 255) brightness = 0;
  ledcWrite(TFT_BL, brightness);
  Serial.println("Boot button pressed!");
}
