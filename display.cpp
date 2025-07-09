#include "display.h"
#include "settings.h"


Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_RST);
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_RGB + NEO_KHZ800);

void initDisplay() {
  strip.begin();
  strip.clear();
  strip.show();

  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  tft.init(172, 320);
  tft.setRotation(3);

  ledcAttach(TFT_BL, 5000, 8);
  ledcWrite(TFT_BL, 128);

  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(4);
  tft.setCursor(30, 30);
  tft.print("Weather App");
  tft.setCursor(60, 70);
  tft.println("Booting!");
  delay(1000);

  strip.setPixelColor(0, strip.Color(0, 0, 255));
  strip.show();
  delay(5000);
  strip.clear();
  strip.show();
  tft.fillScreen(ST77XX_BLACK);
  ledcWrite(TFT_BL, brightness);
}

void displayTime() {
  int minutes = timeinfo.tm_min;
  int hours = timeinfo.tm_hour;
  int displayHours = hours % 12;
  if (displayHours == 0) displayHours = 12;

  char timeString[6];
  snprintf(timeString, sizeof(timeString), "%02d:%02d", displayHours, minutes);

  Serial.println(timeString);
  tft.fillRect(160, 0, 160, 50, ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(5);
  tft.setCursor(160, 10);
  tft.println(timeString);
}

void printTemp(String temp, int x, int y, uint16_t color, int size) {
  tft.setTextColor(color);
  tft.setTextSize(size);
  tft.setCursor(x, y);
  tft.print(temp);
  tft.setTextSize(size > 6 ? 3 : 2);
  tft.print("o");
  tft.setTextSize(5);
  tft.setTextColor(ST77XX_WHITE);
}
