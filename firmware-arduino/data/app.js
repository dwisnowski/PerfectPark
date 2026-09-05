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

refreshStatus();
setInterval(refreshStatus, REFRESH_MS);
