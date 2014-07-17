/*	
 *	graphics.h
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *	Released under the MIT license
 */
 
#ifndef GRAPHICS
#define GRAPHICS

#include <system.h>

unsigned long int getPixCol(int x, int y);

void drawPixel(int x, int y, int RGB, uint8_t* ctx);
void drawRect(int x, int y, int width, int height, int fill, uint8_t* ctx);
long storePixmap(int x, int y, int width, int height);
void drawPixmap(int x, int y, int width, int height, long *pixmap, uint8_t* ctx);


#endif
