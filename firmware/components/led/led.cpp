#include "led/led.hpp"

namespace hydromonitor::led {
StatusLed::StatusLed(gpio_num_t pin) : pin_(pin) {}
esp_err_t StatusLed::initialize()
{
    gpio_config_t config{};
    config.pin_bit_mask = 1ULL << pin_;
    config.mode = GPIO_MODE_OUTPUT;
    return gpio_config(&config);
}
esp_err_t StatusLed::set(bool enabled) { return gpio_set_level(pin_, enabled ? 1 : 0); }
} // namespace hydromonitor::led
