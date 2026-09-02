# PerfectPark Parts Shopping List

Use this list to buy everything needed for a **PerfectPark v1** build. Quantities
are listed for the default **3-spot** configuration; adjust HC-SR04 and resistor
counts if you build fewer spots.

For wiring and GPIO details, see [wiring.md](wiring.md).

---

## Quick checklist (3-spot build)

Copy this into your cart or notes:

- [ ] ESP32-S3 DevKitC-1 (N16R8 with PSRAM recommended)
- [ ] HC-SR04 ultrasonic sensor × 3
- [ ] 1 kΩ resistor × 3
- [ ] 2 kΩ resistor × 3
- [ ] Half-size or full-size breadboard × 1
- [ ] Jumper wire kit (M-M, M-F)
- [ ] USB-C data cable × 1
- [ ] 5 V / 1 A+ USB wall adapter × 1
- [ ] 100 nF ceramic capacitor × 3 (optional, recommended)

---

## Required parts

### Microcontroller

| Item | Qty | Notes | Search terms |
|------|-----|-------|--------------|
| ESP32-S3-DevKitC-1 | 1 | Prefer **N16R8** (16 MB flash, 8 MB PSRAM) for headroom | `ESP32-S3-DevKitC-1 N16R8` |

**Why this board:** Built-in WiFi, native USB, and strong support in PlatformIO.

**Avoid for v1:** ESP32 (non-S3) works with code changes; Pico W does not match
the current firmware target.

---

### Sensors

| Item | Qty (3-spot) | Qty (2-spot) | Notes | Search terms |
|------|--------------|--------------|-------|--------------|
| HC-SR04 ultrasonic module | 3 | 2 | Standard 5 V unit with 4 pins (VCC, Trig, Echo, GND) | `HC-SR04 ultrasonic sensor` |

**One sensor per parking spot.** Mount on the garage wall at bumper height.

---

### Resistors (Echo voltage dividers)

| Item | Qty (3-spot) | Qty (2-spot) | Notes | Search terms |
|------|--------------|--------------|-------|--------------|
| 1 kΩ resistor | 3 | 2 | ¼ W metal film or carbon film | `1k ohm resistor` |
| 2 kΩ resistor | 3 | 2 | ¼ W; 2.2 kΩ also works | `2k ohm resistor` |

**Alternative:** One bidirectional logic level converter per sensor instead of
resistors (more expensive, cleaner for permanent installs).

---

### Prototyping

| Item | Qty | Notes | Search terms |
|------|-----|-------|--------------|
| Breadboard (830 tie-point) | 1 | Half-size OK for bench testing | `830 tie point breadboard` |
| Jumper wire kit | 1 | Include male–male and male–female | `breadboard jumper wire kit` |

For a permanent garage install, plan to move from breadboard to a small perfboard
or screw-terminal breakout later.

---

### Power

| Item | Qty | Notes | Search terms |
|------|-----|-------|--------------|
| USB-C cable (data-capable) | 1 | Required for flashing and power | `USB-C data cable` |
| 5 V USB wall adapter | 1 | **1 A minimum**; 2 A preferred with 3 sensors | `5V 2A USB charger` |

Power the ESP32-S3 and all HC-SR04 modules from the same 5 V supply with shared
GND.

---

## Optional but recommended

| Item | Qty (3-spot) | Why |
|------|--------------|-----|
| 100 nF ceramic capacitor | 3 | Decoupling at each HC-SR04 (VCC to GND) |
| Small project enclosure | 1 | Protect ESP32 near the garage |
| Adhesive cable clips | 1 pack | Route sensor wires cleanly |
| Heat-shrink tubing | 1 assortment | Insulate divider joints in permanent installs |
| Logic level converter (4-channel) | 1 | Replace resistor dividers if you prefer |

---

## Optional upgrades (not required for v1)

| Item | Purpose |
|------|---------|
| Perfboard + screw terminals | Permanent wiring instead of breadboard |
| 3× green / yellow / red LEDs + 220 Ω resistors | On-site visual feedback at each spot |
| Active buzzer | Audio alert when too close |
| ESP32-S3 case with ventilation | Dust and moisture protection |

These are not wired in v1 firmware yet; add only if you plan to extend the project.

---

## Quantity summary

| Build | HC-SR04 | 1 kΩ | 2 kΩ | ESP32-S3 |
|-------|---------|------|------|----------|
| 2-spot | 2 | 2 | 2 | 1 |
| 3-spot (default) | 3 | 3 | 3 | 1 |

Everything else (breadboard, wires, USB cable, adapter) is **1×** regardless of
spot count.

---

## Estimated cost (USD, approximate)

Prices vary by vendor and region. Typical hobby-market ranges:

| Category | Approx. range |
|----------|----------------|
| ESP32-S3 DevKitC-1 | $8 – $15 |
| HC-SR04 (each) | $2 – $4 |
| Resistors + breadboard + wires | $8 – $15 |
| USB cable + adapter | $5 – $12 |
| **3-spot total** | **~$30 – $55** |

Buying a sensor kit or ESP32 starter bundle can reduce shipping cost if you do
not already have a breadboard and jumper wires.

---

## Where to buy

Any reputable electronics retailer works. Common options:

- [Adafruit](https://www.adafruit.com/) — search ESP32-S3 and HC-SR04
- [SparkFun](https://www.sparkfun.com/) — ESP32-S3 boards and jumper kits
- [Amazon](https://www.amazon.com/) — fast shipping; verify board is **ESP32-S3-DevKitC-1**
- [AliExpress / Banggood](https://www.aliexpress.com/) — lower unit cost, longer shipping
- [DigiKey](https://www.digikey.com/) / [Mouser](https://www.mouser.com/) — reliable parts for resistors and caps

**Before checkout:** Confirm the ESP32 board listing says **S3** (not ESP32 classic
or ESP32-C3).

---

## What you do not need for v1

| Item | Reason |
|------|--------|
| Raspberry Pi Pico | Firmware targets ESP32-S3 |
| Separate 3.3 V regulator | ESP32 board includes one |
| OLED display | Web dashboard replaces on-device display |
| External WiFi module | WiFi is built into ESP32-S3 |
| Cloud subscription | Dashboard runs on the device |

---

## Related docs

- [Wiring diagram](wiring.md) — how to connect everything
- [README](../README.md) — firmware build and flash steps
- [config.h](../firmware-arduino/include/config.h) — spot count and GPIO pins
