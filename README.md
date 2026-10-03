# arduino-air-quality-monitor

ESP32 indoor air monitor for an analog gas sensor. Reports raw and moving-average ADC readings over serial and lights an alert LED above a configurable threshold. Calibrate for the sensor and environment; this is not a certified safety alarm.

Requires PlatformIO Core. Run `pio run` to build and `pio run -t upload` to flash the ESP32 DevKit. Sensor: GPIO35; LED: GPIO2; serial: 115200 baud.

Project by [Brunno Dev](https://brunnodev.store).