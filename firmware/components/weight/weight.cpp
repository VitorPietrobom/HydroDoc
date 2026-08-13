#include "weight/weight.hpp"
#include "esp_log.h"

#include <algorithm>

#include "esp_timer.h"


namespace hydromonitor::weight {

constexpr char kTag[] = "WeightSensor";

WeightSensor::WeightSensor(
    hx711::Hx711& hx711,
    WeightCalibration calibration
)
    : hx711_(hx711),
      calibration_(calibration)
{
}

WeightSample WeightSensor::read()
{
    int32_t raw = 0;

    const esp_err_t err = hx711_.read_raw(raw);

    if (err != ESP_OK) {
        ESP_LOGW(kTag, "HX711 read failed: %s", esp_err_to_name(err));
        return {};
    }

    WeightSample sample{
        .timestamp_ms =
            static_cast<uint64_t>(esp_timer_get_time() / 1000),
        .grams = raw_to_grams(raw)
    };

    return filter(sample);
}

void WeightSensor::set_calibration(WeightCalibration calibration)
{
    calibration_ = calibration;

    // Calibration changed, so discard the old filter history.
    samples_.fill(0);
    sample_count_ = 0;
    next_index_ = 0;
}

int32_t WeightSensor::raw_to_grams(int32_t raw) const
{
    if (calibration_.grams_per_count == 0.0f) {
        return 0;
    }

    const float grams =
        static_cast<float>(calibration_.tare_raw - raw)
        * calibration_.grams_per_count;

    return static_cast<int32_t>(grams);
}

WeightSample WeightSensor::filter(WeightSample sample)
{
    samples_[next_index_] = sample.grams;

    next_index_ =
        (next_index_ + 1) % kWindowSize;

    if (sample_count_ < kWindowSize) {
        ++sample_count_;
    }

    int64_t sum = 0;

    for (std::size_t i = 0; i < sample_count_; ++i) {
        sum += samples_[i];
    }

    sample.grams =
        static_cast<int32_t>(sum / sample_count_);

    return sample;
}

} // namespace hydromonitor::weight