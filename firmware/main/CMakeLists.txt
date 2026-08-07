idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "."
    REQUIRES hx711 oled led weight hydration event_store storage sync reminder logging log
)
