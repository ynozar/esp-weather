#pragma once
#include "settings.h"
#include "display.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
void getWeather();
bool printWeatherIcon(String icon, int size, int x_offset, int y_offset);