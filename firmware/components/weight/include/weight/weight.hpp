#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "hx711/hx711.hpp"

namespace hydromonitor::weight {

struct WeightSample {
    uint64_t timestamp_ms{};
    int32_t grams{};
};

struct WeightCalibration {
    int32_t tare_raw{};
    float grams_per_count{};
};

class WeightSensor final {
public:
    WeightSensor(
        hx711::Hx711& hx711,
        WeightCalibration calibration
    );

    WeightSample read();

    void set_calibration(WeightCalibration calibration);

private:
    int32_t raw_to_grams(int32_t raw) const;
    WeightSample filter(WeightSample sample);

    hx711::Hx711& hx711_;
    WeightCalibration calibration_{};

    static constexpr std::size_t kWindowSize = 5;

    std::array<int32_t, kWindowSize> samples_{};
    std::size_t sample_count_{};
    std::size_t next_index_{};
};

} // namespace hydromonitor::weight