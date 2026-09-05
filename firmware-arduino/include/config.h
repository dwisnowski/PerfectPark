#pragma once

#include <Arduino.h>

#ifndef SPOT_COUNT
#define SPOT_COUNT 3
#endif

// Distance thresholds (centimeters)
// 7 ft ~= 213 cm detection range, 3 ft ~= 91 cm target park position
constexpr float DETECT_CM = 213.0f;
constexpr float PARKED_MIN_CM = 76.0f;   // 2.5 ft
constexpr float PARKED_MAX_CM = 107.0f;  // 3.5 ft

constexpr uint32_t SENSOR_SAMPLE_INTERVAL_MS = 200;
constexpr uint32_t SENSOR_SAMPLES = 5;
constexpr uint32_t SENSOR_TIMEOUT_US = 30000;
constexpr uint32_t SENSOR_MIN_INTERVAL_MS = 60;

constexpr char WIFI_PORTAL_NAME[] = "PerfectPark-Setup";

struct SpotPinConfig {
  uint8_t id;
  const char *label;
  uint8_t trigPin;
  uint8_t echoPin;
};

#if SPOT_COUNT == 2
constexpr SpotPinConfig SPOT_PINS[SPOT_COUNT] = {
    {1, "Spot 1", 4, 5},
    {2, "Spot 2", 6, 7},
};
#elif SPOT_COUNT == 3
constexpr SpotPinConfig SPOT_PINS[SPOT_COUNT] = {
    {1, "Spot 1", 4, 5},
    {2, "Spot 2", 6, 7},
    {3, "Spot 3", 15, 16},
};
#else
#error "SPOT_COUNT must be 2 or 3 for the default pin map"
#endif

enum class SpotState : uint8_t {
  Empty = 0,
  Approaching,
  Parked,
  TooClose,
  Error,
};

inline const char *spotStateToString(SpotState state) {
  switch (state) {
    case SpotState::Empty:
      return "empty";
    case SpotState::Approaching:
      return "approaching";
    case SpotState::Parked:
      return "parked";
    case SpotState::TooClose:
      return "too_close";
    default:
      return "error";
  }
}

inline float cmToFeet(float cm) {
  return cm / 30.48f;
}
