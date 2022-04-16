OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{
	. = 0x00000000;
	__loaded_start = .;
	.text       : ALIGN(4) { *(.text.start) *(.text*) }
	.rodata     : ALIGN(4) { *(.rodata*) }
	.data       : ALIGN(4) { *(.data*) }
	. = ALIGN(4);
	__loaded_end = .;
	
	. = 0xFFF00000;
	.bss        : ALIGN(4) { __bss_start = .; *(.bss* COMMON); . = ALIGN(4); __bss_end = .;}

	. = ALIGN(4);
}

__loaded_size = __loaded_end - __loaded_start;
__bss_size = __bss_end - __bss_start;