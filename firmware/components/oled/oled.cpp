#include "oled/oled.hpp"

#include <array>
#include <cstddef>
#include <cstdio>
#include <cstring>

#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

namespace hydromonitor::oled {
namespace {
constexpr char kTag[] = "OLED";
constexpr uint32_t kI2cFrequencyHz = 400000;
constexpr TickType_t kI2cTimeout = pdMS_TO_TICKS(100);
constexpr uint8_t kSsd1306Width = 128;
constexpr uint8_t kSsd1306Pages = 8;
constexpr uint8_t kControlCommand = 0x00;
constexpr uint8_t kControlData = 0x40;
constexpr uint8_t kSsd1306Address = 0x3C;

constexpr std::array<uint8_t, 5> glyph_for(char ch)
{
    switch (ch) {
    case '0': return {0x3E, 0x51, 0x49, 0x45, 0x3E};
    case '1': return {0x00, 0x42, 0x7F, 0x40, 0x00};
    case '2': return {0x42, 0x61, 0x51, 0x49, 0x46};
    case '3': return {0x21, 0x41, 0x45, 0x4B, 0x31};
    case '4': return {0x18, 0x14, 0x12, 0x7F, 0x10};
    case '5': return {0x27, 0x45, 0x45, 0x45, 0x39};
    case '6': return {0x3C, 0x4A, 0x49, 0x49, 0x30};
    case '7': return {0x01, 0x71, 0x09, 0x05, 0x03};
    case '8': return {0x36, 0x49, 0x49, 0x49, 0x36};
    case '9': return {0x06, 0x49, 0x49, 0x29, 0x1E};
    case ':': return {0x00, 0x36, 0x36, 0x00, 0x00};
    case 'C': return {0x3E, 0x41, 0x41, 0x41, 0x22};
    case 'D': return {0x7F, 0x41, 0x41, 0x22, 0x1C};
    case 'E': return {0x7F, 0x49, 0x49, 0x49, 0x41};
    case 'H': return {0x7F, 0x08, 0x08, 0x08, 0x7F};
    case 'K': return {0x7F, 0x08, 0x14, 0x22, 0x41};
    case 'L': return {0x7F, 0x40, 0x40, 0x40, 0x40};
    case 'N': return {0x7F, 0x04, 0x08, 0x10, 0x7F};
    case 'O': return {0x3E, 0x41, 0x41, 0x41, 0x3E};
    case 'R': return {0x7F, 0x09, 0x19, 0x29, 0x46};
    case 'S': return {0x46, 0x49, 0x49, 0x49, 0x31};
    case 'T': return {0x01, 0x01, 0x7F, 0x01, 0x01};
    case 'U': return {0x3F, 0x40, 0x40, 0x40, 0x3F};
    case 'Y': return {0x07, 0x08, 0x70, 0x08, 0x07};
    default: return {0x00, 0x00, 0x00, 0x00, 0x00};
    }
}
} // namespace

esp_err_t Display::initialize(i2c_port_num_t port, gpio_num_t sda_pin, gpio_num_t scl_pin)
{
    i2c_master_bus_config_t bus_config{};
    bus_config.i2c_port = port;
    bus_config.sda_io_num = sda_pin;
    bus_config.scl_io_num = scl_pin;
    bus_config.clk_source = I2C_CLK_SRC_DEFAULT;
    bus_config.glitch_ignore_cnt = 7;
    bus_config.flags.enable_internal_pullup = true;

    esp_err_t result = i2c_new_master_bus(&bus_config, &bus_handle_);
    if (result == ESP_ERR_INVALID_STATE) {
        ESP_RETURN_ON_ERROR(i2c_master_get_bus_handle(port, &bus_handle_), kTag, "I2C bus handle lookup failed");
    } else if (result != ESP_OK) {
        ESP_LOGE(kTag, "I2C master bus init failed: %s", esp_err_to_name(result));
        return result;
    }

    ESP_RETURN_ON_ERROR(i2c_master_probe(bus_handle_, kSsd1306Address, static_cast<int>(pdTICKS_TO_MS(kI2cTimeout))),
                        kTag, "SSD1306 probe failed at address 0x%02X", kSsd1306Address);

    i2c_device_config_t device_config{};
    device_config.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    device_config.device_address = kSsd1306Address;
    device_config.scl_speed_hz = kI2cFrequencyHz;

    ESP_RETURN_ON_ERROR(i2c_master_bus_add_device(bus_handle_, &device_config, &device_handle_), kTag,
                        "SSD1306 device add failed");
    ESP_LOGI(kTag, "Using SSD1306 at I2C address 0x%02X", kSsd1306Address);

    constexpr std::array<uint8_t, 25> init_commands{
        0xAE, 0x20, 0x00, 0xB0, 0xC8, 0x00, 0x10, 0x40, 0x81, 0x7F, 0xA1, 0xA6, 0xA8,
        0x3F, 0xA4, 0xD3, 0x00, 0xD5, 0x80, 0xD9, 0xF1, 0xDA, 0x12, 0xDB, 0x40};
    for (const uint8_t command : init_commands) {
        ESP_RETURN_ON_ERROR(write_command(command), kTag, "SSD1306 init command failed");
    }
    ESP_RETURN_ON_ERROR(write_command(0x8D), kTag, "SSD1306 charge pump command failed");
    ESP_RETURN_ON_ERROR(write_command(0x14), kTag, "SSD1306 charge pump enable failed");
    ESP_RETURN_ON_ERROR(write_command(0xAF), kTag, "SSD1306 display on failed");

    initialized_ = true;
    return clear();
}

esp_err_t Display::write_command(uint8_t command)
{
    const std::array<uint8_t, 2> payload{kControlCommand, command};
    return i2c_master_transmit(device_handle_, payload.data(), payload.size(),
                               static_cast<int>(pdTICKS_TO_MS(kI2cTimeout)));
}

esp_err_t Display::write_data(const uint8_t* data, std::size_t length)
{
    std::array<uint8_t, 17> payload{};
    payload[0] = kControlData;
    while (length > 0) {
        const std::size_t chunk = length > payload.size() - 1 ? payload.size() - 1 : length;
        std::memcpy(&payload[1], data, chunk);
        const int timeout_ms = static_cast<int>(pdTICKS_TO_MS(kI2cTimeout));
        ESP_RETURN_ON_ERROR(i2c_master_transmit(device_handle_, payload.data(), chunk + 1, timeout_ms), kTag,
                            "SSD1306 data write failed");
        data += chunk;
        length -= chunk;
    }
    return ESP_OK;
}

esp_err_t Display::clear()
{
    if (!initialized_) {
        return ESP_ERR_INVALID_STATE;
    }

    std::array<uint8_t, kSsd1306Width> zeros{};
    for (uint8_t page = 0; page < kSsd1306Pages; ++page) {
        ESP_RETURN_ON_ERROR(set_cursor(page, 0), kTag, "SSD1306 cursor failed");
        ESP_RETURN_ON_ERROR(write_data(zeros.data(), zeros.size()), kTag, "SSD1306 clear failed");
    }
    return ESP_OK;
}

esp_err_t Display::set_cursor(uint8_t page, uint8_t column)
{
    ESP_RETURN_ON_ERROR(write_command(static_cast<uint8_t>(0xB0 | (page & 0x07))), kTag, "SSD1306 page failed");
    ESP_RETURN_ON_ERROR(write_command(static_cast<uint8_t>(column & 0x0F)), kTag, "SSD1306 low column failed");
    return write_command(static_cast<uint8_t>(0x10 | (column >> 4)));
}

esp_err_t Display::draw_text(uint8_t page, uint8_t column, const char* text)
{
    ESP_RETURN_ON_ERROR(set_cursor(page, column), kTag, "SSD1306 text cursor failed");
    while (*text != '\0') {
        const auto glyph = glyph_for(*text++);
        std::array<uint8_t, 6> bytes{glyph[0], glyph[1], glyph[2], glyph[3], glyph[4], 0x00};
        ESP_RETURN_ON_ERROR(write_data(bytes.data(), bytes.size()), kTag, "SSD1306 text write failed");
    }
    return ESP_OK;
}

esp_err_t Display::render_hardware_test(uint32_t counter)
{
    ESP_RETURN_ON_ERROR(clear(), kTag, "SSD1306 hardware test clear failed");
    ESP_RETURN_ON_ERROR(draw_text(1, 0, "HYDRODOCK"), kTag, "SSD1306 title draw failed");
    ESP_RETURN_ON_ERROR(draw_text(3, 0, "OLED TEST"), kTag, "SSD1306 subtitle draw failed");

    char counter_text[16]{};
    std::snprintf(counter_text, sizeof(counter_text), "COUNT: %lu", static_cast<unsigned long>(counter));
    return draw_text(5, 0, counter_text);
}

void Display::render(const DisplayState& state) { state_ = state; }
const DisplayState& Display::state() const { return state_; }
} // namespace hydromonitor::oled
