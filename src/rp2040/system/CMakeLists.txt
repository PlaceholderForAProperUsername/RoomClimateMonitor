set(TARGET "RP2040_System")

add_library(${TARGET} INTERFACE)

target_link_libraries(${TARGET} INTERFACE RP2040_System_XOSC)

add_subdirectory(xosc)