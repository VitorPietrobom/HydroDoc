#include "reminder/reminder.hpp"
namespace hydromonitor::reminder {
bool ReminderPolicy::should_remind(uint64_t now_ms, uint64_t last_drink_ms) const { return now_ms > last_drink_ms && (now_ms - last_drink_ms) >= 3600000ULL; }
} // namespace hydromonitor::reminder
