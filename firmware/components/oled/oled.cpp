#include "oled/oled.hpp"

namespace hydromonitor::oled {
void Display::render(const DisplayState& state) { state_ = state; }
const DisplayState& Display::state() const { return state_; }
} // namespace hydromonitor::oled
