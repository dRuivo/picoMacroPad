#include <Arduino.h>
#include <LittleFS.h>
#include <Adafruit_TinyUSB.h>
#include "device_comms.hpp"
#include "macro_handling.hpp"
#include "macro_memory.hpp"
#include "pico_rgb_keypad.hpp"

pimoroni::PicoRGBKeypad pico_keypad;

// State variables
uint16_t button_states = 0;
uint16_t last_button_states = 0;
uint32_t last_update = 0;

// Function to convert 24-bit color to RGB components
void color_to_rgb(uint32_t color, uint8_t &r, uint8_t &g, uint8_t &b) {
  r = (color >> 16) & 0xFF;
  g = (color >> 8) & 0xFF;
  b = color & 0xFF;
}

// Update LED colors based on current state
void update_leds() {
  uint32_t current_time = millis();

  for (int i = 0; i < pimoroni::PicoRGBKeypad::NUM_PADS; i++) {
    uint8_t r, g, b;
  
    // Check if button is currently pressed
    bool is_pressed = (button_states & (1 << i)) != 0;
  
    if (is_pressed) {
      // Bright white when pressed
      color_to_rgb(COLOR_PRESSED, r, g, b);
      r = (r * BRIGHTNESS_BRIGHT);
      g = (g * BRIGHTNESS_BRIGHT);
      b = (b * BRIGHTNESS_BRIGHT);
    } else {
      // Show macro color when not pressed
      color_to_rgb(default_macros[i].color, r, g, b);
      r = (r * BRIGHTNESS_DIM);
      g = (g * BRIGHTNESS_DIM);
      b = (b * BRIGHTNESS_DIM);
    }
  
    pico_keypad.illuminate(i, r, g, b);
  }

  // Add a subtle beat indicator on key 15 if no keys are pressed
  if (button_states == 0) {
    if ((current_time / 1000) % 2 == 0) {
      uint8_t beat_r, beat_g, beat_b;
      color_to_rgb(COLOR_BEAT, beat_r, beat_g, beat_b);
      beat_r *= BRIGHTNESS_DIM;
      beat_g *= BRIGHTNESS_DIM;
      beat_b *= BRIGHTNESS_DIM;
      pico_keypad.illuminate(15, beat_r, beat_g, beat_b);
    }
  }

  pico_keypad.update();
}

void process_keyboard() {
  // Read current button states
  button_states = pico_keypad.get_button_states();
  
  // Check for newly pressed buttons
  uint16_t pressed_buttons = button_states & ~last_button_states;
  
  // Process each newly pressed button
  for (int i = 0; i < pimoroni::PicoRGBKeypad::NUM_PADS; i++) {
    if (pressed_buttons & (1 << i)) {
      // Button was just pressed
      Serial.print("Button ");
      Serial.print(i);
      Serial.println(" pressed!");
      send_macro(default_macros[i]);
    }
  }
  last_button_states = button_states;
  update_leds();
}

void setup() {
  Serial.begin(115200);

  usb_hid.setBootProtocol(HID_ITF_PROTOCOL_KEYBOARD);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setStringDescriptor("Pico Macro Keyboard");
  usb_hid.setPollInterval(10);
  usb_hid.begin();

  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed, using defaults.");
    loadDefaultMacros();
  } else {
    Serial.println("LittleFS mounted.");
    if (!loadConfigFromFlash()) {   // we’ll define this in a second
      Serial.println("No valid config, using defaults.");
      loadDefaultMacros();
    }
  }

  // Wait until USB mounted (if possible)
  uint32_t start = millis();
  while (!TinyUSBDevice.mounted() && (millis() - start < 5000)) {
    delay(10);
  }

  pico_keypad.init();
  pico_keypad.set_brightness(BRIGHTNESS_NORMAL);

  update_leds();

  Serial.println("Pico2 HID test starting...");
}

void loop() {
  process_keyboard();

  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (serialLine.length() > 0) {
        processJsonCommand(serialLine);
        serialLine = "";
      }
    } else {
      serialLine += c;
      // basic safety
      if (serialLine.length() > 512) {
        serialLine = "";
      }
    }
  }
}