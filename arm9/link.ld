OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

MEMORY
{
	/* (-1 - 0xFFFF) == 0xFFFF0000; had to express it like this to stop getting unnecessary
	 * veneer functions when trying to call bootrom stuff. Linker bug? */
	BOOTROM  (RX)  : ORIGIN = (-1 - 0xFFFF), LENGTH = 32K
	/* ITCM length is 32K, but the bootrom will only load firms into it up to 14K */
	ITCM     (RWX) : ORIGIN = 0x00008000, LENGTH = 14K
	DTCM     (RW)  : ORIGIN = 0xFFF00000, LENGTH = 16K
}

SECTIONS
{
	.bootrom (NOLOAD) : ALIGN(4)
	{
		*(.bootrom*)
		. = ALIGN(4);
	} >BOOTROM

	.text : ALIGN(4)
	{
		__loaded_start = .;
		__text_start = ABSOLUTE(.);
		*(.text.start)
		*(.text*)
		. = ALIGN(4);
		__text_len = . - __text_start;
	} >ITCM

	.rodata : ALIGN(4)
	{
		__rodata_start = ABSOLUTE(.);
		*(.rodata*)
		. = ALIGN(4);
		__rodata_len = . - __rodata_start;
	} >ITCM

	.data : ALIGN(4)
	{
		__data_start = ABSOLUTE(.);
		*(.data*)
		. = ALIGN(4);
		__data_len = . - __data_start;
		__loaded_end = .;
	} >ITCM

	.bss (NOLOAD) : ALIGN(4)
	{
		__bss_start = ABSOLUTE(.);
		*(.bss*)
		. = ALIGN(4);
		__bss_len = . - __bss_start;
	} >DTCM
}

__loaded_len = __loaded_end - __loaded_start;