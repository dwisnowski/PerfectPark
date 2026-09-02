# PerfectPark ESP-IDF Port (Planned)

This directory is reserved for a future ESP-IDF implementation that mirrors the
Arduino firmware in `../firmware-arduino/`.

## Shared assets

- Web UI source: `../web/`
- JSON API contract: `GET /api/status`
- Distance thresholds and spot states: see `../firmware-arduino/include/config.h`

## Planned layout

```
firmware-esp-idf/
├── CMakeLists.txt
├── main/
│   ├── CMakeLists.txt
│   ├── main.c
│   ├── parking_manager.c
│   ├── ultrasonic.c
│   └── web_server.c
└── spiffs/ or littlefs image built from ../web/
```

## Status

Version 1 ships with the PlatformIO/Arduino firmware. ESP-IDF support will reuse
the same web assets and API shape once the Arduino build is validated on hardware.
