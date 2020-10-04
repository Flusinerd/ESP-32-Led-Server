#include <Arduino.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include <EEPROM.h>
#include <WiFi.h>

//Define global Variables
const int led = 5;
uint8_t autoplay = 0;
uint8_t autoplayDuration = 10;
unsigned long autoPlayTimeout = 0;
uint8_t currentPatternIndex = 0;
uint8_t gHue = 0;
uint8_t power = 1;
uint8_t brightness = 8;
uint8_t speed = 30;
uint8_t cooling = 50;
uint8_t sparking = 120;
CRGB solidColor = CRGB::Blue;
uint8_t cyclePalettes = 0;
uint8_t paletteDuration = 10;
uint8_t currentPaletteIndex = 0;
unsigned long paletteTimeout = 0;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#define DATA_PIN    14
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_STRIPS  1
#define NUM_LEDS_PER_STRIP 94
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS
CRGB leds[NUM_LEDS];
#define FRAMES_PER_SECOND  120

#include <WebServer.h>
WebServer webServer(80);

#include "pattern.h"
#include "field.h"
#include "fields.h"

#include "secrets.h"
#include "wifi.h"
#include "web.h"
#define USE_SERIAL Serial

void setup() {
  delay(3000);
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);

  Serial.begin(115200);
  setupWebServer(); 
  setupWifi();
}
void loop(){
  webServerLoop();
}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  currentPatternIndex = (currentPatternIndex + 1) % patternCount;
}

void nextPalette()
{
  currentPaletteIndex = (currentPaletteIndex + 1) % paletteCount;
  targetPalette = palettes[currentPaletteIndex];
}
