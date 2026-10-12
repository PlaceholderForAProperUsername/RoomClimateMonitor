set(TARGET "Sensors")

add_library(${TARGET} INTERFACE)

target_link_libraries(${TARGET} INTERFACE SGP30)

add_subdirectory(sgp30)