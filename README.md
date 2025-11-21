# Pico RGB Keypad Macro Keyboard

A customizable macro keyboard implementation for the [Pimoroni Pico RGB Keypad](https://shop.pimoroni.com/products/pico-rgb-keypad-base) using a Raspberry Pi Pico 2.

## Features

- **16 Customizable Macro Keys**: Each key can be programmed with different key combinations
- **RGB LED Feedback**: Each key has its own color and visual feedback when pressed
- **USB HID Keyboard**: Works as a standard USB keyboard with any computer
- **Modifier Key Support**: Supports Ctrl, Alt, Shift, and Windows/Cmd key combinations
- **Easy Configuration**: Simple header file configuration for customizing macros

## Hardware Requirements

- Raspberry Pi Pico 2
- [Pimoroni Pico RGB Keypad Base](https://shop.pimoroni.com/products/pico-rgb-keypad-base?variant=32369517166675)
- USB cable for programming and power

## Software Requirements

- PlatformIO IDE or Arduino IDE
- Adafruit TinyUSB Library (automatically installed via PlatformIO)

## Default Macro Configuration

The default configuration includes common shortcuts:

### Row 1 (Keys 0-3)

- **Key 0**: Copy (Ctrl+C) - Green
- **Key 1**: Paste (Ctrl+V) - Blue
- **Key 2**: Undo (Ctrl+Z) - Red
- **Key 3**: Save (Ctrl+S) - Yellow

### Row 2 (Keys 4-7)

- **Key 4**: Alt+Tab - Magenta
- **Key 5**: Lock Screen (Win+L) - Cyan
- **Key 6**: Refresh (F5) - Lime
- **Key 7**: Escape - Orange

### Row 3 (Keys 8-11)

- **Key 8**: Play/Pause (Space) - Purple
- **Key 9**: Previous Track (←) - Sky Blue
- **Key 10**: Next Track (→) - Pink
- **Key 11**: Delete - Dark Red

### Row 4 (Keys 12-15)

- **Key 12**: Enter - Dark Green
- **Key 13**: Backspace - Dark Blue
- **Key 14**: Home - Olive
- **Key 15**: End - Purple

## Customizing Macros

To customize the macro keys, edit the `default_macros` array in `src/macro_config.hpp`:

```cpp
const MacroKey default_macros[16] = {
  {HID_KEY_C, CTRL_C, "Copy", 0x00FF00},  // Key, Modifier, Description, Color
  // ... more configurations
};
```

### MacroKey Structure

```cpp
struct MacroKey {
  uint8_t key_code;        // HID key code (e.g., HID_KEY_A)
  uint8_t modifier;        // Modifier combination
  const char* description; // Human-readable description
  uint32_t color;         // RGB color (0xRRGGBB format)
};
```

### Available Modifiers

- `MODIFIER_NONE`: No modifier keys
- `MODIFIER_CTRL`: Ctrl key
- `MODIFIER_SHIFT`: Shift key
- `MODIFIER_ALT`: Alt key
- `MODIFIER_GUI`: Windows/Cmd key

You can combine modifiers using bitwise OR:

```cpp
MODIFIER_CTRL | MODIFIER_SHIFT  // Ctrl+Shift combination
```

### Common HID Key Codes

- `HID_KEY_A` through `HID_KEY_Z`: Letter keys
- `HID_KEY_1` through `HID_KEY_0`: Number keys
- `HID_KEY_F1` through `HID_KEY_F12`: Function keys
- `HID_KEY_ENTER`: Enter key
- `HID_KEY_SPACE`: Spacebar
- `HID_KEY_ESCAPE`: Escape key
- `HID_KEY_TAB`: Tab key
- `HID_KEY_BACKSPACE`: Backspace
- `HID_KEY_DELETE`: Delete key
- `HID_KEY_ARROW_UP/DOWN/LEFT/RIGHT`: Arrow keys

### Color Format

Colors are specified in 24-bit RGB format (0xRRGGBB):

- `0xFF0000`: Red
- `0x00FF00`: Green
- `0x0000FF`: Blue
- `0xFFFF00`: Yellow
- `0xFF00FF`: Magenta
- `0x00FFFF`: Cyan
- `0xFFFFFF`: White

## Building and Uploading

### Using PlatformIO

1. Open the project in PlatformIO
2. Build the project:
   ```
   platformio run
   ```
3. Upload to your Pico 2:
   ```
   platformio run --target upload
   ```

### Using Arduino IDE

1. Install the Adafruit TinyUSB Library
2. Select "Raspberry Pi Pico 2" as your board
3. Enable "USB Stack: Adafruit TinyUSB"
4. Upload the code

## Usage

1. Connect the macro keyboard to your computer via USB
2. The device will appear as a standard USB keyboard
3. Each key will light up with its configured color
4. Press any key to send its programmed macro
5. Keys will flash white when pressed

## Visual Feedback

- **Idle State**: Keys show their configured colors at low brightness
- **Key Press**: Keys flash bright white when pressed
- **Beat Indicator**: Key 15 shows a subtle red pulse when no keys are active

## Serial Debug Output

Connect to the serial port at 115200 baud to see debug information:

- Macro configuration on startup
- Key press events with sent key codes
- USB HID status

## Troubleshooting

### Device Not Recognized

- Ensure USB cable supports data transfer
- Try a different USB port
- Check that TinyUSB is properly initialized

### Keys Not Working

- Verify HID key codes in configuration
- Check serial output for error messages
- Ensure USB HID is ready before sending keys

### LED Issues

- Check power supply (USB should be sufficient)
- Verify I2C connections on the keypad
- Adjust brightness settings if needed

## Advanced Customization

### Adding More Complex Macros

For more complex macros (like typing text strings), you can modify the `send_macro()` function to handle special cases:

```cpp
void send_macro(const MacroKey& macro) {
  // Handle special macro types
  if (macro.key_code == SPECIAL_MACRO_CODE) {
    // Send a sequence of keys
    // Implementation depends on your needs
    return;
  }

  // Normal key handling...
}
```

### Runtime Configuration

You can add functionality to change macros at runtime by detecting special key combinations or implementing a configuration mode.

## Contributing

Feel free to contribute improvements, additional macro types, or bug fixes!

## License

This project is open source. Please check individual library licenses for compatibility.
