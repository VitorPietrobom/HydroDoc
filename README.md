# HydroDock

HydroDock is an open-source ESP32-powered smart hydration dock that tracks water intake using a load cell, provides local reminders, and synchronizes hydration events with a personal health platform.

The project is intentionally built as a production-minded embedded system rather than an Arduino-style prototype. The firmware owns the hardware, local reminders, local event history, and optional synchronization. A companion app can add dashboards, health integrations, long-term analytics, and cloud features without being required for the dock to work.

> Status: early foundation. The repository currently contains the ESP-IDF project structure, component boundaries, documentation, and CI scaffolding for future implementation milestones.

## Why this project exists

Hydration tracking devices often depend on a phone or cloud service to be useful. HydroDock takes a local-first approach:

- the dock should keep measuring and reminding even if Bluetooth is disabled;
- events should be stored locally before any sync attempt;
- synchronization should exchange immutable events, not calculated totals;
- the architecture should remain easy to test, document, and extend.

## Project goals

- ESP-IDF only; no Arduino framework.
- Modern C++17 with small, focused components.
- Event-driven architecture where the hydration domain owns all raw-weight interpretation.
- Local-first behavior for measurement, reminders, statistics, storage, and display.
- Optional synchronization with an external Health Hub application.
- Clear documentation for builders, contributors, and future maintainers.

## Hardware baseline

- ESP32 module or development board.
- HX711 load-cell amplifier.
- 3 kg load cell.
- OLED display.
- Reminder/status LED.

See [`hardware/README.md`](hardware/README.md) for hardware notes as they evolve.

## Firmware architecture

The firmware follows a one-way pipeline:

```text
HX711 -> WeightSampler -> WeightFilter -> DrinkDetector -> EventStore -> Storage -> BLE Sync -> Display -> Reminder
```

No component should bypass this flow. Everything downstream consumes events or presentation state, and only the hydration domain should interpret raw weight changes as product behavior.

Read the architecture notes in [`docs/architecture.md`](docs/architecture.md).

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

The initial firmware foundation defines these ESP-IDF components from the start:

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

Each component owns a narrow public interface so implementation can be added incrementally without collapsing the architecture into global state.

## Building the firmware

Install ESP-IDF, export the ESP-IDF environment, then build the firmware:

```sh
cd firmware
idf.py set-target esp32
idf.py build
```

CI also runs the firmware build inside the official ESP-IDF container.

## Roadmap

The project is organized around incremental milestones:

1. Repository, ESP-IDF build, documentation, and CI.
2. HX711, OLED, and LED drivers.
3. Weight filtering, drink detection, and calibration.
4. Event storage, persistence, and local statistics.
5. BLE event synchronization.
6. Production polish and diagnostics.

See [`ROADMAP.md`](ROADMAP.md) for the detailed roadmap.

## Contributing

Contributions are welcome, especially documentation improvements, hardware validation notes, tests, and small firmware changes that preserve the architecture. Please read [`CONTRIBUTING.md`](CONTRIBUTING.md) before opening an issue or pull request.

## License

HydroDock is available under the MIT License. See [`LICENSE`](LICENSE).
