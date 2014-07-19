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

extern uint8_t* backbuffer;

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

unsigned long int getPixCol(int x, int y, uint8_t* ctx)
{
	int offset = x * (scr_pitch / scr_width) + y * (scr_pitch);
    int ret;

    //ret = ((backbuffer[offset + 0] << 16 | 0xff) | (backbuffer[offset + 1] << 8 | 0xff) | (backbuffer[offset + 2] | 0xff));
	unsigned char a[4] = {ctx[offset+0], ctx[offset+1], ctx[offset+2]};
    ret = *(int*)a;
	return ret;
}

long storePixmap(int x, int y, int width, int height, uint8_t* ctx)
{
	int a, b;
	long *map[height][width];
	for(b = y; b < y + height; b++){
		for(a = x; a < x + width; a++){
			map[(b - y)][(a - x)] = getPixCol(a, b, ctx);		
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

void writeBuffer(int x, int y, int width, int height, uint32_t* buffer)
{
	/*int m,n;
	for (m = y; m < y + height; m++) {
		for (n = x; n < x + width; n++) {
			int offset = n * (scr_pitch / scr_width) + m * (scr_pitch);
			int fill = buffer[((m - y) * width) + (n - x)];
			if(fill != -2)
			{
				scr_ptr[offset + 0] = fill & 0xff;           //BLUE
				scr_ptr[offset + 1] = (fill >> 8) & 0xff;    //GREEN
				scr_ptr[offset + 2] = (fill >> 16) & 0xff;   //RED
			}
		}
	}*/
	
	int i;
    int offset = (x) * (scr_pitch / scr_width) + (y) * (scr_pitch);
    for (i = 0; i < height; i++) {
		memcpy(offset + scr_ptr + scr_pitch * i, buffer + width * i, width * (scr_pitch / scr_width));
	}
}

void readBuffer(int x, int y, int width, int height, uint32_t* buffer)
{
	int i;
	int offset = (x) * (scr_pitch / scr_width) + (y) * (scr_pitch);
	for (i = 0; i < height; i++) {
		memcpy(buffer + width * i, offset + scr_ptr + scr_pitch * i, width * (scr_pitch / scr_width));
	}
}

void writePixel(int x, int y, int width, int height, int fill, uint32_t* ctx)
{
	int offset = x + y * width;
	if (offset != -2)
		ctx[offset] = fill;
}
