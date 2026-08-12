#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "event_store/event_store.hpp"
#include "hydration/drink_detector.hpp"
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
constexpr TickType_t kDisplayInterval = pdMS_TO_TICKS(1000);
} // namespace

extern "C" void app_main(void)
{
    hydromonitor::logging::initialize();
    ESP_LOGI(kTag, "Water Bottle Monitor firmware starting");
    ESP_LOGI(kTag, "Starting temporary SSD1306 OLED hardware test on SDA GPIO%d, SCL GPIO%d", kOledSdaPin,
             kOledSclPin);

    hydromonitor::oled::Display display;
    const esp_err_t init_result = display.initialize(I2C_NUM_0, kOledSdaPin, kOledSclPin);
    if (init_result != ESP_OK) {
        ESP_LOGE(kTag, "OLED initialization failed: %s", esp_err_to_name(init_result));
        return;
    }

    uint32_t counter = 0;
    while (true) {
        const esp_err_t render_result = display.render_hardware_test(counter++);
        if (render_result != ESP_OK) {
            ESP_LOGW(kTag, "OLED render failed: %s", esp_err_to_name(render_result));
        }
        vTaskDelay(kDisplayInterval);
    }
}
