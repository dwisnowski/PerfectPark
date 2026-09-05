# PerfectPark Parts Shopping List

Use this list to buy everything needed for a **PerfectPark v1** build. Quantities
are listed for the default **3-spot** configuration; adjust HC-SR04 and resistor
counts if you build fewer spots.

For wiring and GPIO details, see [wiring.md](wiring.md).

**Prices and stock change often.** Verify the listing matches the specs in the
Notes column before you buy.

---

## Recommended products (Amazon & Adafruit)

These are specific picks that match PerfectPark v1. Buy from either store, or mix
and match.

### Microcontroller (pick one)

| Part | Qty | Amazon | Adafruit | Notes |
|------|-----|--------|----------|-------|
| ESP32-S3-DevKitC-1 **N8R8** (8 MB flash, 8 MB PSRAM) | 1 | [Stemedu ESP32-S3-DevKitC-1 N8R8](https://www.amazon.com/Stemedu-ESP32-S3-DevKitC-1-Development-ESP32-S3-WROOM-1-Microcontroller/dp/B09VBFMTDS) | [ESP32-S3-DevKitC-1 — select **8MB Flash 8MB PSRAM** variant](https://www.adafruit.com/product/5312) | **Best match** for the firmware `platformio.ini` target. Confirm listing says **ESP32-S3** with **8 MB PSRAM**. |
| ESP32-S3 DevKitC-1 (Espressif listing) | 1 | [Espressif ESP32-S3-DevKitC-1-N8R8](https://www.amazon.com/Espressif-ESP32-S3-DevKitC-1-N8R8-Development-Board/dp/B09MHP42LY) | — | Official Espressif board. Check USB connector type (Micro-USB vs USB-C) and buy the matching cable. |
| Adafruit Metro ESP32-S3 (alternate) | 1 | — | [Metro ESP32-S3 — 16 MB flash, 8 MB PSRAM](https://www.adafruit.com/product/5500) | Works with Arduino, but **GPIO pins differ** from DevKitC-1. Update `config.h` if you use this board. |

---

### Ultrasonic sensors (3-spot build)

| Part | Qty | Amazon | Adafruit | Notes |
|------|-----|--------|----------|-------|
| HC-SR04 module | 3 | [Ferwooh 3-pack HC-SR04 + brackets + wires](https://www.amazon.com/Ferwooh-Ultrasonic-Distance-Measuring-Mounting/dp/B0D1MDP9V3) | [HC-SR04 + 2× 10 kΩ resistors](https://www.adafruit.com/product/3942) × **3** | One sensor per parking spot. Adafruit units include **10 kΩ** divider resistors (not 1 kΩ/2 kΩ); they work, but buy separate 1 kΩ + 2.2 kΩ packs below for our preferred divider. |

---

### Resistors (Echo voltage dividers)

| Part | Qty | Amazon | Adafruit | Notes |
|------|-----|--------|----------|-------|
| 1 kΩ resistor | 3 | Included in [starter kit below](https://www.amazon.com/dp/B09FDC6BX6) | [1.0 kΩ — 25 pack](https://www.adafruit.com/product/4294) | One divider set per sensor. |
| 2 kΩ resistor | 3 | Included in [starter kit below](https://www.amazon.com/dp/B09FDC6BX6) | [2.2 kΩ — 25 pack](https://www.adafruit.com/product/2782) | Adafruit sells **2.2 kΩ** (not 2 kΩ); it works fine for the Echo divider. |

---

### Breadboard & jumper wires

| Part | Qty | Amazon | Adafruit | Notes |
|------|-----|--------|----------|-------|
| 830 tie-point breadboard + wire kit | 1 | [Electronics Component Fun Kit](https://www.amazon.com/dp/B09FDC6BX6) | [Half-size breadboard + 78 jumper wires](https://www.adafruit.com/product/3314) | Amazon kit also includes **1 kΩ/2 kΩ resistors** and **100 nF caps** — good single-cart option. |
| Full-size breadboard (optional) | 1 | Search `830 tie point breadboard` on Amazon | [Full Sized Premium Breadboard — 830 tie points](https://www.adafruit.com/product/239) | More room for 3 sensors + ESP32. |
| Male–female jumper wires | 1 | Included in kits above | [Premium M/F jumper wires — 40 × 6"](https://www.adafruit.com/product/826) | Useful for reaching HC-SR04 modules. |
| Male–male jumper wires | 1 | Included in kits above | [Breadboarding wire bundle — 75 wires](https://www.adafruit.com/product/153) | For breadboard prototyping. |

---

### Power & USB

| Part | Qty | Amazon | Adafruit | Notes |
|------|-----|--------|----------|-------|
| USB cable (data-capable) | 1 | Often included with ESP32 board; or search `USB-A to USB-C data cable` | [USB-A to USB-C — 1 m / 3 ft](https://www.adafruit.com/product/4474) | Must support **data**, not charge-only. Match connector to your DevKit (USB-C on most current boards). |
| 5 V USB wall adapter (≥ 2 A) | 1 | [Amazon Basics 5 V 2.4 A USB-A adapter](https://www.amazon.com/AmazonBasics-One-Port-USB-Wall-Charger/dp/B0773BHCV1) | [5 V 2 A switching supply — USB-A port](https://www.adafruit.com/product/1994) | Pair with the USB cable above. **2 A** recommended with 3 sensors. |

---

### Optional but recommended

| Part | Qty | Amazon | Adafruit | Notes |
|------|-----|--------|----------|-------|
| 100 nF (0.1 µF) ceramic capacitor | 3 | Included in [Electronics Component Fun Kit](https://www.amazon.com/dp/B09FDC6BX6) | [0.1 µF capacitors — 10 pack](https://www.adafruit.com/product/753) | Place across VCC/GND at each HC-SR04 for cleaner readings. |

---

### Fastest cart options

**Amazon — buy in two items**

1. [Stemedu ESP32-S3-DevKitC-1 N8R8](https://www.amazon.com/Stemedu-ESP32-S3-DevKitC-1-Development-ESP32-S3-WROOM-1-Microcontroller/dp/B09VBFMTDS)
2. [Ferwooh 3× HC-SR04 kit](https://www.amazon.com/Ferwooh-Ultrasonic-Distance-Measuring-Mounting/dp/B0D1MDP9V3)
3. [Electronics Component Fun Kit](https://www.amazon.com/dp/B09FDC6BX6) (breadboard, resistors, wires, caps)
4. [Amazon Basics 5 V 2.4 A adapter](https://www.amazon.com/AmazonBasics-One-Port-USB-Wall-Charger/dp/B0773BHCV1) + USB cable if not included with the ESP32

**Adafruit — buy individually**

1. [ESP32-S3-DevKitC-1 — 8MB Flash 8MB PSRAM](https://www.adafruit.com/product/5312)
2. [HC-SR04 × 3](https://www.adafruit.com/product/3942)
3. [1 kΩ resistors](https://www.adafruit.com/product/4294) + [2.2 kΩ resistors](https://www.adafruit.com/product/2782)
4. [Breadboard + jumper kit](https://www.adafruit.com/product/3314)
5. [USB-A to USB-C cable](https://www.adafruit.com/product/4474) + [5 V 2 A adapter](https://www.adafruit.com/product/1994)
6. [0.1 µF caps (optional)](https://www.adafruit.com/product/753)

---

## Micro Center — Westmont, IL (local pickup)

If you are near the western Chicago suburbs, **Micro Center Westmont** is a good
option for same-day pickup. Browse the [DIY / Maker section](https://www.microcenter.com/search/search_results.aspx?N=4294939904) and set your store to **Westmont** to check stock before you drive over.

### Store info

| | |
|---|---|
| **Store** | [Micro Center Westmont](https://www.microcenter.com/site/stores/westmont.aspx) |
| **Address** | 80 E Ogden Ave, Westmont, IL 60559 |
| **Phone** | (331) 265-7475 |
| **Hours** | Mon–Sat 10:00 AM – 9:00 PM · Sun 11:00 AM – 6:00 PM |

**Tip:** On each product page, select **Westmont** as your store to see in-stock
counts. Some items ship to the store in 5–7 days if not on the shelf.

### Recommended Micro Center parts

| Part | Qty | Product | Notes |
|------|-----|---------|-------|
| ESP32-S3 board | 1 | [Adafruit Metro ESP32-S3 — 16 MB flash, 8 MB PSRAM](https://www.microcenter.com/product/676293/adafruit-industries-metro-esp32-s3-with-16-mb-flash-8-mb-psram) | Best in-store ESP32-S3 match. **GPIO pins differ** from DevKitC-1 — update `config.h` if you use this board. |
| HC-SR04 sensor | 3 | [Leo Sales HC-SR04 Ultrasonic Sensor Module](https://www.microcenter.com/product/613642/leo-sales-ltd-ultrasonic-sensor-module) | One per parking spot. |
| 1 kΩ resistors | 3 | [NTE 1/4 W 1 kΩ — 4 pack](https://www.microcenter.com/product/390158/nte-electronics-1-4-watt-1k-ohm-resistor-4-pack) | Echo divider (high side). |
| 2 kΩ / 2.2 kΩ resistors | 3 | [Inland 1/4 W 1% resistor assortment — 480 pack](https://www.microcenter.com/product/618897/inland-1-4-watt-1-resistors-480-pack) | Includes many values; use **2 kΩ or 2.2 kΩ** for the Echo divider low side. |
| Breadboard + components | 1 | [Inland Pi Kit Deluxe Parts Pack](https://www.microcenter.com/product/603754/inland-pi-kit-deluxe-parts-pack) | Breadboard, jumpers, 1 kΩ resistors, 100 nF caps, and more. |
| Breadboard power (optional) | 1 | [Inland Breadboard 5 V / 3.3 V Power Supply Module](https://www.microcenter.com/product/618900/inland-breadboard-5v-3v-power-supply-module) | USB-powered 5 V rail for sensors on the breadboard. |
| USB wall adapter | 1 | Check in-store phone charger aisle | Micro Center does not always list a dedicated 5 V / 2 A adapter online; any **5 V USB adapter rated ≥ 2 A** works. |

### What Micro Center may not carry

- **ESP32-S3-DevKitC-1** (the exact board in `platformio.ini`) — often **not** on the shelf; the Metro ESP32-S3 above is the usual substitute.
- Full **3-pack HC-SR04** bundles — buy **three** single sensors instead.

### Westmont shopping list (3-spot)

1. [Metro ESP32-S3](https://www.microcenter.com/product/676293/adafruit-industries-metro-esp32-s3-with-16-mb-flash-8-mb-psram) × 1
2. [HC-SR04](https://www.microcenter.com/product/613642/leo-sales-ltd-ultrasonic-sensor-module) × 3
3. [Inland Pi Kit Deluxe](https://www.microcenter.com/product/603754/inland-pi-kit-deluxe-parts-pack) × 1 (breadboard, wires, caps, extra 1 kΩ)
4. [Inland resistor assortment](https://www.microcenter.com/product/618897/inland-1-4-watt-1-resistors-480-pack) × 1 (for 2 kΩ / 2.2 kΩ divider resistors)
5. USB data cable + 5 V / 2 A wall adapter (in-store or from Amazon/Adafruit links above)

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

See **[Recommended products (Amazon & Adafruit)](shopping-list.md#recommended-products-amazon--adafruit)** and **[Micro Center Westmont, IL](shopping-list.md#micro-center--westmont-il-local-pickup)** for specific part links.

Other reputable sources:

- [SparkFun](https://www.sparkfun.com/) — ESP32-S3 boards and jumper kits
- [DigiKey](https://www.digikey.com/) / [Mouser](https://www.mouser.com/) — reliable parts for resistors and caps

**Before checkout:** Confirm the ESP32 board listing says **S3** (not ESP32 classic
or ESP32-C3) and includes enough **PSRAM** for the web dashboard (8 MB recommended).

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
