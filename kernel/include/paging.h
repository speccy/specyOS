/*
* paging.h - implementation of JamesM paging
* Copyright (C) 2014, spectrum
* Part of the specyOS kernel
* Released under the MIT license
*/
#ifndef PAGING_H
#define PAGING_H

#include <system.h>



#define PAGE_PRESENT   0x1
#define PAGE_WRITE     0x2
#define PAGE_USER      0x4
#define PAGE_MASK      0xFFFFF000

typedef struct page
{
   uint32_t present : 1; // Page present in memory
   uint32_t rw : 1; // Read-only if clear, readwrite if set
   uint32_t user : 1; // Supervisor level only if clear
   uint32_t accessed : 1; // Has the page been accessed since last refresh?
   uint32_t dirty : 1; // Has the page been written to since last refresh?
   uint32_t unused : 7; // Amalgamation of unused and reserved bits
   uint32_t frame : 20; // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table
{
   page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
   /**
Array of pointers to pagetables.
**/
   page_table_t *tables[1024];
   /**
Array of pointers to the pagetables above, but gives their *physical*
location, for loading into the CR3 register.
**/
   uint32_t tablesPhysical[1024];
   /**
The physical address of tablesPhysical. This comes into play
when we get our kernel heap allocated and the directory
may be in a different location in virtual memory.
**/
   uint32_t physicalAddr;
} page_directory_t;

/**
Sets up the environment, page directories etc and
enables paging.
**/
void init_paging(uint32_t mem_size);

/**
Causes the specified page directory to be loaded into the
CR3 register.
**/
void switch_page_directory(page_directory_t *new);

/**
Retrieves a pointer to the page required.
If make == 1, if the page-table in which this page should
reside isn't created, create it!
**/
page_t *get_page(uint32_t address, int make, page_directory_t *dir);

/**
Handler for page faults.
**/
void page_fault(registers_t *regs);

void dp_frame(page_t *page, uintptr_t address) ;

uint32_t pageMem(uint32_t virt_addr, uint32_t real_addr, uint32_t fb_length);
#define KHEAP_MAX_ADDRESS 0xFFFF000

uint32_t pmm_alloc_page ();
void pmm_free_page (uint32_t p);
void map(unsigned int address);


#endif
