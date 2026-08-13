idf_component_register(
    SRCS "weight.cpp"
    INCLUDE_DIRS "include"
    REQUIRES hx711 esp_timer log
)