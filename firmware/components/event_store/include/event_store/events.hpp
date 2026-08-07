#pragma once
#include <cstdint>
namespace hydromonitor::event_store {
enum class EventType : uint8_t { Drink, Refill, Calibration, Battery, Sync };
struct EventHeader { uint32_t id{}; uint64_t timestamp_ms{}; EventType type{}; };
struct Event { EventHeader header{}; uint16_t volume_ml{}; };
} // namespace hydromonitor::event_store
