#pragma once

#include <Arduino.h>

class UltrasonicSensor {
 public:
  UltrasonicSensor(uint8_t trigPin, uint8_t echoPin);

  void begin();
  float readDistanceCm(uint8_t samples = 5);

 private:
  uint8_t trigPin_;
  uint8_t echoPin_;
  uint32_t lastReadMs_ = 0;

  float readSingleCm();
  static float median(float *values, uint8_t count);
};
