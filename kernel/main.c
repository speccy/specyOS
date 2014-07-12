/*	
 *	main.c
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *  Released under the MIT license
 */
 
#include <system.h>
#include <screen.h>
#include <timer.h>
#include <paging.h>
#include <kheap.h>
#include <multiboot.h>
#include <console.h>

multiboot_t *sys_multiboot_info;

uint8_t * fb;
uint16_t scr_height;
uint16_t scr_width;
uint16_t scr_pitch;

uint8_t* scr_ptr = (uint8_t*) 0xD0000000;


void k_main(multiboot_t *mboot_ptr)
{
	sys_multiboot_info = mboot_ptr;
	terminal_initialize();
			
	terminal_writestring("Hello, kernel World! also rip cals future\n");
	install_gdt();
	terminal_writestring("GDT Installed\n");
	install_idt();
	terminal_writestring("IDT Installed\n");

	uint32_t memorySize = ((mboot_ptr->mem_lower + mboot_ptr->mem_upper) * 1024); //Bytes
	terminal_writestring("Memory Size: ");
	terminal_writestring_dec(memorySize/(1024));
	terminal_writestring(" kB\n");
	
	uint32_t fbb = (uint32_t)((vbe_info_t *)(mboot_ptr->vbe_mode_info))->physbase;
	scr_pitch = (uint16_t )((vbe_info_t *)(mboot_ptr->vbe_mode_info))->pitch;
	scr_height = (uint16_t )((vbe_info_t *)(mboot_ptr->vbe_mode_info))->Yres;
	scr_width = (uint16_t )((vbe_info_t *)(mboot_ptr->vbe_mode_info))->Xres;
		
	initialise_paging(memorySize, fbb);

	int x;
	int y;
	
	for(y = 0; y < scr_height; y++)
	{
		for(x = 0; x < scr_width; x++)
		{
			//drawPixel(x,y,0xc41f42, scr_ptr);
			drawPixel(x,y,0x000000, scr_ptr);
		}
	}
	
	uint8_t* console_ptr = (uint8_t*) (0xD0000000 + ((21) * scr_pitch));

	drawString(5, 5, "specyOS v0.0.2", 0x33ADFF, 8, 0, scr_ptr);
	//drawString(5+8*8, 5, "", 0xFFFFFF, 8, 0, scr_ptr);

	drawString(5+10, 5+32, "Installed Global Descriptor Tables", 0xffffff, 8, 0, scr_ptr);
	drawString(5+10, 5+48, "Installed Interrupt Descriptor Tables", 0xffffff, 8, 0, scr_ptr);
	drawString(5+10, 5+64, "Initialised Paging", 0xffffff, 8, 0, scr_ptr);
	drawString(5+10, 800-16-5, "Teston", 0xffffff, 8, 0, scr_ptr);

	scroll(scr_ptr);
	asm volatile ("sti");

}

 
