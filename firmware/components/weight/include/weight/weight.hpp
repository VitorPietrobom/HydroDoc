#pragma once
#include <cstdint>
namespace hydromonitor::weight {
struct WeightSample { uint64_t timestamp_ms{}; int32_t grams{}; };
class WeightFilter final { public: WeightSample apply(WeightSample sample); private: WeightSample last_{}; };
} // namespace hydromonitor::weight
