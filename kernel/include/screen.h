/*	
 *	screen.h - stolen from osdev wiki - will be phased out
 * 			   in favour of VESA terminal
 * 
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *  Released under the MIT license
 */
 
#ifndef SCREEN
#define SCREEN

#include <stddef.h>
#include <stdint.h>

enum colors
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

uint8_t combine_colors(enum colors fg, enum colors bg);

uint16_t combine_charcolor(char c, uint8_t color);

size_t strlen(const char* str);

void terminal_initialize();

void terminal_setcolor(uint8_t color);

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);

void terminal_putchar(char c);

void terminal_writestring(const char* data);

void terminal_writestring_hex(uint32_t n);

void terminal_writestring_dec(uint32_t n);



#endif
