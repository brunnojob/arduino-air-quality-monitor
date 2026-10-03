# arduino-air-quality-monitor

ESP32 indoor air monitor for an analog gas sensor. Publishes raw and moving-average ADC values over serial and lights an alert LED above a configurable threshold. ADC values require calibration for the sensor and environment; this is not a certified safety alarm.

Configure GPIO35 for analog input and GPIO2 for the LED, then upload `src/main.cpp` using Arduino IDE.

Project by [Brunno Dev](https://brunnodev.store).