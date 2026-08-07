#pragma once
#include <optional>
#include "event_store/events.hpp"
namespace hydromonitor::event_store {
class EventStore final {
public:
    bool append(const Event& event);
    std::optional<Event> latest() const;
private:
    std::optional<Event> latest_{};
};
} // namespace hydromonitor::event_store
