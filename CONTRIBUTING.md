# Contributing to HydroDock

Thank you for considering a contribution. HydroDock is intended to be a welcoming first open-source embedded project while still keeping a production-quality engineering bar.

## Good first contributions

Helpful early contributions include:

- improving documentation and diagrams;
- validating wiring notes with real hardware;
- adding small tests around pure C++ logic;
- improving CI and developer scripts;
- implementing one narrow component behavior at a time.

## Development principles

Please keep changes aligned with the project philosophy:

- use ESP-IDF, not Arduino abstractions;
- prefer modern C++17, RAII, and composition;
- avoid global mutable state and singleton-style shortcuts;
- keep components small and independently testable;
- do not let display, sync, storage, or reminder code interpret raw weight;
- synchronize immutable events instead of calculated totals.

## Pull request checklist

Before opening a pull request, please verify that:

- the change is scoped to one clear purpose;
- public interfaces are documented through readable names and small types;
- new behavior includes tests where practical;
- documentation is updated when architecture, setup, or user behavior changes;
- `scripts/check_firmware_layout.sh` passes;
- `idf.py build` passes if ESP-IDF is available locally.

## Commit style

Use short, imperative commit messages, for example:

```text
Add weight sample filtering interface
Document HX711 wiring assumptions
```

## Questions and design discussions

If an architectural choice is unclear, open an issue or discussion before implementing. It is better to compare tradeoffs early than to add code that will be hard to unwind later.
