#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_NeoPixel.h>

extern Adafruit_ST7789 tft;

void initDisplay();
void displayTime();
void printTemp(String temp, int x, int y, uint16_t color = ST77XX_WHITE, int size = 5);
