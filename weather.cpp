#include "weather.h"

void getWeather() {
  String url = "https://www.yoel.app/api/weather/" + String(ZIPCODE);
  Serial.println("Getting weather...");

  HTTPClient http;
  http.begin(url);
  int httpResponseCode = http.GET();

  if (httpResponseCode == 200) {
    String response = http.getString();
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
      Serial.println("JSON Error:");
      Serial.println(error.c_str());
      return;
    }

    String current_temp = String(doc["current_temp"]);
    String max_temp = String(doc["max_temp"]);
    String min_temp = String(doc["min_temp"]);
    String current_icon = String(doc["icons"]["current"]);
    String day_icon = String(doc["icons"]["daytime"]);
    String night_icon = String(doc["icons"]["nighttime"]);

    if (!printWeatherIcon(current_icon, 110, 5, 5)) {
      printWeatherIcon(current_icon, 110, 5, 5);
    }

    tft.fillRect(0, 120, 320, 52, ST77XX_BLACK);
    printTemp(current_temp, 10, 120, ST77XX_WHITE, 7);
    printTemp(max_temp, 160, 120, ST77XX_ORANGE);
    printTemp(min_temp, 245, 120, ST77XX_BLUE);
    printWeatherIcon(day_icon, 50, 160, 60);
    printWeatherIcon(night_icon, 50, 245, 60);
  } else {
    Serial.printf("HTTP error: %d\n", httpResponseCode);
  }
  http.end();
}




bool printWeatherIcon(String icon, int size, int x_offset, int y_offset) {
  String url = "https://weather.yoel.workers.dev/api/weather/icon/" + icon + "?size=" + size;
  Serial.println("getting icon");

  HTTPClient http;
  http.begin(url);
  http.addHeader("Connection", "keep-alive");
  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("HTTP GET failed with code %d\n", httpCode);
    http.end();
    return false;
  }

  WiFiClient* stream = http.getStreamPtr();

  uint8_t header[54];
  if (stream->readBytes(header, 54) != 54) {
    Serial.println("Failed to read BMP header");
    http.end();
    return false;
  }

  if (header[0] != 'B' || header[1] != 'M') {
    Serial.println("Not a BMP file");
    http.end();
    return false;
  }

  uint32_t dataOffset = *(uint32_t*)&header[10];
  uint32_t width = *(uint32_t*)&header[18];
  int32_t height = *(int32_t*)&header[22];
  uint16_t bpp = *(uint16_t*)&header[28];

  Serial.printf("BMP: %dx%d %d bpp, offset %d\n", width, height, bpp, dataOffset);

  if (bpp != 24) {
    Serial.println("Only 24-bit BMP supported");
    http.end();
    return false;
  }

  if (dataOffset > 54) {
    uint32_t skipBytes = dataOffset - 54;
    uint8_t discard[32];
    while (skipBytes > 0) {
      uint8_t chunk = skipBytes > sizeof(discard) ? sizeof(discard) : skipBytes;
      int n = stream->readBytes(discard, chunk);
      if (n <= 0) {
        Serial.println("Failed to skip header bytes");
        http.end();
        return false;
      }
      skipBytes -= n;
    }
  }

  uint32_t rowSize = (width * 3 + 3) & (~3);
  uint8_t* row = (uint8_t*)malloc(rowSize);
  if (!row) {
    Serial.println("Failed to allocate memory");
    http.end();
    return false;
  }

  for (int y = 0; y < abs(height); y++) {
    int bytesRead = stream->readBytes(row, rowSize);
    if (bytesRead != rowSize) {
      Serial.println("Failed to read BMP row");
      Serial.println(bytesRead);
      Serial.println(rowSize);
      Serial.print("stopped at row: ");
      Serial.println(y);
      free(row);
      http.end();
      return false;
    }

    for (uint32_t x = 0; x < width; x++) {
      uint8_t b = row[x * 3 + 0];
      uint8_t g = row[x * 3 + 1];
      uint8_t r = row[x * 3 + 2];
      uint16_t color = tft.color565(r, g, b);

      tft.drawPixel(x + x_offset, y + y_offset, color);
    }
  }

  free(row);
  http.end();
  return true;
}