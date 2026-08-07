#include "weight/weight.hpp"
namespace hydromonitor::weight {
WeightSample WeightFilter::apply(WeightSample sample) { last_ = sample; return last_; }
} // namespace hydromonitor::weight
