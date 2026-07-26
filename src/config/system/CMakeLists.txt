set(TARGET "Config_System")

add_library(${TARGET} OBJECT)

target_sources(${TARGET} PRIVATE config_sys.cpp)

target_include_directories(${TARGET} PUBLIC include)

target_link_libraries(${TARGET} PRIVATE RP2040_System)