set(TARGET "RP2040_Hardware")

add_library(${TARGET} INTERFACE)

target_link_libraries(${TARGET} INTERFACE
        RP2040_Hardware_I2C
        RP2040_Hardware_SPI
        RP2040_Hardware_Timer
        RP2040_Hardware_Watchdog
)

add_subdirectory(i2c)
add_subdirectory(spi)
add_subdirectory(timer)
add_subdirectory(watchdog)