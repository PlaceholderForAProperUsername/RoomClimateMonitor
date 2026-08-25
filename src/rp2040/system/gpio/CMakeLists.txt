set(TARGET "RP2040_System_GPIO")

add_library(${TARGET} INTERFACE)

target_include_directories(${TARGET} INTERFACE include)

target_link_libraries(${TARGET} INTERFACE RP2040_RegAccess Status RP2040_System_Resets)