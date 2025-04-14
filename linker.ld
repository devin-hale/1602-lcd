ENTRY(Reset_Handler)
MEMORY
{
	flash (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    sram (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}
_estack = ORIGIN(sram) + LENGTH(sram);

SECTIONS {
	.vector :
	{
		. = ALIGN(4);
		KEEP(*(.isr_vector))
		. = ALIGN(4);
	} >flash

    .text : 
	{
		. = ALIGN(4);
		*(.text*)		
		. = ALIGN(4);
    } >flash

	.rodata :
	{
		. = ALIGN(4);
		*(.rodata*)
		. = ALIGN(4);
	} >flash

	.data : 
	{
        . = ALIGN(4);
        _sdata = .;
		*(.first_data)
        *(.data SORT(.data.*))
        _edata = .;
        . = ALIGN(4);
    } > sram AT > flash
	_sidata = LOADADDR(.data);

    .bss : {
        . = ALIGN(4);
        _sbss = .;
        *(.bss SORT(.bss.*) COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >sram

	PROVIDE ( end = . );
	PROVIDE ( _end = . );
}
