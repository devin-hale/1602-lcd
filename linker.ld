ENTRY(_reset)
MEMORY
{
	flash (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    sram (wx) : ORIGIN = 0x20000000, LENGTH = 128K
}
_Min_Heap_Size = 0x400;
_Min_Stack_Size = 0x900;
_estack = ORIGIN(sram) + LENGTH(sram);

SECTIONS {
	.vector_table :
	{
		. = ALIGN(4);
		KEEP(*(.vector_table))
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
	_si_data = LOADADDR(.data);

    .bss : {
        . = ALIGN(4);
        _sbss = .;
        *(.bss SORT(.bss.*) COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >sram

	.dynamic :
    {
        . = ALIGN(8);
		. = . + _Min_Heap_Size;
		. = . + _Min_Stack_Size;
		. = ALIGN(8);
    } >sram
}
