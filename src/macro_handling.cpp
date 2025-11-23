#include "Arduino.h"
#include <Adafruit_TinyUSB.h>
#include "macro_handling.hpp"
#include "device_comms.hpp"

void send_macro(const MacroKey& macro) {
  if (!usb_hid.ready()) {
    Serial.println("HID not ready (still trying anyway)");
  }

  if (macro.consumer_code != 0) {
    // Press
    usb_hid.sendReport16(REPORT_ID_CONSUMER, macro.consumer_code);
    delay(50);
    // Release
    usb_hid.sendReport16(REPORT_ID_CONSUMER, 0);

    Serial.print("Consumer key sent: 0x");
    Serial.println(macro.consumer_code, HEX);
    return;
  }

  uint8_t report[8] = {0};
  report[0] = macro.modifier;  // Modifier byte
  report[2] = macro.key_code;   // Keycode

  usb_hid.sendReport(REPORT_ID_KEYBOARD, report, 8); // key press
  delay(50);
  memset(report, 0, sizeof(report));
  usb_hid.sendReport(REPORT_ID_KEYBOARD, report, 8); // key release

  Serial.print("Macro sent: ");
  Serial.println(macro.description);
}