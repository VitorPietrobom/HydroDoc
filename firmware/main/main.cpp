#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "event_store/event_store.hpp"
#include "hydration/drink_detector.hpp"
#include "hx711/hx711.hpp"
#include "led/led.hpp"
#include "logging/logging.hpp"
#include "oled/oled.hpp"
#include "reminder/reminder.hpp"
#include "storage/storage.hpp"
#include "sync/sync.hpp"
#include "weight/weight.hpp"

namespace {
constexpr char kTag[] = "HydroDock";
constexpr gpio_num_t kOledSdaPin = GPIO_NUM_21;
constexpr gpio_num_t kOledSclPin = GPIO_NUM_22;
constexpr gpio_num_t kHx711DataPin = GPIO_NUM_26;
constexpr gpio_num_t kHx711ClockPin = GPIO_NUM_25;
constexpr TickType_t kHardwareTestInterval = pdMS_TO_TICKS(500);
constexpr uint32_t kHx711TimeoutMs = 250;
} // namespace

extern "C" void app_main(void)
{
    hydromonitor::logging::initialize();
    ESP_LOGI(kTag, "Water Bottle Monitor firmware starting");
    ESP_LOGI(kTag, "Starting temporary HX711 + SSD1306 hardware test");
    ESP_LOGI(kTag, "HX711 DOUT GPIO%d, SCK GPIO%d", kHx711DataPin, kHx711ClockPin);
    ESP_LOGI(kTag, "SSD1306 OLED SDA GPIO%d, SCL GPIO%d, address 0x3C", kOledSdaPin, kOledSclPin);

    hydromonitor::hx711::Hx711 scale(kHx711DataPin, kHx711ClockPin);
    const esp_err_t hx711_init_result = scale.initialize();
    if (hx711_init_result != ESP_OK) {
        ESP_LOGE(kTag, "HX711 initialization failed: %s", esp_err_to_name(hx711_init_result));
    }

    hydromonitor::oled::Display display;
    const esp_err_t oled_init_result = display.initialize(I2C_NUM_0, kOledSdaPin, kOledSclPin);
    if (oled_init_result != ESP_OK) {
        ESP_LOGE(kTag, "OLED initialization failed: %s", esp_err_to_name(oled_init_result));
    }

    while (true) {
        int32_t raw_value = 0;
        const esp_err_t read_result =
            hx711_init_result == ESP_OK ? scale.read_raw(raw_value, kHx711TimeoutMs) : hx711_init_result;

        esp_err_t render_result = ESP_OK;
        if (read_result == ESP_OK) {
            ESP_LOGI(kTag, "HX711 raw: %ld", static_cast<long>(raw_value));
            if (oled_init_result == ESP_OK) {
                render_result = display.render_hx711_raw(raw_value);
            }
        } else {
            ESP_LOGW(kTag, "HX711 read failed: %s", esp_err_to_name(read_result));
            if (oled_init_result == ESP_OK) {
                render_result = display.render_hx711_error();
            }
        }

        if (render_result != ESP_OK) {
            ESP_LOGW(kTag, "OLED render failed: %s", esp_err_to_name(render_result));
        }

        vTaskDelay(kHardwareTestInterval);
    }
}
