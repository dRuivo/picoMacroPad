#pragma once
#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

enum {
  REPORT_ID_KEYBOARD = 1,
  REPORT_ID_CONSUMER = 2,
};

// Standard keyboard + consumer control descriptor
uint8_t const desc_hid_report[] = {
  TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(REPORT_ID_KEYBOARD)),
  TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(REPORT_ID_CONSUMER))
};

// Hardware objects
extern Adafruit_USBD_HID usb_hid;

void processJsonCommand(const String &line);

extern String serialLine;

