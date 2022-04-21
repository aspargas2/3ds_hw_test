OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{
	. = 0x00008000;
	__loaded_start = .;
	.text       : ALIGN(4) { *(.text.start) *(.text*) }
	.rodata     : ALIGN(4) { *(.rodata*) }
	.data       : ALIGN(4) { *(.data*) }
	. = ALIGN(4);
	__loaded_end = .;
	
	. = 0xFFF00000;
	.bss        : ALIGN(4) { __bss_start = .; *(.bss* COMMON); . = ALIGN(4); __bss_end = .;}

	/* (-1 - 0xFFFF) == 0xFFFF0000; had to express it like this to stop getting unnecessary
	 * veneer functions when trying to call bootrom stuff. Linker bug? */
	. = (-1 - 0xFFFF);
	.bootrom (NOLOAD) : ALIGN(4) { *(.bootrom*); }

	. = ALIGN(4);
}

__loaded_size = __loaded_end - __loaded_start;
__bss_size = __bss_end - __bss_start;