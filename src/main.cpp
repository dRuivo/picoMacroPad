
#include "Arduino.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include <Adafruit_TinyUSB.h>

#include "pico_rgb_keypad.hpp"
#include "macro_config.hpp"

/*
  Pico RGB Keypad Macro Keyboard
  
  Features:
  - 16 customizable macro keys
  - USB HID keyboard functionality
  - RGB LED feedback
  - Configurable key mappings with modifiers
  - Visual feedback for key presses
*/

using namespace pimoroni;

// Hardware instances
PicoRGBKeypad pico_keypad;
Adafruit_USBD_HID usb_hid;

// State variables
uint16_t button_states = 0;
uint16_t last_button_states = 0;
uint32_t last_update = 0;
uint8_t beat_counter = 0;
bool hid_ready = false;

// Macro configuration (can be customized)
MacroKey current_macros[16];

// Function to convert 24-bit color to RGB components
void color_to_rgb(uint32_t color, uint8_t &r, uint8_t &g, uint8_t &b) {
  r = (color >> 16) & 0xFF;
  g = (color >> 8) & 0xFF;
  b = color & 0xFF;
}

// Update LED colors based on current state
void update_leds() {
  uint32_t current_time = millis();
  
  for (int i = 0; i < PicoRGBKeypad::NUM_PADS; i++) {
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
      color_to_rgb(current_macros[i].color, r, g, b);
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

// Initialize the macro keyboard
void setup_macro_keyboard() {
  // Copy default macro configuration
  for (int i = 0; i < 16; i++) {
    current_macros[i] = default_macros[i];
  }
  
  // Initialize hardware
  pico_keypad.init();
  pico_keypad.set_brightness(BRIGHTNESS_NORMAL);
  
  // Initialize USB HID 
  usb_hid.setPollInterval(2);
  usb_hid.begin();
  
  // Wait for USB to be ready
  while (!TinyUSBDevice.mounted()) {
    delay(1);
  }
  
  hid_ready = true;
  
  // Initial LED setup - show macro colors
  update_leds();
}

// Send macro key combination
void send_macro(const MacroKey& macro) {
  if (!hid_ready || !usb_hid.ready()) {
    return;
  }
  
  // Send key press using the correct TinyUSB API
  uint8_t keycodes[6] = {macro.key_code, 0, 0, 0, 0, 0};
  usb_hid.keyboardReport(0, macro.modifier, keycodes);
  
  delay(10); // Small delay to ensure key is registered
  
  // Release all keys (send empty report)
  uint8_t empty_keys[6] = {0, 0, 0, 0, 0, 0};
  usb_hid.keyboardReport(0, 0, empty_keys);
  
  // Debug output
  Serial.print("Macro sent: ");
  Serial.print(macro.description);
  Serial.print(" (Key: 0x");
  Serial.print(macro.key_code, HEX);
  Serial.print(", Mod: 0x");
  Serial.print(macro.modifier, HEX);
  Serial.println(")");
}

// Main keyboard processing loop
void process_keyboard() {
  // Read current button states
  button_states = pico_keypad.get_button_states();
  
  // Check for newly pressed buttons
  uint16_t pressed_buttons = button_states & ~last_button_states;
  
  // Process each newly pressed button
  for (int i = 0; i < PicoRGBKeypad::NUM_PADS; i++) {
    if (pressed_buttons & (1 << i)) {
      // Button was just pressed
      send_macro(current_macros[i]);
    }
  }
  
  // Update LED display
  update_leds();
  
  // Store current state for next iteration
  last_button_states = button_states;
}

// Print macro configuration to serial
void print_macro_config() {
  Serial.println("\n=== Current Macro Configuration ===");
  for (int i = 0; i < 16; i++) {
    Serial.print("Key ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(current_macros[i].description);
    Serial.print(" (0x");
    Serial.print(current_macros[i].key_code, HEX);
    Serial.print(", Mod: 0x");
    Serial.print(current_macros[i].modifier, HEX);
    Serial.print(", Color: 0x");
    Serial.print(current_macros[i].color, HEX);
    Serial.println(")");
  }
  Serial.println("==================================\n");
}

void setup() {
  // Initialize serial for debugging
  Serial.begin(115200);
  
  // Small delay to allow serial to initialize
  delay(1000);
  
  Serial.println("Pico RGB Keypad Macro Keyboard Starting...");
  
  // Setup the macro keyboard
  setup_macro_keyboard();
  
  Serial.println("Macro keyboard ready!");
  print_macro_config();
}

void loop() {
  process_keyboard();
    delay(10); // 100Hz update rate
}