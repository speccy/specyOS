/*	
 *	console.h
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *	Released under the MIT license
 */

#ifndef CONSOLE
#define CONSOLE

#include <system.h>

void init_console();
void clear(uint8_t* ctx);
void scroll(uint8_t* ctx);
void console_putch(char c, uint8_t* ctx);
void console_putstr(char *string, uint8_t* ctx);




#endif
