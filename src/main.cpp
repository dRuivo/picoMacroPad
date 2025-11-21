#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include "macro_config.hpp"
#include "pico_rgb_keypad.hpp"

// Standard keyboard descriptor
uint8_t const desc_hid_report[] = {
  TUD_HID_REPORT_DESC_KEYBOARD()
};

// Hardware objects
Adafruit_USBD_HID usb_hid;
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

void send_macro(const MacroKey& macro) {
  if (!usb_hid.ready()) {
    Serial.println("HID not ready (still trying anyway)");
  }

  uint8_t report[8] = {0};
  report[0] = macro.modifier;  // Modifier byte
  report[2] = macro.key_code;   // Keycode

  usb_hid.sendReport(0, report, 8); // key press
  delay(50);
  memset(report, 0, sizeof(report));
  usb_hid.sendReport(0, report, 8); // key release

  Serial.print("Macro sent: ");
  Serial.println(macro.description);
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
}