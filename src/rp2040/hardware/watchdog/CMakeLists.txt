set(TARGET "RP2040_Hardware_Watchdog")

add_library(${TARGET} INTERFACE)

target_include_directories(${TARGET} INTERFACE include)

target_link_libraries(${TARGET} INTERFACE RP2040_RegAccess)