#!/usr/bin/env bash
set -euo pipefail

required_components=(hx711 oled led weight hydration event_store storage sync reminder logging)
for component in "${required_components[@]}"; do
  test -f "firmware/components/${component}/CMakeLists.txt"
  test -d "firmware/components/${component}/include/${component}"
done

test -f firmware/CMakeLists.txt
test -f firmware/main/main.cpp
test -f firmware/sdkconfig.defaults
