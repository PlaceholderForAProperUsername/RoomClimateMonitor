set(TARGET "Sensors")

add_library(${TARGET} INTERFACE)

target_link_libraries(${TARGET} INTERFACE BME280 SGP30)

add_subdirectory(bme280)
add_subdirectory(sgp30)