ENTRY(Reset_Handler)

MEMORY {
    FLASH(rx) : ORIGIN = 0x10000000, LENGTH = 2M
    RAM(rwx) : ORIGIN = 0x20000000, LENGTH = 264K
}

__end_stack__ = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    .boot2 : {
        __boot2_start__ = .;
        KEEP(*(.boot2))
        __boot2_end__ = .;
    } > FLASH

    ASSERT(__boot2_end__ - __boot2_start__ == 256, "ERROR: Pico second stage bootloader must be 256 bytes in size")

    .text : {
        . = ALIGN(4);
        KEEP(*(.isr_vector_table))
        *(.text*)
        *(.rodata*)
        *(.init*)
        *(.fini*)
        *(.eh_frame*)
        *(.ARM.extab*)
        *(.ARM.exidx*)
        . = ALIGN(4);
        _text_end = .;
    } > FLASH

    __stored_data__ = .;

    .data : AT(__stored_data__) {
        . = ALIGN(4);
        __data_start__ = .;
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } > RAM

    .bss (NOLOAD) : {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM

    _start_heap = .;
}