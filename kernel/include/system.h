/*	
 *	system.h 
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *  Released under the MIT license
 */
 
#ifndef SYSTEM
#define SYSTEM

#include <stddef.h>
#include <stdint.h>
#include <screen.h>
#include <timer.h>
//#include <compositor.h>

void* memset(void* dest, int val, size_t count);
void* memcpy(void* restrict dest, const void* restrict src, size_t count);
void* memmove(void* dest, const void* src, size_t count);

#define ASSERT(b) ((b) ? (void)0 : strcmp("a","b"));
#define abs(x) ((x)<0 ? -(x) : (x))

void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);

int strcmp(char *string1, char *string2);

typedef struct gdt_descriptor {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt_ptr_struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

typedef struct idt_descriptor {
	uint16_t base_low;
	uint16_t sel;
	uint8_t zero;
	uint8_t flags;
	uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_ptr_struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idt_ptr_t;

typedef struct registers
{
   uint32_t gs, fs, es, ds;                  // Data segment selector
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
   uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t; 

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
extern void isr255();

typedef void (*irq_handler_t)(registers_t*);
//typedef void (*isr_t)(registers_t);

void install_gdt();
void install_idt();
void gdt_set_gate(size_t num, uint32_t base, uint32_t limit, uint8_t flags, uint8_t gran);
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t	flags);

void install_irq_handler(size_t n, irq_handler_t handler);

// keyboard
typedef void (*kb_handler_t)(uint8_t layout);
void init_kbd();
void add_kb_handler(kb_handler_t handler);
void del_kb_handler(kb_handler_t handler);


// mouse
typedef struct click {
	signed int x;
	signed int y;
	signed int gx;
	signed int gy;
	char mb;
} click_t;

typedef void (*mouse_handler_t)(click_t click);
void init_mouse();
void add_mouse_handler(mouse_handler_t handler);
void del_mouse_handler(mouse_handler_t handler);





#endif
