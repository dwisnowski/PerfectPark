const REFRESH_MS = 2000;

const stateLabels = {
  empty: "Empty",
  approaching: "Approaching",
  parked: "Parked",
  too_close: "Too Close",
  error: "Sensor Error",
};

const stateDescriptions = {
  empty: "No vehicle detected within range.",
  approaching: "Vehicle detected. Guide into the bay.",
  parked: "Vehicle is in the target parking zone.",
  too_close: "Vehicle is too close. Back up slightly.",
  error: "Sensor reading unavailable.",
};

const spotsGrid = document.getElementById("spots-grid");
const connectionStatus = document.getElementById("connection-status");
const lastUpdated = document.getElementById("last-updated");
const occupiedCount = document.getElementById("occupied-count");
const availableCount = document.getElementById("available-count");
const detectRange = document.getElementById("detect-range");
const updateForms = Array.from(document.querySelectorAll(".update-form"));
const updateStatus = document.getElementById("update-status");
const updateProgress = document.querySelector(".update-progress");
const updateProgressBar = document.getElementById("update-progress-bar");
let refreshTimer;

function formatDistance(spot) {
  if (!spot.valid || spot.distance_ft == null) {
    return "--";
  }
  return `${Number(spot.distance_ft).toFixed(1)} ft`;
}

function renderSpots(spots) {
  spotsGrid.innerHTML = spots
    .map((spot) => {
      const state = spot.state || "error";
      return `
        <article class="spot-card" data-state="${state}">
          <div class="spot-top">
            <h2 class="spot-title">${spot.label || `Spot ${spot.id}`}</h2>
            <span class="state-pill" data-state="${state}">${stateLabels[state] || state}</span>
          </div>
          <div class="distance">${formatDistance(spot)} <small>from sensor</small></div>
          <p class="spot-detail">${stateDescriptions[state] || ""}</p>
        </article>
      `;
    })
    .join("");
}

function updateSummary(data) {
  const spots = data.spots || [];
  const occupied = spots.filter((spot) => spot.occupied).length;
  const available = Math.max(spots.length - occupied, 0);

  occupiedCount.textContent = occupied;
  availableCount.textContent = available;
  detectRange.textContent = data.detect_ft ? `${Number(data.detect_ft).toFixed(1)} ft` : "7 ft";
}

async function refreshStatus() {
  try {
    const response = await fetch("/api/status", { cache: "no-store" });
    if (!response.ok) {
      throw new Error(`HTTP ${response.status}`);
    }

    const data = await response.json();
    renderSpots(data.spots || []);
    updateSummary(data);

    connectionStatus.textContent = "Live";
    connectionStatus.className = "badge badge-live";
    lastUpdated.textContent = `Updated ${new Date().toLocaleTimeString()}`;
  } catch (error) {
    connectionStatus.textContent = "Offline";
    connectionStatus.className = "badge badge-error";
    lastUpdated.textContent = "Unable to reach device";
    console.error(error);
  }
}

function setUpdateControlsDisabled(disabled) {
  updateForms.forEach((form) => {
    Array.from(form.elements).forEach((control) => {
      control.disabled = disabled;
    });
  });
}

function waitForDevice() {
  updateStatus.textContent = "Waiting for device to restart…";
  let attempts = 0;

  const reconnectTimer = setInterval(async () => {
    attempts += 1;
    try {
      const response = await fetch(`/api/status?ota=${Date.now()}`, { cache: "no-store" });
      if (response.ok) {
        clearInterval(reconnectTimer);
        window.location.reload();
      }
    } catch (error) {
      // A connection failure is expected while the ESP32 reboots.
    }

    if (attempts >= 45) {
      clearInterval(reconnectTimer);
      updateStatus.textContent = "Restart timed out; reload this page when the device is online";
      setUpdateControlsDisabled(false);
    }
  }, 2000);
}

function uploadImage(kind, file) {
  const endpoint = `/api/update/${kind}`;
  const formData = new FormData();
  formData.append("update", file, file.name);

  setUpdateControlsDisabled(true);
  clearInterval(refreshTimer);
  updateStatus.textContent = `Uploading ${kind}…`;
  updateProgress.classList.add("is-active");
  updateProgress.setAttribute("aria-hidden", "false");
  updateProgressBar.style.width = "0%";

  const request = new XMLHttpRequest();
  request.open("POST", endpoint);
  request.upload.addEventListener("progress", (event) => {
    if (event.lengthComputable) {
      const percent = Math.round((event.loaded / event.total) * 100);
      updateProgressBar.style.width = `${percent}%`;
      updateStatus.textContent = `Uploading ${kind}: ${percent}%`;
    }
  });
  request.addEventListener("load", () => {
    let result = {};
    try {
      result = JSON.parse(request.responseText);
    } catch (error) {
      result.error = request.responseText || `HTTP ${request.status}`;
    }

    if (request.status >= 200 && request.status < 300 && result.ok) {
      updateProgressBar.style.width = "100%";
      waitForDevice();
      return;
    }

    updateStatus.textContent = `Update failed: ${result.error || `HTTP ${request.status}`}`;
    setUpdateControlsDisabled(false);
    refreshTimer = setInterval(refreshStatus, REFRESH_MS);
  });
  request.addEventListener("error", () => {
    updateStatus.textContent = "Upload connection failed";
    setUpdateControlsDisabled(false);
    refreshTimer = setInterval(refreshStatus, REFRESH_MS);
  });
  request.send(formData);
}

updateForms.forEach((form) => {
  form.addEventListener("submit", (event) => {
    event.preventDefault();
    const kind = form.dataset.updateKind;
    const file = form.querySelector('input[type="file"]').files[0];
    const expectedFilename = kind === "filesystem" ? "littlefs.bin" : "firmware.bin";

    if (!file || file.name.toLowerCase() !== expectedFilename) {
      updateStatus.textContent = `Select ${expectedFilename} for this update`;
      return;
    }

    if (!window.confirm(`Upload ${file.name} as the ${kind} image? The device will reboot.`)) {
      return;
    }

    uploadImage(kind, file);
  });
});

refreshStatus();
refreshTimer = setInterval(refreshStatus, REFRESH_MS);
