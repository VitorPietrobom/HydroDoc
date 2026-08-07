# Firmware Architecture

HydroDoc uses a local-first, event-driven firmware architecture. The firmware owns hardware interactions, local reminders, persistent event storage, and synchronization state. External applications consume events for dashboards, long-term analytics, cloud sync, and health integrations.

## Component pipeline

```text
HX711 -> WeightSampler -> WeightFilter -> DrinkDetector -> EventStore -> Storage -> BLE Sync -> Display -> Reminder
```

Only the `hydration` component may interpret raw weight changes as product behavior such as drinks or refills. Downstream components consume immutable events or presentation state derived from events.

## Event model

Events are immutable records with monotonically increasing identifiers. Synchronization exchanges events by cursor: a peer reports the latest event identifier it has received, and the device returns later events. Derived totals are intentionally not synchronized.

## Storage model

- NVS stores calibration data, settings, device configuration, and synchronization metadata.
- Onboard flash file storage stores event history and debug logs.
- SD cards and external EEPROM are intentionally excluded from the product baseline.
