set(TARGET "RP2040_System_Clocks")

add_library(${TARGET} INTERFACE)

target_include_directories(${TARGET} INTERFACE include)

target_link_libraries(${TARGET} INTERFACE Utils_Reg_Access Utils_Status )