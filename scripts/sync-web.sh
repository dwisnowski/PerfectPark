#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SRC_DIR="${ROOT_DIR}/web"
DEST_DIR="${ROOT_DIR}/firmware-arduino/data"

mkdir -p "${DEST_DIR}"
cp "${SRC_DIR}/index.html" "${SRC_DIR}/style.css" "${SRC_DIR}/app.js" "${DEST_DIR}/"
echo "Synced web assets to ${DEST_DIR}"
