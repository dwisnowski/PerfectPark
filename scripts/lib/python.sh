# Shared Python version helpers for PerfectPark install scripts.
# Compatible range: PlatformIO Core (3.9+) with ESP32 tooling (prefer 3.10–3.13).
# shellcheck shell=bash

PYTHON_MIN_MAJOR=3
PYTHON_MIN_MINOR=10
PYTHON_MAX_MINOR=13
PYTHON_RECOMMENDED=3.12
PYTHON_WINGET_ID=Python.Python.3.12
PYTHON_BREW_FORMULA=python@3.12

# PYTHON_BIN / PYTHON_ARGS describe how to invoke a compatible interpreter.
PYTHON_BIN=""
PYTHON_ARGS=()

run_python() {
  "${PYTHON_BIN}" "${PYTHON_ARGS[@]}" "$@"
}

python_version_ok() {
  local bin=$1
  shift
  "${bin}" "$@" -c "import sys; v=sys.version_info; raise SystemExit(0 if v.major == ${PYTHON_MIN_MAJOR} and ${PYTHON_MIN_MINOR} <= v.minor <= ${PYTHON_MAX_MINOR} else 1)" >/dev/null 2>&1
}

python_print_version() {
  run_python -c "import sys; print(f'{sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro}')"
}

find_compatible_python() {
  local candidate ver
  PYTHON_BIN=""
  PYTHON_ARGS=()

  for candidate in \
    python3.12 python3.13 python3.11 python3.10 \
    python3 python \
    python; do
    if command -v "${candidate}" >/dev/null 2>&1 && python_version_ok "${candidate}"; then
      PYTHON_BIN="${candidate}"
      return 0
    fi
  done

  if command -v py >/dev/null 2>&1; then
    for ver in 3.12 3.13 3.11 3.10; do
      if python_version_ok py "-${ver}"; then
        PYTHON_BIN=py
        PYTHON_ARGS=("-${ver}")
        return 0
      fi
    done
  fi

  return 1
}

ensure_platformio() {
  if command -v pio >/dev/null 2>&1; then
    echo "PlatformIO already installed: $(pio --version 2>/dev/null | head -n 1)"
    return 0
  fi

  echo "Installing PlatformIO Core with $(run_python --version 2>&1)..."
  run_python -m pip install --upgrade pip
  run_python -m pip install -U platformio

  if ! command -v pio >/dev/null 2>&1; then
    echo "PlatformIO installed, but 'pio' is not on PATH yet."
    echo "Close and reopen this terminal, then re-run the install target."
    echo "On Windows, also ensure Python Scripts is on PATH (winget Python usually offers this)."
    return 1
  fi

  echo "PlatformIO ready: $(pio --version 2>/dev/null | head -n 1)"
}
