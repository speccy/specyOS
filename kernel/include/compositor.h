/*	
 *	compositor.h
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *	Released under the MIT license
 */

#ifndef COMPOSITOR
#define COMPOSITOR

#include <system.h>

void init_compositor(uint8_t* ctx);

void drawCursor(signed int mouseX,signed int mouseY, int ghostX, int ghostY, uint8_t* ctx);

#endif 
