#include "hx711/hx711.hpp"

#include "esp_rom_sys.h"
#include "esp_timer.h"

namespace hydromonitor::hx711 {
namespace {
constexpr int kHx711Bits = 24;
constexpr int kChannelAGain128Pulses = 1;
constexpr int64_t kUsPerMs = 1000;
} // namespace

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

bool Hx711::is_ready() const { return gpio_get_level(data_pin_) == 0; }

esp_err_t Hx711::read_raw(int32_t& value, uint32_t timeout_ms) const
{
    const int64_t deadline = esp_timer_get_time() + (static_cast<int64_t>(timeout_ms) * kUsPerMs);
    while (!is_ready()) {
        if (esp_timer_get_time() >= deadline) {
            return ESP_ERR_TIMEOUT;
        }
        esp_rom_delay_us(100);
    }

    uint32_t raw = 0;
    for (int bit = 0; bit < kHx711Bits; ++bit) {
        gpio_set_level(clock_pin_, 1);
        esp_rom_delay_us(1);
        raw = (raw << 1) | static_cast<uint32_t>(gpio_get_level(data_pin_));
        gpio_set_level(clock_pin_, 0);
        esp_rom_delay_us(1);
    }

    for (int pulse = 0; pulse < kChannelAGain128Pulses; ++pulse) {
        gpio_set_level(clock_pin_, 1);
        esp_rom_delay_us(1);
        gpio_set_level(clock_pin_, 0);
        esp_rom_delay_us(1);
    }

    if ((raw & 0x800000U) != 0) {
        raw |= 0xFF000000U;
    }
    value = static_cast<int32_t>(raw);
    return ESP_OK;
}

} // namespace hydromonitor::hx711
