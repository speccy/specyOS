/*	
 *	linker.ld
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *  Released under the MIT license
 */

ENTRY(start)
SECTIONS
{

    .text 0x100000 :
    {
        code = .; _code = .; __code = .;
        *(.multiboot)
        *(.text)
        . = ALIGN(4096);
    }
   

    .data :
    {
        data = .; _data = .; __data = .;
        *(.data)
        *(.rodata)
        . = ALIGN(4096);
    }

    .bss :
    {
        bss = .; _bss = .; __bss = .;
        *(.bss)
        . = ALIGN(4096);
    }

    end = .; _end = .; __end = .;
}
