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
constexpr gpio_num_t kHx711DataPin = GPIO_NUM_26;
constexpr gpio_num_t kHx711ClockPin = GPIO_NUM_25;
constexpr TickType_t kReadInterval = pdMS_TO_TICKS(500);
} // namespace

extern "C" void app_main(void)
{
    hydromonitor::logging::initialize();
    ESP_LOGI(kTag, "Water Bottle Monitor firmware starting");
    ESP_LOGI(kTag, "Starting temporary HX711 raw ADC hardware test on DOUT GPIO%d, SCK GPIO%d", kHx711DataPin,
             kHx711ClockPin);

    hydromonitor::hx711::Hx711 scale{kHx711DataPin, kHx711ClockPin};
    const esp_err_t init_result = scale.initialize();
    if (init_result != ESP_OK) {
        ESP_LOGE(kTag, "HX711 initialization failed: %s", esp_err_to_name(init_result));
        return;
    }

    while (true) {
        int32_t raw_value = 0;
        const esp_err_t read_result = scale.read_raw(raw_value);
        if (read_result == ESP_OK) {
            ESP_LOGI(kTag, "HX711 raw: %ld", static_cast<long>(raw_value));
        } else {
            ESP_LOGW(kTag, "HX711 read failed: %s", esp_err_to_name(read_result));
        }
        vTaskDelay(kReadInterval);
    }
}
