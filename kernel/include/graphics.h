/*	
 *	graphics.h
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *	Released under the MIT license
 */
 
#ifndef GRAPHICS
#define GRAPHICS

#include <system.h>

unsigned long int getPixCol(int x, int y, uint8_t* ctx);

void drawPixel(int x, int y, int RGB, uint8_t* ctx);
void drawRect(int x, int y, int width, int height, int fill, uint8_t* ctx);
long storePixmap(int x, int y, int width, int height, uint8_t* ctx);
void drawPixmap(int x, int y, int width, int height, long *pixmap, uint8_t* ctx);

void writeBuffer(int x, int y, int width, int height, uint32_t* buffer);
void writePixel(int x, int y, int width, int height, int fill, uint32_t* ctx);
void readBuffer(int x, int y, int width, int height, uint32_t* buffer);

void drawLine(int x0, int x1, int y0, int y1, int fill, uint8_t* ctx);
void drawLineThick(int x0, int x1, int y0, int y1, int fill, int thickness, uint8_t* ctx);




#endif
