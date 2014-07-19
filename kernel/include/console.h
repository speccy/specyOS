/*	
 *	console.h
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *	Released under the MIT license
 */

#ifndef CONSOLE
#define CONSOLE

#include <system.h>
#include <compositor.h>

void init_console(window_t window);
void clear(uint8_t* ctx);
void scroll(uint8_t* ctx);
void console_putch(char c, uint8_t* ctx);
void console_putstr(char *string, uint8_t* ctx);

void console_putstr_dec(uint32_t n, uint8_t* ctx);

//kb handlers
void console_getch(char c);
void console_putch_wrapper(char c);
void shell_handle(char c);

void init_shell(uint8_t* ctx);

#endif
