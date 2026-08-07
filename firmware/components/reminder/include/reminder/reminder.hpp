#pragma once
#include <cstdint>
namespace hydromonitor::reminder {
class ReminderPolicy final { public: bool should_remind(uint64_t now_ms, uint64_t last_drink_ms) const; };
} // namespace hydromonitor::reminder
