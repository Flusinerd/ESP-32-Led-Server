String getPower() {
  return String(power);
}

String setPower(String value) {
  power = value.toInt();
  power = power == 0 ? 0 : 1;
  return String(power);
}

String getBrightness() {
  return String(brightness);
}

String setBrightness(String value) {
  brightness = value.toInt();
  FastLED.setBrightness(brightness);
  return String(brightness);
}

String getPattern() {
  return String(currentPatternIndex);
}

void setPattern(uint8_t value){
  if (value >= patternCount){
    value = patternCount - 1;
  }
  currentPatternIndex = value;
}

String setPattern(String value) {
  setPattern(value.toInt());
  return String(currentPatternIndex);
}

String getPatterns() {
  String json = "";

  for (uint8_t i = 0; i < patternCount; i++) {
    json += "\"" + patterns[i].name + "\"";
    if (i < patternCount - 1)
      json += ",";
  }

  return json;
}

String getPalette() {
  return String(currentPaletteIndex);
}

String setPalette(String value) {
  // value.toInt() returns long, while currentPaletteIndex is declared as uint8_t
  long tmp = value.toInt();
  // clamp to [0.. paletteCount-1]
  if (tmp < 1) {
    tmp = 1;
  } else if (tmp > (paletteCount-1)) {
    tmp = paletteCount-1;
  }
  currentPaletteIndex = tmp;
  targetPalette = palettes[currentPaletteIndex];
  return String(currentPaletteIndex);
}

String getPalettes() {
  String json = "";

  for (uint8_t i = 0; i < paletteCount; i++) {
    json += "\"" + paletteNames[i] + "\"";
    if (i < paletteCount - 1)
      json += ",";
  }

  return json;
}

String getSpeed() {
  return String(speed);
}

String setSpeed(String value) {
  speed = value.toInt();
  return String(speed);
}

String getAutoplay() {
  return String(autoplay);
}

String setAutoplay(String value) {
  autoplay = value.toInt();
  autoplay = autoplay == 0 ? 0 : 1;
  autoPlayTimeout = millis() + (autoplayDuration * 1000);
  return String(autoplay);
}

String getAutoplayDuration() {
  return String(autoplayDuration);
}

String setAutoplayDuration(String value) {
  // value.toInt() returns long, while autoplayDuration is declared as uint8_t
  long tmp = value.toInt();
  // clamp to [1..255]
  if (tmp < 1) {
    tmp = 1;
  } else if (tmp > 255) {
    tmp = 255;
  }
  autoplayDuration = tmp;
  autoPlayTimeout = millis() + (tmp * 1000);
  return String(autoplayDuration);
}

String getCyclePalettes() {
  return String(cyclePalettes);
}

String setCyclePalettes(String value) {
  cyclePalettes = value.toInt();
  cyclePalettes = cyclePalettes == 0 ? 0 : 1;
  paletteTimeout = millis() + (paletteDuration * 1000);
  return String(cyclePalettes);
}

String getPaletteDuration() {
  return String(paletteDuration);
}

String setPaletteDuration(String value) {
  // value.toInt() returns long, while paletteDuration is declared as uint8_t
  long tmp = value.toInt();
  // clamp to [1..255]
  if (tmp < 1) {
    tmp = 1;
  } else if (tmp > 255) {
    tmp = 255;
  }
  paletteDuration = tmp;
  paletteTimeout = millis() + (tmp * 1000);
  return String(paletteDuration);
}

String getSolidColor() {
  return String(solidColor.r) + "," + String(solidColor.g) + "," + String(solidColor.b);
}

String setSolidColor(uint8_t r, uint8_t g, uint8_t b)
{
  solidColor = CRGB(r, g, b);

  return "\"" + String(solidColor.r) + "," + String(solidColor.g) + "," + String(solidColor.b) + "\"";
}

String setSolidColor(CRGB color) {
  return setSolidColor(color.r, color.g, color.b);
}

String setSolidColor(String value) {
  CRGB color = parseColor(value);
  
  return setSolidColor(color);
}

String getCooling() {
  return String(cooling);
}

String setCooling(String value) {
  cooling = value.toInt();
  return String(cooling);
}

String getSparking() {
  return String(sparking);
}

String setSparking(String value) {
  sparking = value.toInt();
  return String(sparking);
}

String getTwinkleSpeed() {
  return String(twinkleSpeed);
}

String setTwinkleSpeed(String value) {
  // value.toInt() returns long, while twinkleSpeed is declared as uint8_t
  long tmp = value.toInt();
  // clamp to [0..8]
  if (tmp < 0) {
    tmp = 0;
  } else if (tmp > 8) {
    tmp = 8;
  }
  twinkleSpeed = tmp;
  return String(twinkleSpeed);
}

String getTwinkleDensity() {
  return String(twinkleDensity);
}

String setTwinkleDensity(String value) {
  // value.toInt() returns long, while twinkleDensity is declared as uint8_t
  long tmp = value.toInt();
  // clamp to [0..8]
  if (tmp < 0) {
    tmp = 0;
  } else if (tmp > 8) {
    tmp = 8;
  }
  twinkleDensity = tmp;
  return String(twinkleDensity);
}

FieldList fields = {
  // name                 label                type               min,          max,  getValue,            getOptions,   setValue
  { "power",              "Power",             BooleanFieldType,    0,            1,  getPower,            NULL,         setPower            },
  { "brightness",         "Brightness",        NumberFieldType,     1,          255,  getBrightness,       NULL,         setBrightness       },
  { "speed",              "Speed",             NumberFieldType,     1,          255,  getSpeed,            NULL,         setSpeed            },
  
  { "patternSection",     "Pattern",           SectionFieldType,    0,            0,  NULL,                NULL,         NULL                },
  { "pattern",            "Pattern",           SelectFieldType,     0, patternCount,  getPattern,          getPatterns,  setPattern          },
  { "autoplay",           "Cycle Patterns",    BooleanFieldType,    0,            1,  getAutoplay,         NULL,         setAutoplay         },
  { "autoplayDuration",   "Pattern Duration",  NumberFieldType,     1,          255,  getAutoplayDuration, NULL,         setAutoplayDuration },
  
  { "paletteSection",     "Palette",           SectionFieldType,    0,            0,  NULL,                NULL,         NULL                },
  { "palette",            "Palette",           SelectFieldType,     0, paletteCount,  getPalette,          getPalettes,  setPalette          },
  { "cyclePalettes",      "Cycle Palettes",    BooleanFieldType,    0,            1,  getCyclePalettes,    NULL,         setCyclePalettes    },
  { "paletteDuration",    "Palette Duration",  NumberFieldType,     1,          255,  getPaletteDuration,  NULL,         setPaletteDuration  },
  
  { "solidColorSection",  "Solid Color",       SectionFieldType,    0,            0,  NULL,                NULL,         NULL                },
  { "solidColor",         "Color",             ColorFieldType,      0,          255,  getSolidColor,       NULL,         setSolidColor       },
  
  { "fire",               "Fire & Water",      SectionFieldType,    0,            0,  NULL,                NULL,         NULL                },
  { "cooling",            "Cooling",           NumberFieldType,     0,          255,  getCooling,          NULL, setCooling                  },
  { "sparking",           "Sparking",          NumberFieldType,     0,          255,  getSparking,         NULL, setSparking                 },
  
  { "twinklesSection",    "Twinkles",          SectionFieldType,    0,            0,  NULL,                NULL,         NULL                },
  { "twinkleSpeed",       "Twinkle Speed",     NumberFieldType,     0,            8,  getTwinkleSpeed,     NULL, setTwinkleSpeed             },
  { "twinkleDensity",     "Twinkle Density",   NumberFieldType,     0,            8,  getTwinkleDensity,   NULL, setTwinkleDensity           },
};

uint8_t fieldCount = ARRAY_SIZE(fields);
