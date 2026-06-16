set(TARGET "RP2040_System_XOSC")

add_library(${TARGET} INTERFACE)

target_include_directories(${TARGET} INTERFACE include)

target_link_libraries(${TARGET} INTERFACE Utils_Reg_Access)