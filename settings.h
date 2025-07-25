#pragma once
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Preferences.h>
#include <time.h>
#include <cstring>
#include <Arduino.h>
#include "esp_timer.h"
#include "env.h"

#define LED_PIN     8
#define NUM_LEDS    1
#define TFT_CS      14
#define TFT_RST     21
#define TFT_DC      15
#define TFT_BL      22
#define TFT_MOSI    6
#define TFT_SCLK    7
#define BOOT_BUTTON_PIN 9  
#define WEATHER_INTERVAL 15


extern int brightness_level;
extern int levels[];

extern time_t now;
extern struct tm timeinfo;
extern Preferences preferences;

extern const char* tz;

void initWiFi();
void initTime();