## Core Firmware / HID

- [ ] Basic keyboard HID working (single key + modifiers)
- [ ] Media / consumer keys (play/pause, next/prev)
- [ ] Multi-key chords (macros that press several keys at once, not just one)
- [ ] Proper key event abstraction
- [ ] e.g. a small “action” layer: MACRO_KEYBOARD, MACRO_CONSUMER, MACRO_LAYER_SWITCH, etc.
- [ ] Tap / hold handling
- [ ] Short tap → action A
- [ ] Hold (pressed > X ms) → action B (e.g. different shortcut, layer toggle)
- [ ] Double-tap handling per key
- [ ] Double-tap → third action (e.g. “super macro” or layer switch)
- [ ] Multi-layer support
- [ ] Multiple macro banks (Layer 0 = global, Layer 1 = coding, Layer 2 = media, etc.)
- [ ] One or more keys / encoder clicks used to switch or momentarily activate layers

---

### Encoders & Pots

- [ ] Encoder support in firmware
- [ ] Count ticks (with debouncing)
- [ ] Map rotation + press to “actions” (same abstraction as keys)
- [ ] First encoder use-case: global volume knob
- [ ] Rotate → Volume Up/Down (consumer control)
- [ ] Press → Mute or Play/Pause
- [ ] Add optional scroll / zoom encoder mode (switchable via layer)
- [ ] Potentiometer support (analog read)
- [ ] Map 0–4095 → 0–100% “virtual parameter”
- [ ] First pot use-case: simple volume / brightness / parameter control
- [ ] Initially as keyboard/media stepping
- [ ] (Later can become MIDI CC if you want)

---

### LEDs, Display & Feedback (Device-side)

- [ ] Per-key LED colors from config (already partly there with color)
- [ ] LED behavior for key states
- [ ] Idle, pressed, layer indication, error state
- [ ] “Beat” / status LED modes (you already have COLOR_BEAT, etc.)
- [ ] Display support
  - [ ] Show current layer name
  - [ ] Optional text for the selected key / encoder function

---

### Configuration & JSON (next step after this)

- [ ] JSON configuration format
- [ ] Define a schema: layers → keys → actions (keyboard/consumer/layer/etc.)
- [ ] Include color, description, modifiers, multi-key lists, encoder roles, etc.
- [ ] Load JSON configuration from:
  - [ ] Flash / filesystem (LittleFS, etc.)
  - [ ] Fallback to default_macros if no config is present or JSON fails
  - [ ] Runtime config reload (e.g. on reset or special key combo)
  - [ ] Basic validation / error handling (invalid key codes, missing fields)
  - [ ] Save JSON configuration from device (allow edits from the board)

---

### Desktop / PC Side (for later, but on the list)

- [ ] Add USB CDC (serial) interface alongside HID
- [ ] Design a simple protocol for PC ↔ device:
- [ ] Device → PC: events, current layer, etc.
- [ ] PC → Device: status (volume, active app, battery), new config, pings
- [ ] Node.js desktop helper app
- [ ] Poll OS APIs for:
  - [ ] Active window / app
  - [ ] System volume
  - [ ] Battery level (on laptops)
- [ ] Send status packets to the Pico over serial
- [ ] PC-side JSON editor / online configurator (web UI)
- [ ] Export/import JSON that matches the firmware schema
- [ ] Live preview of keys, colors, layers
- [ ] Context-aware layers (long-term)
- [ ] Helper app tells the pad what app is active → pad switches layer automatically (e.g. VSCode layer, DAW layer, Browser layer)

---

### Housekeeping / Quality

- [ ] Structured logging over Serial (for debugging macros / layers / encoders)
- [ ] Simple test mode for LEDs and key/encoder inputs
- [ ] Document the JSON schema + action types (so your future self doesn’t hate you)
- [ ] Version your config format (so it can evolve it safely)
