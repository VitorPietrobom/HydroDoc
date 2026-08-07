#pragma once
#include <optional>
#include "event_store/events.hpp"
#include "weight/weight.hpp"
namespace hydromonitor::hydration {
class DrinkDetector final {
public:
    std::optional<event_store::Event> process(weight::WeightSample sample);
};
} // namespace hydromonitor::hydration
