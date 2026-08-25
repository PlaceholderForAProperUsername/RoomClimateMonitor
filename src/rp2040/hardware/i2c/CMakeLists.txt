set(TARGET "RP2040_Hardware_I2C")

add_library(${TARGET} INTERFACE)

target_include_directories(${TARGET} INTERFACE include)

target_link_libraries(${TARGET} PRIVATE )