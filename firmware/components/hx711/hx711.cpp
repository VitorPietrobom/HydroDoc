#include "hx711/hx711.hpp"

namespace hydromonitor::hx711 {

Hx711::Hx711(gpio_num_t data_pin, gpio_num_t clock_pin) : data_pin_(data_pin), clock_pin_(clock_pin) {}

esp_err_t Hx711::initialize()
{
    gpio_config_t io_conf{};
    io_conf.pin_bit_mask = (1ULL << data_pin_);
    io_conf.mode = GPIO_MODE_INPUT;
    ESP_RETURN_ON_ERROR(gpio_config(&io_conf), "Hx711", "configure data pin");

    io_conf = gpio_config_t{};
    io_conf.pin_bit_mask = (1ULL << clock_pin_);
    io_conf.mode = GPIO_MODE_OUTPUT;
    ESP_RETURN_ON_ERROR(gpio_config(&io_conf), "Hx711", "configure clock pin");
    return gpio_set_level(clock_pin_, 0);
}

} // namespace hydromonitor::hx711
