#pragma once
#include "esp_err.h"
namespace hydromonitor::storage {
class Storage final { public: esp_err_t initialize(); };
} // namespace hydromonitor::storage
