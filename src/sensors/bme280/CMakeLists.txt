set(TARGET "BME280")

add_library(${TARGET} INTERFACE )

target_include_directories(${TARGET} INTERFACE include)

target_link_libraries(${TARGET} INTERFACE)