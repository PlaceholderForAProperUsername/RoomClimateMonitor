set(TARGET "RoomClimateMonitor")

add_executable(${TARGET})

target_sources(${TARGET} PRIVATE main.cpp)

target_link_libraries(${TARGET} PRIVATE RP2040)

add_subdirectory(config)
add_subdirectory(rp2040)
add_subdirectory(sensors)