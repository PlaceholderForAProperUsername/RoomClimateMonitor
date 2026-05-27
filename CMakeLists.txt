cmake_minimum_required(VERSION 3.31)


include(cmake/CMakeLists.txt)

project(
        RoomClimateMonitor
        VERSION 1.0
        DESCRIPTION "A RP2040 project to monitor indoor climate parameters"
        LANGUAGES ASM C CXX
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
add_subdirectory(src)
