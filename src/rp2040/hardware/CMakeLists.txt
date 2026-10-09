set(TARGET "RP2040_Hardware")

add_library(${TARGET} INTERFACE)

target_link_libraries(${TARGET} INTERFACE
        RP2040_Hardware_I2C
        RP2040_Hardware_SPI
)

add_subdirectory(i2c)
add_subdirectory(spi)