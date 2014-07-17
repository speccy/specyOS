/*	
 *	graphics.c
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *	Released under the MIT license
 */

#include <system.h>
#include <graphics.h>

extern uint16_t scr_height;
extern uint16_t scr_width;
extern uint16_t scr_pitch;
extern uint8_t* scr_ptr;

void drawPixel(int x, int y, int RGB, uint8_t* ctx)
{
  if (x > scr_width)
      x = scr_width;
  if (y > scr_height)
      y = scr_height;

  int offset = x * (scr_pitch / scr_width) + y * (scr_pitch);

  ctx[offset + 0] = RGB & 0xff;           //BLUE
  ctx[offset + 1] = (RGB >> 8) & 0xff;    //GREEN
  ctx[offset + 2] = (RGB >> 16) & 0xff;   //RED
}

void drawRect(int x, int y, int width, int height, int fill, uint8_t* ctx)
{
  int offset, tmpX = x, tmpY = y;
  
  for(y = tmpY; y < height + tmpY; y++)
    for(x = tmpX; x < width + tmpX; x++)
      drawPixel(x, y, fill, ctx);
}

unsigned long int getPixCol(int x, int y)
{
	int offset = x * (scr_pitch / scr_width) + y * (scr_pitch);
    int ret;

    //ret = ((backbuffer[offset + 0] << 16 | 0xff) | (backbuffer[offset + 1] << 8 | 0xff) | (backbuffer[offset + 2] | 0xff));
	unsigned char a[4] = {scr_ptr[offset+0], scr_ptr[offset+1], scr_ptr[offset+2]};
    ret = *(int*)a;
	return ret;
}

long storePixmap(int x, int y, int width, int height)
{
	int a, b;
	long *map[height][width];
	for(b = y; b < y + height; b++){
		for(a = x; a < x + width; a++){
			map[(b-y)][(a-x)] = getPixCol(a,b);		
		}
	}
	return map;
}

void drawPixmap(int x, int y, int width, int height, long *pixmap, uint8_t* ctx)
{
	int a, b, c;
	int color;
	long offset;
	for (b = y; b < y + height - 1; b++){
		for (a = x; a < x + width - 1; a++){

			offset = (a - x) + (b - y)*width;
			color = pixmap[offset];
			if (color != -2)
				drawPixel(a, b, color, ctx);           //BLUE			
		}
	}
}
