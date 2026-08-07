#include "esp_log.h"

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

extern "C" void app_main(void)
{
    hydromonitor::logging::initialize();
    ESP_LOGI("HydroDock", "Water Bottle Monitor firmware starting");
}
