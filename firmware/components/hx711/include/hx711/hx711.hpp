#pragma once

#include <cstdint>

#include "driver/gpio.h"
#include "esp_check.h"
#include "esp_err.h"

namespace hydromonitor::hx711 {

class Hx711 final {
public:
    Hx711(gpio_num_t data_pin, gpio_num_t clock_pin);
    esp_err_t initialize();
    bool is_ready() const;
    esp_err_t read_raw(int32_t& value, uint32_t timeout_ms = 1000) const;

private:
    gpio_num_t data_pin_;
    gpio_num_t clock_pin_;
};

} // namespace hydromonitor::hx711
