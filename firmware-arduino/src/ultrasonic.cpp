#include "ultrasonic.h"

#include "config.h"

UltrasonicSensor::UltrasonicSensor(uint8_t trigPin, uint8_t echoPin)
    : trigPin_(trigPin), echoPin_(echoPin) {}

void UltrasonicSensor::begin() {
  pinMode(trigPin_, OUTPUT);
  pinMode(echoPin_, INPUT);
  digitalWrite(trigPin_, LOW);
}

float UltrasonicSensor::median(float *values, uint8_t count) {
  for (uint8_t i = 1; i < count; ++i) {
    float key = values[i];
    int8_t j = static_cast<int8_t>(i) - 1;
    while (j >= 0 && values[j] > key) {
      values[j + 1] = values[j];
      --j;
    }
    values[j + 1] = key;
  }

  if (count % 2 == 0) {
    return (values[count / 2 - 1] + values[count / 2]) / 2.0f;
  }
  return values[count / 2];
}

float UltrasonicSensor::readSingleCm() {
  const uint32_t nowMs = millis();
  if (nowMs - lastReadMs_ < SENSOR_MIN_INTERVAL_MS) {
    delay(SENSOR_MIN_INTERVAL_MS - (nowMs - lastReadMs_));
  }
  lastReadMs_ = millis();

  digitalWrite(trigPin_, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_, LOW);

  const uint32_t startWait = micros();
  while (digitalRead(echoPin_) == LOW) {
    if (micros() - startWait > SENSOR_TIMEOUT_US) {
      return -1.0f;
    }
  }

  const uint32_t pulseStart = micros();
  while (digitalRead(echoPin_) == HIGH) {
    if (micros() - pulseStart > SENSOR_TIMEOUT_US) {
      return -1.0f;
    }
  }

  const uint32_t pulseEnd = micros();
  const uint32_t durationUs = pulseEnd - pulseStart;

  // HC-SR04 timing: distance (cm) ~= duration (us) / 58
  const float distanceCm = static_cast<float>(durationUs) / 58.0f;
  if (distanceCm < 2.0f || distanceCm > 450.0f) {
    return -1.0f;
  }

  return distanceCm;
}

float UltrasonicSensor::readDistanceCm(uint8_t samples) {
  float values[SENSOR_SAMPLES];
  uint8_t validCount = 0;

  for (uint8_t i = 0; i < samples && i < SENSOR_SAMPLES; ++i) {
    const float reading = readSingleCm();
    if (reading > 0.0f) {
      values[validCount++] = reading;
    }
    delay(10);
  }

  if (validCount == 0) {
    return -1.0f;
  }

  return median(values, validCount);
}
