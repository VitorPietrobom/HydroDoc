#pragma once

#include "driver/gpio.h"
#include "esp_check.h"
#include "esp_err.h"

namespace hydromonitor::hx711 {

class Hx711 final {
public:
    Hx711(gpio_num_t data_pin, gpio_num_t clock_pin);
    esp_err_t initialize();

private:
    gpio_num_t data_pin_;
    gpio_num_t clock_pin_;
};

} // namespace hydromonitor::hx711
