#pragma once

#include <cstdint>

namespace hydromonitor::oled {

struct DisplayState {
    uint16_t intake_ml{};
    uint16_t goal_ml{};
    int32_t bottle_weight_g{};
    bool sync_connected{};
};

class Display final {
public:
    void render(const DisplayState& state);
    const DisplayState& state() const;

private:
    DisplayState state_{};
};

} // namespace hydromonitor::oled
