set(TARGET "Peripherals")

add_library(${TARGET} INTERFACE)

target_link_libraries(${TARGET} INTERFACE BH1745 BME280 SGP30)

add_subdirectory(bh1745)
add_subdirectory(bme280)
add_subdirectory(sgp30)