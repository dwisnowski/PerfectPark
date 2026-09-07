#!/usr/bin/env bash
# Ensure Python 3.10–3.13 (installs 3.12 if needed) and PlatformIO on macOS.
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=lib/python.sh
source "${SCRIPT_DIR}/lib/python.sh"

install_python_via_brew() {
  if ! command -v brew >/dev/null 2>&1; then
    echo "error: Homebrew not found, and no compatible Python is installed."
    echo "Install Homebrew from https://brew.sh then re-run:"
    echo "  make on-mac-install"
    echo "Or install Python ${PYTHON_RECOMMENDED} manually and re-run this target."
    exit 1
  fi

  echo "No compatible Python found (need ${PYTHON_MIN_MAJOR}.${PYTHON_MIN_MINOR}–${PYTHON_MIN_MAJOR}.${PYTHON_MAX_MINOR})."
  echo "Installing ${PYTHON_BREW_FORMULA} via Homebrew..."
  HOMEBREW_NO_AUTO_UPDATE=1 brew install "${PYTHON_BREW_FORMULA}"

  local brew_prefix
  brew_prefix="$(brew --prefix "${PYTHON_BREW_FORMULA}")"
  export PATH="${brew_prefix}/bin:${PATH}"
  hash -r 2>/dev/null || true
}

echo "==> PerfectPark macOS dependency setup"
echo "    Required Python: ${PYTHON_MIN_MAJOR}.${PYTHON_MIN_MINOR}–${PYTHON_MIN_MAJOR}.${PYTHON_MAX_MINOR} (recommended ${PYTHON_RECOMMENDED})"

if find_compatible_python; then
  echo "Found compatible Python: $(python_print_version) (${PYTHON_BIN} ${PYTHON_ARGS[*]})"
else
  install_python_via_brew
  if ! find_compatible_python; then
    echo "error: Python was installed but is not on PATH yet."
    echo "Try:  brew link ${PYTHON_BREW_FORMULA}"
    echo "Then re-run:  make on-mac-install"
    exit 1
  fi
  echo "Using Python: $(python_print_version) (${PYTHON_BIN} ${PYTHON_ARGS[*]})"
fi

if command -v pio >/dev/null 2>&1; then
  echo "PlatformIO already installed: $(pio --version 2>/dev/null | head -n 1)"
elif command -v brew >/dev/null 2>&1; then
  echo "Installing PlatformIO via Homebrew..."
  if HOMEBREW_NO_AUTO_UPDATE=1 brew install platformio && command -v pio >/dev/null 2>&1; then
    echo "PlatformIO ready: $(pio --version 2>/dev/null | head -n 1)"
  else
    echo "Homebrew PlatformIO install failed or pio not on PATH; falling back to pip..."
    ensure_platformio
  fi
else
  ensure_platformio
fi

echo ""
echo "macOS setup complete."
echo "Next: connect the ESP32, then run  make on-mac-deploy"
