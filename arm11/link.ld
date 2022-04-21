OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(__boot)

MEMORY
{
    /* Virtual address where bootrom will be mappped to avoid veneers */
    BOOTROM  (RX)  : ORIGIN = 0x17000000, LENGTH = 32K
    VRAMLOAD (RWX) : ORIGIN = 0x18100000, LENGTH = 1M
    HIGHRAM  (RWX) : ORIGIN = 0xFFFF0000, LENGTH = 4K
}

SECTIONS
{
    .bootrom (NOLOAD) : ALIGN(4K)
    {
        *(.bootrom*)
        . = ALIGN(4K);
    } >BOOTROM

    .text : ALIGN(4K)
    {
        __text_pa = LOADADDR(.text);
        __text_va = ABSOLUTE(.);
        *(.text*)
        . = ALIGN(4K);
        __text_len = . - __text_va;
    } >VRAMLOAD

    .data : ALIGN(4K)
    {
        __data_pa = LOADADDR(.data);
        __data_va = ABSOLUTE(.);
        *(.data*)
        . = ALIGN(4K);
        __data_len = . - __data_va;
    } >VRAMLOAD

    .rodata : ALIGN(4K)
    {
        __rodata_pa = LOADADDR(.rodata);
        __rodata_va = ABSOLUTE(.);
        *(.rodata*)
        . = ALIGN(4K);
        __rodata_len = . - __rodata_va;
    } >VRAMLOAD

    .shared (NOLOAD) : ALIGN(4K)
    {
        __shared_pa = LOADADDR(.shared);
        __shared_va = ABSOLUTE(.);
        *(.shared*)
        . = ALIGN(4K);
        __shared_len = . - __shared_va;
    } >VRAMLOAD

    .bss (NOLOAD) : ALIGN(4K)
    {
        __bss_pa = LOADADDR(.bss);
        __bss_va = ABSOLUTE(.);
        *(.bss*)
        . = ALIGN(4K);
        __bss_len = . - __bss_va;
    } >VRAMLOAD
}
