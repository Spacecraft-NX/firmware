OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

MEMORY
{
	FLASH : ORIGIN =  0x8003000, LENGTH = 0x1D000
	IRAM  : ORIGIN = 0x20000300, LENGTH =  0x3D00
}

SECTIONS
{
	PROVIDE(__start__ = ORIGIN(FLASH));
    . = __start__;

    .text   : ALIGN(4)
	{
		*(.text.start)
		*(.text*)
		*(.rodata*)
		. = ALIGN(4);
	} > FLASH
    
	.data : ALIGN(4)
	{
		__data_start__ = .;
		*(.data*)
		. = ALIGN(4);
		__data_end__ = .;
	} > IRAM AT> FLASH
    
	.bss : ALIGN(8)
	{
		__bss_start__ = .;
		*(.bss* COMMON)
		. = ALIGN(8);
		__bss_end__ = .;
	} > IRAM

	__data_flash_start__ = LOADADDR(.data);
    __stack_top__    = 0x20002000;
    __stack_bottom__ = 0x20001C00;
}