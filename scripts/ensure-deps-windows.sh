#!/usr/bin/env bash
# Ensure Python 3.10–3.13 (installs 3.12 if needed) and PlatformIO on Windows.
# Intended for Git Bash / MSYS make. Uses winget for Python.
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=lib/python.sh
source "${SCRIPT_DIR}/lib/python.sh"

add_common_python_paths() {
  local base bases=()
  if [[ -n "${LOCALAPPDATA:-}" ]]; then
    bases+=("${LOCALAPPDATA}/Programs/Python/Python312")
    bases+=("${LOCALAPPDATA}/Programs/Python/Python313")
    bases+=("${LOCALAPPDATA}/Programs/Python/Python311")
    bases+=("${LOCALAPPDATA}/Programs/Python/Python310")
  fi
  if [[ -n "${HOME:-}" ]]; then
    bases+=("${HOME}/AppData/Local/Programs/Python/Python312")
  fi
  if [[ -n "${USERNAME:-}" ]]; then
    bases+=("/c/Users/${USERNAME}/AppData/Local/Programs/Python/Python312")
  fi

  for base in "${bases[@]}"; do
    if [[ -x "${base}/python.exe" ]]; then
      export PATH="${base}:${base}/Scripts:${PATH}"
    fi
  done
}

install_python_via_winget() {
  if ! command -v winget >/dev/null 2>&1; then
    echo "error: winget not found."
    echo "Install 'App Installer' from the Microsoft Store, then re-run:"
    echo "  make on-windows-install"
    exit 1
  fi

  echo "No compatible Python found (need ${PYTHON_MIN_MAJOR}.${PYTHON_MIN_MINOR}–${PYTHON_MIN_MAJOR}.${PYTHON_MAX_MINOR})."
  echo "Installing Python ${PYTHON_RECOMMENDED} via winget (${PYTHON_WINGET_ID})..."
  winget install \
    -e \
    --id "${PYTHON_WINGET_ID}" \
    --accept-package-agreements \
    --accept-source-agreements \
    --scope user

  add_common_python_paths
  hash -r 2>/dev/null || true
}

echo "==> PerfectPark Windows dependency setup"
echo "    Required Python: ${PYTHON_MIN_MAJOR}.${PYTHON_MIN_MINOR}–${PYTHON_MIN_MAJOR}.${PYTHON_MAX_MINOR} (recommended ${PYTHON_RECOMMENDED})"

add_common_python_paths

if find_compatible_python; then
  echo "Found compatible Python: $(python_print_version) (${PYTHON_BIN} ${PYTHON_ARGS[*]})"
else
  install_python_via_winget
  if ! find_compatible_python; then
    echo "error: Python ${PYTHON_RECOMMENDED} was installed, but this shell cannot see it yet."
    echo "Close this terminal, open a new Git Bash window, and run:"
    echo "  make on-windows-install"
    exit 1
  fi
  echo "Using Python: $(python_print_version) (${PYTHON_BIN} ${PYTHON_ARGS[*]})"
fi

ensure_platformio

echo ""
echo "Windows setup complete."
echo "Next: connect the ESP32, then run  make on-windows-deploy"
echo "If the board is not detected, install the USB serial driver from the board vendor."
