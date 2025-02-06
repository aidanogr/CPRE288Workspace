MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x40000
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x8000
}

SECTIONS
{
    .intvecs : {
        KEEP(*(.isr_vector))
    } > FLASH

    .text : {
        *(.text*)
        *(.rodata*)
    } > FLASH

    .data : {
        *(.data*)
    } > SRAM AT > FLASH

    .bss : {
        *(.bss*)
    } > SRAM

    .stack : {
        . = ALIGN(8);
        __stack_start__ = .;
        . += 0x200;
        __stack_end__ = .;
    } > SRAM

    .heap : {
        __heap_start__ = .;
        . += 0x200;
        __heap_end__ = .;
    } > SRAM
}

/* Define stack top */
__STACK_TOP = ORIGIN(SRAM) + LENGTH(SRAM);
