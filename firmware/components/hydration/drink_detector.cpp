#include "hydration/drink_detector.hpp"
namespace hydromonitor::hydration {
std::optional<event_store::Event> DrinkDetector::process(weight::WeightSample) { return std::nullopt; }
} // namespace hydromonitor::hydration
