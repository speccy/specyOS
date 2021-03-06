/*	
 *	paging.c - implementation of JamesM paging
 *	Copyright (C) 2014, spectrum
 *	Part of the specyOS kernel
 *	Released under the MIT license
 */
 
#include <system.h>
#include <paging.h>
#include <kheap.h>
#include <console.h>

// The kernel's page directory
page_directory_t *kernel_directory=0;

// The current page directory;
page_directory_t *current_directory=0;

extern uint32_t fb;
extern uint16_t scr_height;
extern uint16_t scr_pitch;

extern uint8_t* fbb;
extern uint8_t* scr_ptr;

// A bitset of frames - used or free.
uint32_t *frames;
uint32_t nframes;

// Defined in kheap.c
extern uint32_t placement_address;

// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

/*
 * 	The following functions implement a bitset
 */
 
// Static function to set a bit in the frames bitset
static void set_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set.
static uint32_t test_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   return (frames[idx] & (0x1 << off));
}

// Static function to find the first free frame.
static uint32_t first_frame()
{
   uint32_t i, j;
   for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
   {
       if (frames[i] != 0xFFFFFFFF) // nothing free, exit early.
       {
           // at least one bit is free here.
           for (j = 0; j < 32; j++)
           {
               uint32_t toTest = 0x1 << j;
               if ( !(frames[i]&toTest) )
               {
                   return i*4*8+j;
               }
           }
       }
   }
} 

/*
 * 	The following functions manage physical memory
 */
 
// Function to allocate a frame.
void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
   if (page->frame != 0)
   {
       return; // Frame was already allocated, return straight away.
   }
   else
   {
       uint32_t idx = first_frame(); // idx is now the index of the first free frame.
       if (idx == (uint32_t)-1)
       {
           // PANIC is just a macro that prints a message to the screen then hits an infinite loop.
           //PANIC("No free frames!");
           for (;;);
       }
       set_frame(idx*0x1000); // this frame is now ours!
       page->present = 1; // Mark it as present.
       page->rw = (is_writeable)?1:0; // Should the page be writeable?
       page->user = (is_kernel)?0:1; // Should the page be user-mode?
       page->frame = idx;
   }
}

// direct paging
void dp_frame(page_t *page, uint32_t address) 
{
	page->present = 1;
	page->rw = 1;
	page->user = 0;
	page->frame = address / 0x1000;
	
	if (address < nframes * 4 * 0x400) {
		set_frame(address);
	}
}

// Function to deallocate a frame.
void free_frame(page_t *page)
{
	uint32_t frame;
	if (!(frame=page->frame))
	{
	   return; 
	}
	else
	{
	   clear_frame(frame); 
	   page->frame = 0x0; 
	}
} 

void init_paging(uint32_t mem_size)
{
	uint32_t mem_end_page = mem_size;
	uint32_t cr0;
	
	nframes = mem_end_page / 0x1000;
	frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
	
	memset(frames, 0, INDEX_FROM_BIT(nframes));
	
	// create page directory
	kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
	memset(kernel_directory, 0, sizeof(page_directory_t));
	current_directory = kernel_directory;
	
	// identity map first 4MB of memory
	int i = 0;
	while (i < placement_address) {
		alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000;
	}
	
	// map the framebuffer 
	int n;
	for (n = fbb; n <= fbb + 0xFF0000; n += 0x1000) {
		dp_frame(get_page(n, 1, kernel_directory), n);
	}
	
	// install fault handler
	install_irq_handler(14, &page_fault);
	
	// switch directory
	switch_page_directory(kernel_directory);
	
	// enable paging
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; // Enable paging!
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void switch_page_directory(page_directory_t *dir)
{
	current_directory = dir;
	asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir)
{
	// Turn the address into an index.
	address /= 0x1000;
	// Find the page table containing this address.
	uint32_t table_idx = address / 1024;
	if (dir->tables[table_idx]) // If this table is already assigned
	{
	   return &dir->tables[table_idx]->pages[address%1024];
	}
	else if(make)
	{
	   uint32_t tmp;
	   dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
	   memset(dir->tables[table_idx], 0, 0x1000);
	   dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
	   return &dir->tables[table_idx]->pages[address%1024];
	}
	else
	{
	   return 0;
	}
} 

void page_fault(registers_t * regs)
{
   // faulting address is stored in CR2
   uint32_t faulting_address;
   asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

   int present   = !(regs->err_code & 0x1); // Page not present
   int rw = regs->err_code & 0x2;           // Write operation?
   int us = regs->err_code & 0x4;           // Processor was in user-mode?
   int reserved = regs->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
   int id = regs->err_code & 0x10;          // Caused by an instruction fetch?

   // Output an error message.
   console_putstr("Page fault! ( ", scr_ptr);
   if (present) {console_putstr("present ", scr_ptr);}
   if (rw) {console_putstr("read-only ", scr_ptr);}
   if (us) {console_putstr("user-mode ", scr_ptr);}
   if (reserved) {console_putstr("reserved ", scr_ptr);}
   console_putstr(") at ", scr_ptr);
   console_putstr_hex(faulting_address, scr_ptr);
   console_putstr("\n", scr_ptr);
   //PANIC("Page fault");
   for(;;);
} 

/*	RIP IN PEACE
uint32_t pageMem(uint32_t virt_addr, uint32_t real_addr, uint32_t fb_length)
{
	int i = 0;
	uint32_t fb_addr;
	
	// Align framebuffer length to page boundaries
	fb_length += 0x1000;
	fb_length &= 0x0FFFF000;
	
	// Map enough framebuffer
	for(i = virt_addr; i < virt_addr + fb_length; i += 0x1000) {
		page_t* page = get_page(i, 1, kernel_directory);
		
		fb_addr = (i & 0x0FFFF000) + real_addr;
		
		page->present = 1;
		page->rw = 1;
		page->user = 1;
		page->frame = fb_addr >> 12;
	}
	return virt_addr;
}*/

