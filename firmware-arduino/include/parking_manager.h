#pragma once

#include <Arduino.h>
#include <array>

#include "config.h"
#include "ultrasonic.h"

struct SpotReading {
  uint8_t id;
  const char *label;
  SpotState state;
  bool occupied;
  bool valid;
  float distanceCm;
  float distanceFt;
  uint32_t updatedAtMs;
};

class ParkingManager {
 public:
  void begin();
  void update();

  uint8_t spotCount() const { return SPOT_COUNT; }
  const SpotReading &reading(uint8_t index) const { return readings_[index]; }

 private:
  std::array<UltrasonicSensor, SPOT_COUNT> sensors_{
      UltrasonicSensor(SPOT_PINS[0].trigPin, SPOT_PINS[0].echoPin),
#if SPOT_COUNT > 1
      UltrasonicSensor(SPOT_PINS[1].trigPin, SPOT_PINS[1].echoPin),
#endif
#if SPOT_COUNT > 2
      UltrasonicSensor(SPOT_PINS[2].trigPin, SPOT_PINS[2].echoPin),
#endif
  };

  std::array<SpotReading, SPOT_COUNT> readings_{};
  uint32_t lastSampleMs_ = 0;

  static SpotState classifyDistance(float distanceCm, bool valid);
};
