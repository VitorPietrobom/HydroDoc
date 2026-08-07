#include "logging/logging.hpp"
#include "esp_log.h"
namespace hydromonitor::logging {
void initialize() { esp_log_level_set("*", ESP_LOG_INFO); }
} // namespace hydromonitor::logging
