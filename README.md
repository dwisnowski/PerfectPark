# PerfectPark

ESP32-S3 garage parking monitor with multiple HC-SR04 ultrasonic sensors, a
self-hosted web dashboard, and a JSON status API.

## Features

- Multiple parking spots (default: 3, configurable)
- Vehicle detection up to ~7 ft (~213 cm)
- Target parked distance ~3 ft (~91 cm)
- Joins home WiFi with a captive-portal setup flow
- Serves a mobile-friendly dashboard from the ESP32-S3
- Exposes live status at `GET /api/status`
- Updates firmware and dashboard assets over local WiFi

## Hardware

See the full **[wiring diagram](docs/wiring.md)** for pin connections, voltage
dividers, breadboard layout, and mounting guidance.

See the **[parts shopping list](docs/shopping-list.md)** for quantities, buying
notes, estimated cost, and specific links for **Amazon**, **Adafruit**, and
**Micro Center (Westmont, IL)**.

| Part | Qty | Notes |
|------|-----|-------|
| ESP32-S3 DevKitC-1 | 1 | N16R8 with PSRAM recommended |
| HC-SR04 ultrasonic sensor | 1 per spot | 5 V module |
| 1 kΩ + 2 kΩ resistors | 1 set per spot | Voltage divider on each Echo pin |
| 5 V USB power | 1 | Stable supply for ESP + sensors |

### Default GPIO map

| Spot | Trigger | Echo |
|------|---------|------|
| 1 | GPIO 4 | GPIO 5 |
| 2 | GPIO 6 | GPIO 7 |
| 3 | GPIO 15 | GPIO 16 |

**Important:** HC-SR04 Echo is 5 V. Use a divider before connecting Echo to ESP32 GPIO.

## Distance logic

| Distance | State | Meaning |
|----------|-------|---------|
| > 7 ft | `empty` | No vehicle in range |
| 3.5–7 ft | `approaching` | Vehicle detected |
| 2.5–3.5 ft | `parked` | In target zone |
| < 2.5 ft | `too_close` | Back up slightly |

## Project layout

```
PerfectPark/
├── docs/
│   ├── wiring.md         # hardware wiring diagram
│   └── shopping-list.md  # parts list and buying guide
├── firmware-arduino/   # v1 firmware (PlatformIO + Arduino)
├── firmware-esp-idf/   # future ESP-IDF port
├── web/                # dashboard source files
└── scripts/sync-web.sh # copy web/ -> firmware-arduino/data/
```

## Firmware setup

1. Install [PlatformIO](https://platformio.org/)
2. Sync the web assets:

```bash
./scripts/sync-web.sh
```

3. Build and upload from `firmware-arduino/`:

```bash
cd firmware-arduino
pio run -t uploadfs
pio run -t upload
pio device monitor
```

4. On first boot, connect to the `PerfectPark-Setup` WiFi portal and enter your home network credentials.
5. Open the Serial Monitor for the device IP, then visit `http://<device-ip>/`.

## OTA updates

OTA is available after the first USB deployment. The default hostname is
`perfectpark.local`; use the device IP if mDNS is unavailable.

To update both the LittleFS dashboard and firmware from PlatformIO:

```bash
make on-esp32-ota ESP32_HOST=perfectpark.local
```

For the 2-spot environment:

```bash
make on-esp32-ota ESP32_ENV=esp32-s3-devkitc-1-2spots
```

The target uploads LittleFS first, waits for the ESP32 to restart, and then
uploads firmware. Override `OTA_REBOOT_WAIT` if the device takes longer than
eight seconds to reconnect.

You can also upload images from the **OTA Updates** panel in the dashboard.
Build the selected environment and its filesystem image first:

```bash
cd firmware-arduino
pio run -e esp32-s3-devkitc-1
pio run -e esp32-s3-devkitc-1 -t buildfs
```

Then select:

- `.pio/build/esp32-s3-devkitc-1/firmware.bin` for firmware
- `.pio/build/esp32-s3-devkitc-1/littlefs.bin` for dashboard assets

OTA has no password and is intended only for a trusted private LAN. Anyone who
can reach the device can replace its software. If an update fails or the device
does not boot, reconnect it over USB and run the normal deploy target:

```bash
make on-esp32-deploy UPLOAD_PORT=/dev/cu.usbmodem...
```

## API example

`GET /api/status`

```json
{
  "device": "PerfectPark",
  "version": "1.0.0",
  "spot_count": 3,
  "detect_ft": 7.0,
  "park_target_ft": 3.0,
  "spots": [
    {
      "id": 1,
      "label": "Spot 1",
      "state": "parked",
      "occupied": true,
      "valid": true,
      "distance_cm": 94.2,
      "distance_ft": 3.1,
      "updated_at": 123456
    }
  ]
}
```

## Configuration

Edit `firmware-arduino/include/config.h` or use PlatformIO environments:

- Default 3 spots: `esp32-s3-devkitc-1`
- 2 spots: `esp32-s3-devkitc-1-2spots`

## Development notes

- Edit UI files in `web/`, then run `./scripts/sync-web.sh` before `uploadfs`.
- Arduino firmware is the v1 target. ESP-IDF will follow the same API and UI.
