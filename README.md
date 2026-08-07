# HydroDoc Water Bottle Monitor

HydroDoc is an ESP32-powered smart hydration dock that tracks water intake using an HX711 amplifier and a 3 kg load cell. The firmware is designed to work fully offline while optionally synchronizing immutable hydration events with an external Health Hub application over BLE.

## Project goals

- ESP-IDF only; no Arduino framework.
- Modern C++17 with small RAII-friendly components.
- Event-driven architecture where the hydration domain owns all raw-weight interpretation.
- Local-first behavior for measurement, reminders, statistics, storage, and display.
- Optional synchronization that transfers events, not derived analytics.

## Repository layout

```text
firmware/          ESP-IDF application and components
hardware/          Hardware notes, wiring, and mechanical assets
docs/              Architecture and product documentation
assets/            Images and design assets
scripts/           Developer automation
.github/           CI workflows
```

## Firmware components

The initial firmware skeleton defines these ESP-IDF components from the start:

- `hx711`
- `oled`
- `led`
- `weight`
- `hydration`
- `event_store`
- `storage`
- `sync`
- `reminder`
- `logging`

The intended data flow is documented in [`docs/architecture.md`](docs/architecture.md).

## Building

Install ESP-IDF, export the ESP-IDF environment, then build the firmware:

```sh
cd firmware
idf.py set-target esp32
idf.py build
```
