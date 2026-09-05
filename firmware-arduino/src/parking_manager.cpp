#include "parking_manager.h"

SpotState ParkingManager::classifyDistance(float distanceCm, bool valid) {
  if (!valid) {
    return SpotState::Error;
  }
  if (distanceCm > DETECT_CM) {
    return SpotState::Empty;
  }
  if (distanceCm < PARKED_MIN_CM) {
    return SpotState::TooClose;
  }
  if (distanceCm <= PARKED_MAX_CM) {
    return SpotState::Parked;
  }
  return SpotState::Approaching;
}

void ParkingManager::begin() {
  for (auto &sensor : sensors_) {
    sensor.begin();
  }

  for (uint8_t i = 0; i < SPOT_COUNT; ++i) {
    readings_[i].id = SPOT_PINS[i].id;
    readings_[i].label = SPOT_PINS[i].label;
    readings_[i].state = SpotState::Empty;
    readings_[i].occupied = false;
    readings_[i].valid = false;
    readings_[i].distanceCm = -1.0f;
    readings_[i].distanceFt = -1.0f;
    readings_[i].updatedAtMs = millis();
  }
}

void ParkingManager::update() {
  const uint32_t nowMs = millis();
  if (nowMs - lastSampleMs_ < SENSOR_SAMPLE_INTERVAL_MS) {
    return;
  }
  lastSampleMs_ = nowMs;

  for (uint8_t i = 0; i < SPOT_COUNT; ++i) {
    const float distanceCm = sensors_[i].readDistanceCm(SENSOR_SAMPLES);
    const bool valid = distanceCm > 0.0f;
    const SpotState state = classifyDistance(distanceCm, valid);

    readings_[i].valid = valid;
    readings_[i].distanceCm = valid ? distanceCm : -1.0f;
    readings_[i].distanceFt = valid ? cmToFeet(distanceCm) : -1.0f;
    readings_[i].state = state;
    readings_[i].occupied =
        valid && (state == SpotState::Approaching || state == SpotState::Parked ||
                  state == SpotState::TooClose);
    readings_[i].updatedAtMs = nowMs;
  }
}
