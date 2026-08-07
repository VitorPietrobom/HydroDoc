#include "storage/storage.hpp"
#include "nvs_flash.h"
namespace hydromonitor::storage {
esp_err_t Storage::initialize() { return nvs_flash_init(); }
} // namespace hydromonitor::storage
