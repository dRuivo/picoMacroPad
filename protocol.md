# Pico MacroPad – JSON Serial Protocol

**Version 1.0** | _Firmware ↔ Web Configurator Interface_

---

## Protocol Overview

This document defines the JSON-based communication protocol between the **Pico MacroPad firmware** and external configuration tools (like web-based configurators).

### **Transport Details**

| Parameter       | Value            |
| --------------- | ---------------- |
| **Interface**   | USB CDC (Serial) |
| **Baud Rate**   | `115200`         |
| **Format**      | JSON objects     |
| **Encoding**    | UTF-8            |
| **Termination** | `\n` (newline)   |
| **Direction**   | Bidirectional    |

### **Message Format**

- **Commands**: `Host → Device`
- **Responses**: `Device → Host`
- **Structure**: One JSON object per line
- **Validation**: All messages must be valid JSON

### **Quick Example**

```json
{"cmd":"get_config"}
{"cmd":"set_config","macros":[...]}
{"cmd":"test_macro","index":4}
```

---

## Commands (Host → Device)

### 1. `get_config` - Retrieve Current Configuration

Requests the complete macro configuration currently loaded in device RAM.

#### **Request**

```json
{ "cmd": "get_config" }
```

#### **Response**

Returns a [`config`](#config-response) message with all 16 macro definitions.

---

### 2. `set_config` - Update Configuration

Uploads a new complete configuration to the device. The firmware will:

1. Validate the JSON structure
2. Update `current_macros[]` in RAM
3. Save configuration to LittleFS as `/macros.json`

#### **Request**

```json
{
  "cmd": "set_config",
  "macros": [
    {
      "key_code": 67,
      "modifier": 8,
      "consumer_code": 0,
      "color": 65280
    }
    // ... exactly 16 macro objects total
  ]
}
```

#### **Field Requirements**

- **`macros`**: Array of exactly **16** macro objects
- **Missing fields**: Ignored (previous values retained)
- **Invalid values**: Device will use defaults

#### **Success Response**

```json
{ "status": "ok", "cmd": "set_config" }
```

#### **Error Response**

```json
{ "status": "error", "reason": "missing macros" }
```

---

### 3. `test_macro` - Execute Macro Immediately

Triggers immediate execution of a specific macro (useful for live testing in configuration UIs).

#### **Request**

```json
{
  "cmd": "test_macro",
  "index": 7
}
```

#### **Parameters**

| Field   | Type     | Range  | Description                       |
| ------- | -------- | ------ | --------------------------------- |
| `index` | `number` | `0-15` | Zero-based macro index to execute |

#### **Success Response**

```json
{ "status": "ok", "cmd": "test_macro" }
```

#### **Error Response**

```json
{ "status": "error", "reason": "bad index" }
```

---

## Responses (Device → Host)

### `config` Response

Sent by device in response to `get_config` command.

#### **Example**

```json
{
  "cmd": "config",
  "macros": [
    {
      "key_code": 67,
      "modifier": 8,
      "consumer_code": 0,
      "color": 65280,
      "description": "Copy"
    },
    {
      "key_code": 86,
      "modifier": 8,
      "consumer_code": 0,
      "color": 255,
      "description": "Paste"
    }
    // ... exactly 16 macro objects
  ]
}
```

---

## Macro Object Schema

Every macro in the system follows this structure:

```json
{
  "key_code": 67, // HID key code (0-255)
  "modifier": 8, // Modifier bitmask (0-255)
  "consumer_code": 0, // Media/Consumer HID usage (0-65535)
  "color": 65280, // RGB color as integer (0xRRGGBB)
  "description": "Copy" // Human-readable label (read-only)
}
```

### Field Definitions

| Field               | Type     | Range        | Description                        | Example               |
| ------------------- | -------- | ------------ | ---------------------------------- | --------------------- |
| **`key_code`**      | `number` | `0-255`      | Standard HID key code (`0` = none) | `67` (C key)          |
| **`modifier`**      | `number` | `0-255`      | Modifier key bitmask               | `8` (GUI/Cmd)         |
| **`consumer_code`** | `number` | `0-65535`    | Consumer HID usage (`0` = none)    | `0x00CD` (Play/Pause) |
| **`color`**         | `number` | `0-16777215` | 24-bit RGB color                   | `0x00FF00` (Green)    |
| **`description`**   | `string` | -            | Short descriptive label            | `"Copy"`              |

### Modifier Bitmask

| Bit | Value  | Modifier   | macOS | Windows/Linux |
| --- | ------ | ---------- | ----- | ------------- |
| 0   | `0x01` | Left Ctrl  | ⌃     | Ctrl          |
| 1   | `0x02` | Left Shift | ⇧     | Shift         |
| 2   | `0x04` | Left Alt   | ⌥     | Alt           |
| 3   | `0x08` | Left GUI   | ⌘     | Win           |

#### Modifier Combinations

```json
// Single modifier
{"modifier": 8}         // ⌘ (Cmd) or Win

// Multiple modifiers (OR them together)
{"modifier": 5}         // ⌃⌥ (Ctrl+Alt) = 0x01 | 0x04 = 0x05
{"modifier": 10}        // ⇧⌘ (Shift+Cmd) = 0x02 | 0x08 = 0x0A
```

### Consumer Control Codes

For media and system control functions:

| Usage                 | Code     | Description       |
| --------------------- | -------- | ----------------- |
| `MEDIA_PLAY_PAUSE`    | `0x00CD` | Play/Pause toggle |
| `MEDIA_SCAN_NEXT`     | `0x00B5` | Next track        |
| `MEDIA_SCAN_PREVIOUS` | `0x00B6` | Previous track    |
| `MEDIA_VOL_INCREMENT` | `0x00E9` | Volume up         |
| `MEDIA_VOL_DECREMENT` | `0x00EA` | Volume down       |
| `MEDIA_MUTE`          | `0x00E2` | Mute toggle       |

---

## Error Handling

The device returns standardized error responses for various failure conditions:

### JSON Parse Errors

```json
{ "status": "error", "reason": "json_parse_error" }
```

### Missing Required Fields

```json
{ "status": "error", "reason": "invalid_command" }
```

### **Unknown Commands**

```json
{ "status": "error", "reason": "unknown_cmd" }
```

### Invalid Parameters

```json
{"status":"error","reason":"bad index"}
{"status":"error","reason":"missing macros"}
```

---

## Complete Example Session

### **Scenario**: Configure device, test a macro, then retrieve config

```bash
# 1. Get current configuration
Host → Device: {"cmd":"get_config"}
Device → Host: {"cmd":"config","macros":[...16 macro objects...]}

# 2. Test macro at index 3
Host → Device: {"cmd":"test_macro","index":3}
Device → Host: {"status":"ok","cmd":"test_macro"}

# 3. Upload new configuration
Host → Device: {"cmd":"set_config","macros":[...16 new macros...]}
Device → Host: {"status":"ok","cmd":"set_config"}

# 4. Verify the changes
Host → Device: {"cmd":"get_config"}
Device → Host: {"cmd":"config","macros":[...updated macros...]}
```

---

## 🔮 Future Extensions

_Reserved for future protocol versions:_

### **Planned Features**

- `get_status` → Device status (uptime, memory usage)
- `set_brightness` / `get_brightness` → LED brightness control
- `set_animation` → LED animation modes
- `get_version` → Firmware version info
- `factory_reset` → Restore defaults

### **Advanced Features**

- Multi-layer support (`set_layer`, `get_layer`)
- Rotary encoder configuration
- Context-aware profiles
- Live configuration sync

---

## Implementation Notes

### **Storage Details**

- **Configuration File**: `/macros.json` in LittleFS
- **Backup**: Defaults stored in firmware (fallback)
- **Persistence**: Configuration survives power cycles

### **Performance**

- **JSON Parsing**: Uses ArduinoJson library
- **Memory**: ~4KB allocated for JSON operations
- **Response Time**: <100ms for most commands

### **Compatibility**

- **Protocol Version**: `1.0`
- **Firmware**: Compatible with current codebase
- **Backward Compatibility**: Future versions will extend, not break

---

## Revision History

| Version  | Date       | Changes                                                        |
| -------- | ---------- | -------------------------------------------------------------- |
| **v1.0** | 2025-11-23 | Initial protocol with `get_config`, `set_config`, `test_macro` |
