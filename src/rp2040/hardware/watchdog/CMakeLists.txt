set(TARGET "RP2040_Hardware_Watchdog")

add_library(${TARGET} INTERFACE)

target_include_directories(${TARGET} INTERFACE include)