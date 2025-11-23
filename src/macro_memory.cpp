#include "macro_memory.hpp"
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "macro_handling.hpp"

// Define the actual current_macros array here
MacroKey current_macros[16];

void loadDefaultMacros() {
  for (int i = 0; i < 16; i++) {
    current_macros[i] = default_macros[i];
  }
}

bool loadConfigFromFlash() {
  if (!LittleFS.exists("/macros.json")) {
    Serial.println("Config file not found.");
    return false;
  }

  File f = LittleFS.open("/macros.json", "r");
  if (!f) {
    Serial.println("Failed to open config file.");
    return false;
  }
  
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, f);
  f.close();

  if (err) {
    Serial.print("JSON parse error: ");
    Serial.println(err.c_str());
    return false;
  }

  JsonArray macros = doc["macros"];
  if (macros.isNull() || macros.size() < 16) {
    Serial.println("Config macros missing or wrong size.");
    return false;
  }

  for (int i = 0; i < 16; i++) {
    JsonObject m = macros[i];
    current_macros[i].key_code      = m["key_code"]     | default_macros[i].key_code;
    current_macros[i].modifier      = m["modifier"]     | default_macros[i].modifier;
    current_macros[i].consumer_code = m["consumer_code"]| default_macros[i].consumer_code;
    current_macros[i].description   = default_macros[i].description; // keep flash string
    uint32_t color                  = m["color"]        | default_macros[i].color;
    current_macros[i].color         = color;
  }

  Serial.println("Config loaded from flash.");
  return true;
}

bool saveConfigToFlash() {
  StaticJsonDocument<4096> doc;

  JsonArray macros = doc.createNestedArray("macros");
  for (int i = 0; i < 16; i++) {
    JsonObject m = macros.createNestedObject();
    m["key_code"]      = current_macros[i].key_code;
    m["modifier"]      = current_macros[i].modifier;
    m["consumer_code"] = current_macros[i].consumer_code;
    m["color"]         = current_macros[i].color;
    // description left out or added later as a separate map
  }

  File f = LittleFS.open("/macros.json", "w");
  if (!f) {
    Serial.println("Failed to open config file for write.");
    return false;
  }

  if (serializeJson(doc, f) == 0) {
    Serial.println("Failed to write JSON.");
    f.close();
    return false;
  }

  f.close();
  Serial.println("Config saved to flash.");
  return true;
}