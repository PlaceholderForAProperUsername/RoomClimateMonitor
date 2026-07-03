set(TARGET "RP2040_System")

add_library(${TARGET} INTERFACE)

target_link_libraries(${TARGET} INTERFACE
        RP2040_System_Clocks
        RP2040_System_PLL
        RP2040_System_Resets
        RP2040_System_XOSC
)

add_subdirectory(clocks)
add_subdirectory(pll)
add_subdirectory(resets)
add_subdirectory(xosc)