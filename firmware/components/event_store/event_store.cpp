#include "event_store/event_store.hpp"
namespace hydromonitor::event_store {
bool EventStore::append(const Event& event) { latest_ = event; return true; }
std::optional<Event> EventStore::latest() const { return latest_; }
} // namespace hydromonitor::event_store
