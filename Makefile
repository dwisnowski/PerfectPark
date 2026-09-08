# PerfectPark — host and device make targets
#
# Usage:
#   make help
#   make on-mac-install
#   make on-esp32-deploy

.DEFAULT_GOAL := help

ROOT        := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
FIRMWARE    := $(ROOT)/firmware-arduino
WEB         := $(ROOT)/web
DATA        := $(FIRMWARE)/data
PIO         ?= pio
ESP32_ENV   ?= esp32-s3-devkitc-1
ESP32_HOST  ?= perfectpark.local
ESP32_USER  ?= root
UPLOAD_PORT ?=
OTA_ENV     ?= $(ESP32_ENV)-ota
OTA_REBOOT_WAIT ?= 8

.PHONY: help sync-web \
	on-windows on-windows-install on-windows-build on-windows-deploy on-windows-ssh \
	on-mac on-mac-install on-mac-build on-mac-deploy on-mac-ssh \
	on-esp32 on-esp32-install on-esp32-build on-esp32-deploy on-esp32-ota on-esp32-ssh

help:
	@echo "PerfectPark Make targets"
	@echo ""
	@echo "  help                 Show this help"
	@echo ""
	@echo "on-windows (develop from Windows)"
	@echo "  on-windows-install   Install PlatformIO Core (pip)"
	@echo "  on-windows-build     Sync web assets and build firmware"
	@echo "  on-windows-deploy    Flash LittleFS + firmware to the ESP32"
	@echo "  on-windows-ssh       SSH into the ESP32 (ESP32_HOST / ESP32_USER)"
	@echo ""
	@echo "on-mac (develop from macOS)"
	@echo "  on-mac-install       Install PlatformIO Core (Homebrew, pip fallback)"
	@echo "  on-mac-build         Sync web assets and build firmware"
	@echo "  on-mac-deploy        Flash LittleFS + firmware to the ESP32"
	@echo "  on-mac-ssh           SSH into the ESP32 (ESP32_HOST / ESP32_USER)"
	@echo ""
	@echo "on-esp32 (device-focused)"
	@echo "  on-esp32-install     Install Espressif32 platform / board packages"
	@echo "  on-esp32-build       Sync web assets and build firmware"
	@echo "  on-esp32-deploy      Flash LittleFS + firmware to the ESP32"
	@echo "  on-esp32-ota         Upload LittleFS + firmware over WiFi"
	@echo "  on-esp32-ssh         SSH into the ESP32 (ESP32_HOST / ESP32_USER)"
	@echo ""
	@echo "Variables (override on the command line):"
	@echo "  PIO=$(PIO)"
	@echo "  ESP32_ENV=$(ESP32_ENV)"
	@echo "  OTA_ENV=$(OTA_ENV)"
	@echo "  ESP32_HOST=$(ESP32_HOST)"
	@echo "  ESP32_USER=$(ESP32_USER)"
	@echo "  UPLOAD_PORT=$(UPLOAD_PORT)   e.g. COM3 or /dev/cu.usbmodem*"
	@echo "  OTA_REBOOT_WAIT=$(OTA_REBOOT_WAIT)   seconds between OTA uploads"

# ---------------------------------------------------------------------------
# Shared helpers
# ---------------------------------------------------------------------------

sync-web:
	@mkdir -p "$(DATA)"
	@cp "$(WEB)/index.html" "$(WEB)/style.css" "$(WEB)/app.js" "$(DATA)/"
	@echo "Synced web assets to $(DATA)"

define pio-build
	$(MAKE) sync-web
	cd "$(FIRMWARE)" && $(PIO) run -e $(ESP32_ENV)
endef

define pio-deploy
	$(MAKE) sync-web
	cd "$(FIRMWARE)" && $(PIO) run -e $(ESP32_ENV) -t uploadfs $(if $(UPLOAD_PORT),--upload-port $(UPLOAD_PORT),)
	cd "$(FIRMWARE)" && $(PIO) run -e $(ESP32_ENV) -t upload $(if $(UPLOAD_PORT),--upload-port $(UPLOAD_PORT),)
endef

define pio-ota
	$(MAKE) sync-web
	cd "$(FIRMWARE)" && $(PIO) run -e $(OTA_ENV) -t uploadfs --upload-port "$(ESP32_HOST)"
	@echo "Waiting $(OTA_REBOOT_WAIT)s for $(ESP32_HOST) to restart..."
	@sleep "$(OTA_REBOOT_WAIT)"
	cd "$(FIRMWARE)" && $(PIO) run -e $(OTA_ENV) -t upload --upload-port "$(ESP32_HOST)"
endef

define esp32-ssh
	ssh "$(ESP32_USER)@$(ESP32_HOST)"
endef

# ===========================================================================
# on-windows
# ===========================================================================

on-windows:
	@$(MAKE) help | sed -n '/^on-windows/,/^on-mac/p' | sed '$$d'

on-windows-install:
	python -m pip install -U platformio

on-windows-build:
	$(pio-build)

on-windows-deploy:
	$(pio-deploy)

on-windows-ssh:
	$(esp32-ssh)

# ===========================================================================
# on-mac
# ===========================================================================

on-mac:
	@$(MAKE) help | sed -n '/^on-mac/,/^on-esp32/p' | sed '$$d'

on-mac-install:
	@if command -v brew >/dev/null 2>&1; then \
		brew install platformio; \
	else \
		python3 -m pip install -U platformio; \
	fi

on-mac-build:
	$(pio-build)

on-mac-deploy:
	$(pio-deploy)

on-mac-ssh:
	$(esp32-ssh)

# ===========================================================================
# on-esp32
# ===========================================================================

on-esp32:
	@$(MAKE) help | sed -n '/^on-esp32/,/^Variables/p' | sed '$$d'

on-esp32-install:
	$(PIO) platform install espressif32
	cd "$(FIRMWARE)" && $(PIO) pkg install

on-esp32-build:
	$(pio-build)

on-esp32-deploy:
	$(pio-deploy)

on-esp32-ota:
	$(pio-ota)

on-esp32-ssh:
	$(esp32-ssh)
