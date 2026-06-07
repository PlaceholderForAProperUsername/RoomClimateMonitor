cmake_minimum_required(VERSION 3.31)

include(cmake/CMakeLists.txt)

project(
        RoomClimateMonitor
        VERSION 1.0
        DESCRIPTION "A RP2040 project to monitor indoor climate parameters"
        LANGUAGES ASM CXX
)

set(CMAKE_CXX_STANDARD 23)
add_subdirectory(src)
