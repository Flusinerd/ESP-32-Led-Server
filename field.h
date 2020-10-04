typedef String (*FieldSetter) (String);
typedef String (*FieldGetter) ();

const String NumberFieldType = "Number";
const String BooleanFieldType = "Boolean";
const String SelectFieldType = "Select";
const String ColorFieldType = "Color";
const String SectionFieldType = "Section";

typedef struct {
  public:
    String name;
    String label;
    String type;
    uint8_t min;
    uint8_t max;
    FieldGetter getValue;
    FieldGetter getOptions;
    FieldSetter setValue;
} Field;

typedef Field FieldList[];

Field getField(String name, FieldList fields, uint8_t count){
  for (uint8_t i = 0; i < count; i++) {
    Field field = fields[i];
    if (field.name == name) {
      return field;
    }
  }
  return Field();
}

String getFieldValue(String name, FieldList fields, uint8_t count) {
  Field field = getField(name, fields, count);
  if (field.getValue) {
    return field.getValue();
  }
  return String();
}

CRGB parseColor(String value) {
  uint8_t rIndex = value.indexOf(",");
  uint8_t gIndex = value.indexOf(",", rIndex + 1);

  String rString = value.substring(0, rIndex);
  String gString = value.substring(rIndex + 1, gIndex);
  String bString = value.substring(gIndex + 1);

  uint8_t r = rString.toInt();
  uint8_t g = gString.toInt();
  uint8_t b = bString.toInt();

  return CRGB(r, g, b);
}

void saveFieldsInEEPROM(FieldList fields, uint8_t count) {
  uint8_t index = 0;

  EEPROM.write(index, 0);

  for (uint8_t i = 0; i < count; i++) {
    Field field = fields[i];
    if (!field.getValue && !field.setValue)
      continue;

     String value = field.getValue();

     if (field.type == ColorFieldType) {
      CRGB color = parseColor(value);
      EEPROM.write(index++, color.r);
      EEPROM.write(index++, color.g);
      EEPROM.write(index++, color.b);
     }  else {
      byte v = value.toInt();
      EEPROM.write(index++, v);
     }
  }

  EEPROM.commit();
}

String setFieldValue(String name, String value, FieldList fields, uint8_t count) {
  String result;

  Field field = getField(name, fields, count);
  if (field.setValue) {
    if (field.type == ColorFieldType) {
      String r = webServer.arg("r");
      String g = webServer.arg("g");
      String b = webServer.arg("b");
      String combinedValue = r + "," + g + "," + b;
      result = field.setValue(combinedValue);
    } else {
      result = field.setValue(value);
    }
  }

  saveFieldsInEEPROM(fields, count);

  return result;
}

void loadFieldsFromEEPROM(FieldList fields, uint8_t count) {
  uint8_t byteCount = 1;

  for (uint8_t i = 0; i < count; i++) {
    Field field = fields[i];
    if (!field.setValue)
      continue;

    if (field.type == ColorFieldType) {
      byteCount += 3;
    } else {
      byteCount++;
    }
  }

  if (!EEPROM.begin(count)) {
    Serial.println("Failed to initialize EEPROM!");
    return;
  }

  if (EEPROM.read(0) == 255) {
    Serial.println("First run, or EEPROM erased, skipping settings load!");
    return;
  }

  uint8_t index = 0;

  for (uint8_t i = 0; i < count; i++) {
    Field field = fields[i];
    if (!field.setValue)
      continue;

    if (field.type == ColorFieldType) {
      String r = String(EEPROM.read(index++));
      String g = String(EEPROM.read(index++));
      String b = String(EEPROM.read(index++));
      field.setValue(r + "," + g + "," + b);
    } else {
      byte v = EEPROM.read(index++);
      field.setValue(String(v));
    }
  }
}

String getFieldsJson(FieldList fields, uint8_t count) {
  String json = "[";

  for (uint8_t i = 0; i < count; i++) {
    Field field = fields[i];

    json += "{\"name\":\"" + field.name + "\",\"label\":\"" + field.label + "\",\"type\":\"" + field.type + "\"";

    if (field.getValue) {
      if (field.type == ColorFieldType || field.type == "String") {
        json += ",\"value\":\"" + field.getValue() + "\"";
      }
      else {
        json += ",\"value\":" + field.getValue();
      }
    }

    if (field.type == NumberFieldType) {
      json += ",\"min\":" + String(field.min);
      json += ",\"max\":" + String(field.max);
    }

    if (field.getOptions) {
      json += ",\"options\":[";
      json += field.getOptions();
      json += "]";
    }

    json += "}";

    if (i < count - 1)
      json += ",";
  }

  json += "]";

  return json;
}
