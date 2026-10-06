set(TARGET "RP2040_Hardware_SPI")

add_library(${TARGET} INTERFACE)

target_include_directories(${TARGET} INTERFACE include)

target_link_libraries(${TARGET} INTERFACE RP2040_RegAccess RP2040_System_Resets Status)