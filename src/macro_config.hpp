#pragma once
#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

// Macro keyboard configuration
struct MacroKey {
  uint8_t key_code;           // HID key code
  uint8_t modifier;           // Modifier keys (CTRL, ALT, etc.)
  const char* description;    // Human readable description
  uint32_t color;            // RGB color for this key (0xRRGGBB)
};

// Modifier key definitions
#define MODIFIER_NONE     0x00
#define MODIFIER_CTRL     0x01
#define MODIFIER_SHIFT    0x02
#define MODIFIER_ALT      0x04
#define MODIFIER_GUI      0x08  // Windows/Cmd key

enum Colors {
    GREEN = 0x00FF00,
    BLUE = 0x0000FF,
    RED = 0xFF0000,
    YELLOW = 0xFFFF00,
    MAGENTA = 0xFF00FF,
    CYAN = 0x00FFFF,
    LIME = 0x80FF00,
    ORANGE = 0xFF8000,
    PURPLE = 0x8000FF,
    SKY_BLUE = 0x0080FF,
};

// Default macro configuration (16 keys)
const MacroKey default_macros[16] = {
  // Row 1 (keys 0-3)
  {HID_KEY_C,           MODIFIER_GUI,        "Copy",           0x00FF00},  // Green
  {HID_KEY_V,           MODIFIER_GUI,        "Paste",          0x0000FF},  // Blue
  {HID_KEY_Z,           MODIFIER_GUI,        "Undo",           0xFF0000},  // Red
  {HID_KEY_S,           MODIFIER_GUI,        "Save",           0xFFFF00},  // Yellow
  
  // Row 2 (keys 4-7)
  {HID_KEY_TAB,         MODIFIER_ALT,       "Alt+Tab",        0xFF00FF},  // Magenta
  {HID_KEY_L,           MODIFIER_GUI,         "Lock Screen",    0x00FFFF},  // Cyan
  {HID_KEY_F5,          MODIFIER_NONE, "Refresh",        0x80FF00},  // Lime
  {HID_KEY_ESCAPE,      MODIFIER_NONE, "Escape",         0xFF8000},  // Orange
  
  // Row 3 (keys 8-11)
  {HID_KEY_SPACE,       MODIFIER_NONE, "Play/Pause",     0x8000FF},  // Purple
  {HID_KEY_ARROW_LEFT,  MODIFIER_NONE, "Previous Track", 0x0080FF},  // Sky Blue
  {HID_KEY_ARROW_RIGHT, MODIFIER_NONE, "Next Track",     0xFF0080},  // Pink
  {HID_KEY_DELETE,      MODIFIER_NONE, "Delete",         0x800000},  // Dark Red
  
  // Row 4 (keys 12-15)
  {HID_KEY_ENTER,       MODIFIER_NONE, "Enter",          0x008000},  // Dark Green
  {HID_KEY_BACKSPACE,   MODIFIER_NONE, "Backspace",      0x000080},  // Dark Blue
  {HID_KEY_HOME,        MODIFIER_NONE, "Home",           0x808000},  // Olive
  {HID_KEY_END,         MODIFIER_NONE, "End",            0x800080}   // Purple
};

// Color definitions for LED states
#define COLOR_OFF         0x000000
#define COLOR_PRESSED     0xFFFFFF  // White when pressed
#define COLOR_DIM         0x101010  // Dim white when idle
#define COLOR_BEAT        0xFF0000  // Red for beat indicator

// Brightness settings
#define BRIGHTNESS_NORMAL 0.5f
#define BRIGHTNESS_DIM    0.1f
#define BRIGHTNESS_BRIGHT 1.0f