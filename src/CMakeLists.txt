set(TARGET "RoomClimateMonitor")

add_executable(${TARGET})

target_sources(${TARGET} PRIVATE main.cpp)

target_link_libraries(${TARGET} PRIVATE RP2040 RP2040_System_XOSC)

add_subdirectory(config)
add_subdirectory(rp2040)
add_subdirectory(sensors)
add_subdirectory(st7789)
add_subdirectory(utils)