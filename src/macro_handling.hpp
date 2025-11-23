#pragma once
#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

// Macro keyboard configuration
struct MacroKey {
  uint8_t key_code;           // HID key code
  uint8_t modifier;           // Modifier keys (CTRL, ALT, etc.)
  uint16_t consumer_code;     // 0 = none, otherwise Consumer usage (media)
  const char* description;    // Human readable description
  uint32_t color;            // RGB color for this key (0xRRGGBB)
};

// Modifier key definitions
#define MODIFIER_NONE     0x00
#define MODIFIER_CTRL     0x01
#define MODIFIER_SHIFT    0x02
#define MODIFIER_ALT      0x04
#define MODIFIER_GUI      0x08  // Windows/Cmd key

// Consumer control usage IDs (16-bit)
#define MEDIA_PLAY_PAUSE       0x00CD
#define MEDIA_SCAN_NEXT        0x00B5
#define MEDIA_SCAN_PREVIOUS    0x00B6
#define MEDIA_VOL_INCREMENT    0x00E9
#define MEDIA_VOL_DECREMENT    0x00EA
#define MEDIA_MUTE             0x00E2

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
  {HID_KEY_C,           MODIFIER_GUI,0,        "Copy",           0x00FF00},  // Green
  {HID_KEY_V,           MODIFIER_GUI,0,        "Paste",          0x0000FF},  // Blue
  {HID_KEY_Z,           MODIFIER_GUI,0,        "Undo",           0xFF0000},  // Red
  {HID_KEY_S,           MODIFIER_GUI,0,        "Save",           0xFFFF00},  // Yellow
  
  // Row 2 (keys 4-7)
  {HID_KEY_TAB,         MODIFIER_GUI,0,       "CMD+Tab",        0xFF00FF},  // Magenta
  {HID_KEY_L,           MODIFIER_GUI,0,         "Lock Screen",    0x00FFFF},  // Cyan
  {HID_KEY_Z,          MODIFIER_GUI | MODIFIER_SHIFT,0, "Redo",        0x80FF00},  // Lime
  {HID_KEY_ESCAPE,      MODIFIER_NONE,0, "Escape",         0xFF8000},  // Orange
  
  // Row 3 (keys 8-11)
  {0,       MODIFIER_NONE,MEDIA_PLAY_PAUSE, "Play/Pause",     0x8000FF},  // Purple
  {0,  MODIFIER_NONE,MEDIA_SCAN_PREVIOUS, "Previous Track", 0x0080FF},  // Sky Blue
  {0, MODIFIER_NONE,MEDIA_SCAN_NEXT, "Next Track",     0xFF0080},  // Pink
  {HID_KEY_DELETE,      MODIFIER_NONE,0, "Delete",         0x800000},  // Dark Red
  
  // Row 4 (keys 12-15)
  {HID_KEY_ENTER,       MODIFIER_NONE,0, "Enter",          0x008000},  // Dark Green
  {HID_KEY_BACKSPACE,   MODIFIER_NONE,0, "Backspace",      0x000080},  // Dark Blue
  {HID_KEY_HOME,        MODIFIER_NONE,0, "Home",           0x808000},  // Olive
  {HID_KEY_END,         MODIFIER_NONE,0, "End",            0x800080}   // Purple
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

void send_macro(const MacroKey& macro);