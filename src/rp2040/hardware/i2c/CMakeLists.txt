set(TARGET "RP2040_Hardware_I2C")

add_library(${TARGET} INTERFACE)

target_include_directories(${TARGET} INTERFACE include)

target_link_libraries(${TARGET} INTERFACE RP2040_RegAccess RP2040_System_Resets Status RP2040_System_PLL)