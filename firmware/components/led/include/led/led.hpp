#pragma once

#include "driver/gpio.h"
#include "esp_err.h"

namespace hydromonitor::led {
class StatusLed final {
public:
    explicit StatusLed(gpio_num_t pin);
    esp_err_t initialize();
    esp_err_t set(bool enabled);
private:
    gpio_num_t pin_;
};
} // namespace hydromonitor::led
