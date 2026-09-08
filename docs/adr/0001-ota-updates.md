# ADR 0001: Support LAN-Based OTA Updates

## Status

Accepted

## Context

PerfectPark currently requires a serial USB connection to deploy ESP32-S3
firmware and the LittleFS-hosted dashboard. The installed device is expected to
run on a trusted home Wi-Fi network, where routine physical access may be
inconvenient.

The project needs over-the-air updates from both its PlatformIO development
workflow and its browser dashboard. Both the application image and LittleFS
assets must be updateable. The existing `default.csv` partition layout is
framework-provided, so its support for dual application slots must be verified
rather than assumed.

## Decision

PerfectPark will support two LAN OTA delivery paths:

- ArduinoOTA with PlatformIO's `espota` uploader for developer-driven updates.
- HTTP upload endpoints on the existing asynchronous web server for
  browser-driven updates.

Both paths will accept firmware and LittleFS images. The root Makefile will
provide `make on-esp32-ota`, which uploads LittleFS, waits for the device to
restart, and then uploads firmware to `ESP32_HOST`. The dashboard will provide
separate, explicit controls for each image type.

OTA will not require a password. It is intended only for trusted private
networks. Browser requests with a foreign `Origin` will be rejected as a
limited cross-site request safeguard, but this is not an authentication
boundary.

Before OTA is enabled, the selected partition table will be verified to contain
two sufficiently sized application slots and a LittleFS partition. If the
framework default is unsuitable, the project will commit and select an explicit
OTA-capable partition table.

Serial USB deployment remains the bootstrap and recovery mechanism.

## Consequences

- Routine firmware and dashboard updates will no longer require physical access
  after the first USB installation.
- Developers can use one Make target while browser users can upload generated
  `.bin` artifacts directly.
- Updating both images causes an intermediate reboot, so the automated workflow
  must wait for the device to return before sending firmware.
- Anyone able to reach the OTA services on the local network can replace device
  software. Deployments on untrusted or shared networks will require a future
  authentication decision.
- A failed or incompatible update may still require USB recovery.
- OTA consumes flash space for dual application partitions and may constrain
  future firmware or filesystem growth.
