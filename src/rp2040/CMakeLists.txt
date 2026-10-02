set(TARGET "RP2040")

add_library(${TARGET} INTERFACE)

add_subdirectory(hardware)
add_subdirectory(reg_access)
add_subdirectory(startup)
add_subdirectory(system)

target_link_libraries(${TARGET} INTERFACE RP2040_Hardware RP2040_Startup RP2040_System)