/*	
 *	main.c
 *	Copyright (C) 2014, spectrum
 *	Part of the specyOS kernel
 *	Released under the MIT license
 */
 
#include <system.h>
#include <screen.h>
#include <timer.h>
#include <paging.h>
#include <kheap.h>
#include <multiboot.h>
#include <console.h>

multiboot_t *sys_multiboot_info;
uint32_t mem_size;

uint16_t scr_height;
uint16_t scr_width;
uint16_t scr_pitch;

uint8_t* fbb;
uint8_t* scr_ptr = (uint8_t*) 0xD0000000;


void k_main(multiboot_t *mboot_ptr)
{
	sys_multiboot_info = mboot_ptr;
				
	install_gdt();
	install_idt();
	
	mem_size = ((mboot_ptr->mem_lower + mboot_ptr->mem_upper) * 1024); //Bytes
	
	fbb = (uint8_t*)((vbe_info_t *)(mboot_ptr->vbe_mode_info))->physbase;
	scr_pitch = (uint16_t )((vbe_info_t *)(mboot_ptr->vbe_mode_info))->pitch;
	scr_height = (uint16_t )((vbe_info_t *)(mboot_ptr->vbe_mode_info))->Yres;
	scr_width = (uint16_t )((vbe_info_t *)(mboot_ptr->vbe_mode_info))->Xres;
		
	initialise_paging(mem_size, fbb);
	
	//uint8_t* console_ptr = (uint8_t*) (0xD0000000 + ((21) * scr_pitch));

	init_console(scr_ptr);
	console_putstr("specyOS v0.0.2\nType help for a list of commands\n\n", scr_ptr);
	//console_putstr("PhysBase pointer: ", scr_ptr);
	//console_putstr_hex(&fbb, scr_ptr);
	
	//console_putstr("\n\n", scr_ptr);

	init_kbd();
	init_shell(scr_ptr);

	asm volatile ("sti");
}

 
