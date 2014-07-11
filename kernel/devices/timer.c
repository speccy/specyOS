/*	
 *	timer.c - Sets up the PIT
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *  Released under the MIT license
 */
 
#include <timer.h>
#include <system.h>
#include <screen.h>

uint32_t tick = 0;

void timer_callback(registers_t *r)
{
   tick++;
}

void init_timer(uint32_t frequency)
{
   install_irq_handler(32, &timer_callback);
   
   uint32_t divisor = 1193180 / frequency;

   // Send the command byte.
   outb(0x43, 0x36);

   uint8_t l = (uint8_t)(divisor & 0xFF);
   uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
   
} 
