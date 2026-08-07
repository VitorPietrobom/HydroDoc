#pragma once
#include <cstdint>
namespace hydromonitor::sync {
struct SyncCursor { uint32_t last_event_id{}; };
class SyncService final { public: void acknowledge(SyncCursor cursor); SyncCursor cursor() const; private: SyncCursor cursor_{}; };
} // namespace hydromonitor::sync
