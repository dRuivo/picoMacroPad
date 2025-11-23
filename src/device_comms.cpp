#include "device_comms.hpp"
#include "ArduinoJson.h"
#include "macro_memory.hpp"

String serialLine;
Adafruit_USBD_HID usb_hid;

void processJsonCommand(const String &line) {
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, line);

  if (err) {
    Serial.print("JSON cmd parse error: ");
    Serial.println(err.f_str());
    return;
  }

  const char* cmd = doc["cmd"];
  if (!cmd) {
    Serial.println("Missing cmd field.");
    return;
  }

  if (strcmp(cmd, "get_config") == 0) {
    // build and send config JSON
    JsonDocument out;
    JsonArray macros = out["macros"].to<JsonArray>();
    for (int i = 0; i < 16; i++) {
      JsonObject m = macros.add<JsonObject>();
      m["key_code"]      = current_macros[i].key_code;
      m["modifier"]      = current_macros[i].modifier;
      m["consumer_code"] = current_macros[i].consumer_code;
      m["color"]         = current_macros[i].color;
      // optionally description if you want, but keep it short
      m["description"]   = default_macros[i].description;
    }
    out["cmd"] = "config";

    serializeJson(out, Serial);
    Serial.println();
  }
  else if (strcmp(cmd, "set_config") == 0) {
    JsonArray macros = doc["macros"];
    if (!macros.isNull() && macros.size() >= 16) {
      for (int i = 0; i < 16; i++) {
        JsonObject m = macros[i];
        current_macros[i].key_code      = m["key_code"]      | current_macros[i].key_code;
        current_macros[i].modifier      = m["modifier"]      | current_macros[i].modifier;
        current_macros[i].consumer_code = m["consumer_code"] | current_macros[i].consumer_code;
        uint32_t color = m["color"]     | current_macros[i].color;
        current_macros[i].color         = color;
      }
      saveConfigToFlash();
      Serial.println("{\"status\":\"ok\",\"cmd\":\"set_config\"}");
    } else {
      Serial.println("{\"status\":\"error\",\"reason\":\"missing macros\"}");
    }
  }
  else if (strcmp(cmd, "test_macro") == 0) {
    int index = doc["index"] | -1;
    if (index >= 0 && index < 16) {
      send_macro(current_macros[index]);
      Serial.println("{\"status\":\"ok\",\"cmd\":\"test_macro\"}");
    } else {
      Serial.println("{\"status\":\"error\",\"reason\":\"bad index\"}");
    }
  }
  else {
    Serial.print("Unknown cmd: ");
    Serial.println(cmd);
  }
}