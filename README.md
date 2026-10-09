# Air Quality Monitor

An ADC monitor with a moving window, variance, warm-up, threshold confirmation, hysteresis, and sensor-failure detection.

## Run

Requirements: ESP32, C++17, and PlatformIO.

```sh
pio run -e esp32dev
pio run -e esp32dev -t upload
python -m pip install -r cloud/requirements.txt
python cloud/serial_bridge.py /dev/ttyUSB0
```

## Behavior

ADC: GPIO 35. Indicator: GPIO 2. Initial warm-up takes 60 seconds. Values are ADC readings, not certified gas concentrations. Pure logic is in `include/air_monitor.hpp`; calibration depends on the chosen sensor.

## Result synchronization

The [operations archive](https://vercel-home-telemetry-api.vercel.app/laboratory.html?project=arduino-air-quality-monitor) stores execution results. Supabase migrations are in the [API repository](https://github.com/brunnojob/vercel-home-telemetry-api/tree/main/supabase/migrations).

```sh
python cloud/sync.py enqueue result.json --project arduino-air-quality-monitor
python cloud/sync.py sync
```

Set `BRUNNODEV_ACCESS_TOKEN` to your session token. The SQLite outbox retains reports until the server confirms persistence; identical content does not create duplicate records. Tokens are not stored in source code. To run the synchronization tests:

```sh
python -m unittest discover -s cloud
```
