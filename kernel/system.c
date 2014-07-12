/*	
 *	system.c
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *  Released under the MIT license
 */
 
#include <system.h>
#include <screen.h>

void * memset(void * dest, int val, size_t count) 
{
	asm volatile ("cld; rep stosb" : "+c" (count), "+D" (dest) : "a" (val) : "memory");
	return dest;
}

void * memcpy(void * restrict dest, const void * restrict src, size_t count) {
	asm volatile ("cld; rep movsb" : "+c" (count), "+S" (src), "+D" (dest) :: "memory");
	return dest;
}



void outb(unsigned short port, unsigned char data) {
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (data));
}

unsigned char inb(unsigned short port) {
	unsigned char rv;
	asm volatile ("inb %1, %0" : "=a" (rv) : "dN" (port));
	return rv;
}

