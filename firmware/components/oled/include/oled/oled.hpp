#pragma once

#include <cstddef>
#include <cstdint>

#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "esp_err.h"

namespace hydromonitor::oled {

struct DisplayState {
    uint16_t intake_ml{};
    uint16_t goal_ml{};
    int32_t bottle_weight_g{};
    bool sync_connected{};
};

class Display final {
public:
    esp_err_t initialize(i2c_port_num_t port, gpio_num_t sda_pin, gpio_num_t scl_pin);
    esp_err_t clear();
    esp_err_t render_hardware_test(uint32_t counter);
    esp_err_t render_hx711_raw(int32_t raw_value);
    esp_err_t render_hx711_error();

    void render(const DisplayState& state);
    const DisplayState& state() const;

private:
    esp_err_t write_command(uint8_t command);
    esp_err_t write_data(const uint8_t* data, std::size_t length);
    esp_err_t set_cursor(uint8_t page, uint8_t column);
    esp_err_t draw_text(uint8_t page, uint8_t column, const char* text);

    i2c_master_bus_handle_t bus_handle_{};
    i2c_master_dev_handle_t device_handle_{};
    bool initialized_{};
    DisplayState state_{};
};

} // namespace hydromonitor::oled
