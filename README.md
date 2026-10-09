# Air Quality Monitor

Monitor ADC com janela móvel, variância, aquecimento, confirmação de limiar, histerese e identificação de falha de sensor.

## Executar

Requisitos: ESP32, C++17 e PlatformIO.

```sh
pio run -e esp32dev
pio run -e esp32dev -t upload
python -m pip install -r cloud/requirements.txt
python cloud/serial_bridge.py /dev/ttyUSB0
```

## Funcionamento

ADC: GPIO 35. Indicador: GPIO 2. Aquecimento inicial de 60 segundos. Valores são leituras ADC e não concentrações certificadas de gases. A lógica pura está em `include/air_monitor.hpp`; calibração depende do sensor utilizado.

## Persistência de resultados

O arquivo de operações está em [vercel-home-telemetry-api.vercel.app](https://vercel-home-telemetry-api.vercel.app/laboratory.html?project=arduino-air-quality-monitor). As migrações Supabase estão no [repositório da API](https://github.com/brunnojob/vercel-home-telemetry-api/tree/main/supabase/migrations).

```sh
python cloud/sync.py enqueue resultado.json --project arduino-air-quality-monitor
python cloud/sync.py sync
```

Defina `BRUNNODEV_ACCESS_TOKEN` com sua sessão. A fila SQLite conserva os relatórios até confirmação do servidor; o mesmo conteúdo não gera registros duplicados. Tokens não são gravados no código.
