# Quick Setup Guide

## Option 1: PlatformIO (Recommended)

1. **Install PlatformIO**:

   - Install the PlatformIO extension in VS Code
   - Or install PlatformIO CLI: `pip install platformio`

2. **Open the project**:

   - Open the project folder in VS Code
   - PlatformIO should automatically detect the `platformio.ini` file

3. **Build and Upload**:

   ```bash
   platformio run                    # Build
   platformio run --target upload   # Upload
   ```

4. **Monitor Serial Output**:
   ```bash
   platformio device monitor
   ```

## Option 2: Arduino IDE

1. **Install Board Support**:

   - Open Arduino IDE
   - Go to File > Preferences
   - Add this URL to "Additional Boards Manager URLs":
     ```
     https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
     ```
   - Go to Tools > Board > Boards Manager
   - Search for "pico" and install "Raspberry Pi Pico/RP2040"

2. **Install Required Libraries**:

   - Go to Tools > Manage Libraries
   - Search and install: "Adafruit TinyUSB Library"

3. **Configure Board Settings**:

   - Tools > Board > Raspberry Pi Pico/RP2040 > Raspberry Pi Pico 2
   - Tools > USB Stack > Adafruit TinyUSB

4. **Copy Pimoroni Files**:

   - Copy `pico_rgb_keypad.hpp` and `pico_rgb_keypad.cpp` to your Arduino sketch folder
   - Open `macro_keyboard_arduino.ino` in Arduino IDE

5. **Upload**:
   - Connect your Pico 2
   - Click Upload

## Hardware Connection

1. **Assemble the Keypad**:

   - Solder the Pico 2 to the Pico RGB Keypad Base
   - Ensure all connections are solid

2. **Connect to Computer**:
   - Use a USB cable that supports data transfer
   - Connect the Pico 2 to your computer

## Troubleshooting

### Device Not Detected

- Try a different USB cable
- Press and hold BOOTSEL button while plugging in
- Check that "Adafruit TinyUSB" is selected as USB Stack

### Build Errors

- Make sure all library dependencies are installed
- Check that the Pimoroni library files are in the correct location
- Verify board and USB stack settings

### Keys Not Working

- Open Serial Monitor to check for debug output
- Verify USB HID initialization messages
- Test with simple key combinations first

## Customization

Edit the `default_macros` array in the code to customize your key mappings:

```cpp
const MacroKey default_macros[16] = {
  {HID_KEY_A, MODIFIER_CTRL, "Ctrl+A", 0xFF0000},  // Red Ctrl+A
  // ... add your own macros
};
```

## Key Mapping Reference

Common HID key codes:

- `HID_KEY_A` through `HID_KEY_Z` - Letter keys
- `HID_KEY_1` through `HID_KEY_0` - Number keys
- `HID_KEY_F1` through `HID_KEY_F12` - Function keys
- `HID_KEY_ENTER` - Enter key
- `HID_KEY_SPACE` - Spacebar
- `HID_KEY_ESCAPE` - Escape
- `HID_KEY_TAB` - Tab
- `HID_KEY_BACKSPACE` - Backspace
- `HID_KEY_DELETE` - Delete

Modifier combinations:

- `MODIFIER_CTRL` - Ctrl key
- `MODIFIER_SHIFT` - Shift key
- `MODIFIER_ALT` - Alt key
- `MODIFIER_GUI` - Windows/Cmd key
- `MODIFIER_CTRL | MODIFIER_SHIFT` - Ctrl+Shift

Colors (0xRRGGBB format):

- `0xFF0000` - Red
- `0x00FF00` - Green
- `0x0000FF` - Blue
- `0xFFFF00` - Yellow
- `0xFF00FF` - Magenta
- `0x00FFFF` - Cyan
