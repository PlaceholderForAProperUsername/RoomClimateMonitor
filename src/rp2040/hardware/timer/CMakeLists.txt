set(TARGET "RP2040_Hardware_Timer")

add_library(${TARGET} INTERFACE)

target_include_directories(${TARGET} INTERFACE include)

target_link_libraries(${TARGET} INTERFACE RP2040_Hardware_Watchdog RP2040_RegAccess RP2040_System_Resets Status)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${TARGET} INTERFACE DEBUG_WITH_TIMERS=1)
endif()

