#include "air_monitor.hpp"
#include <Arduino.h>
constexpr int sensorPin = 35, ledPin = 2;
AirMonitor monitor;
std::uint32_t lastSample = 0, sequence = 0;
void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  analogReadResolution(12);
  Serial.begin(115200);
}
void loop() {
  std::uint32_t now = millis();
  if (std::uint32_t(now - lastSample) < 1000)
    return;
  lastSample = now;
  auto result = monitor.sample(analogRead(sensorPin), now);
  digitalWrite(ledPin, result.state == AirState::Alert ||
                               result.state == AirState::Fault
                           ? HIGH
                           : LOW);
  Serial.printf("{\"deviceId\":\"air-01\",\"sequence\":%lu,\"timestampMs\":%lu,"
                "\"adc\":%d,\"average\":%.2f,\"variance\":%.2f,\"state\":%d}\n",
                static_cast<unsigned long>(++sequence),
                static_cast<unsigned long>(now), result.raw, result.average,
                result.variance, int(result.state));
}
