#include "settings.h"
#include "display.h"
#include "weather.h"

void IRAM_ATTR AdjustBrightness(); //Boot Button Interrupt
volatile int64_t last_interrupt_time = 0;

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
  int64_t now = esp_timer_get_time();  // Time in microseconds
  if (now - last_interrupt_time > 250000) { // 250ms debounce
    int brightness = levels[brightness_level % 5]; //5 brightness options in the array
    ledcWrite(TFT_BL, brightness);
    Serial.printf("Brightness: %d\n", brightness);
    brightness_level++;
    last_interrupt_time = now;
  }
}
