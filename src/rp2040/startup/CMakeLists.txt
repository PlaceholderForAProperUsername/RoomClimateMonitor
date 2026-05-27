set(TARGET "RP2040_Startup")

add_library(${TARGET} OBJECT)

target_sources(
        ${TARGET} PUBLIC
        bs2_default_padded_checksummed.S
        rp2040_ls.ld
        rp2040_startup.S
        syscalls.cpp
)