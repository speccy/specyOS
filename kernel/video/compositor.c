/*	
 *	compositor.c
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *	Released under the MIT license
 */

#include <system.h>
#include <compositor.h>
#include <graphics.h>
#include <kheap.h>

extern uint16_t scr_height;
extern uint16_t scr_width;
extern uint16_t scr_pitch;
extern uint8_t* scr_ptr;

uint8_t* backbuffer;

long mouseBox;

int x, y;

long int mousePixmap[400] = 
{
  
  0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  0xffffff, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  0xffffff, 0x000000, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  0xffffff, 0x000000, 0x000000, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  0xffffff, 0x000000, 0x000000, 0x000000, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  0xffffff, 0x000000, 0x000000, 0x000000, 0x000000, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  0xffffff, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  0xffffff, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  0xffffff, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  0xffffff, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  
  0xffffff, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  0xffffff, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2,
  0xffffff, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, 0xffffff, -2, -2, -2, -2, -2, -2, -2,
  0xffffff, 0x000000, 0x000000, 0x000000, 0xffffff, 0x000000, 0x000000, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  0xffffff, 0x000000, 0x000000, 0xffffff, -2, 0xffffff, 0x000000, 0x000000, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  0xffffff, 0x000000, 0xffffff, -2, -2, 0xffffff, 0x000000, 0x000000, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  0xffffff, 0xffffff, -2, -2, -2, -2, 0xffffff, 0x000000, 0x000000, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  0xffffff, -2, -2, -2, -2, -2, 0xffffff, 0x000000, 0x000000, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  -2, -2, -2, -2, -2, -2, -2, 0xffffff, 0xffffff, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2
};

void init_compositor(uint8_t* ctx)
{
	//backbuffer = (uint8_t*)kmalloc(scr_pitch * scr_height);
	//memset(backbuffer, 0xFF, scr_pitch * scr_height);
	for(y = 0; y < scr_height; y++)
	{
		for(x = 0; x < scr_width; x++)
		{
			drawPixel(x, y, 0xc41f42, ctx);
		}
	}
	
	drawString(scr_width * 0.5 - 8 * 8, scr_height * 0.1, "muh minimalism tho", 0xffffff, 8, 0, ctx);
	drawString(scr_width * 0.9 - 6 * 8, scr_height * 0.95, "specyOS 0.0.2", 0x222222, 8, 0, ctx);
	//memcpy(scr_ptr, backbuffer, scr_pitch*scr_height);
}

void drawCursor(signed int mouseX,signed int mouseY, int ghostX, int ghostY, uint8_t* ctx)
{
	if (mouseBox == 0){
		mouseBox = storePixmap(ghostX+scr_width/2,ghostY+scr_height/2,20,20);
		drawPixmap(mouseX+scr_width/2, mouseY+scr_height/2, 20, 20, mousePixmap, scr_ptr);
	}
	else {
		drawPixmap(ghostX+scr_width/2,ghostY+scr_height/2,20,20,mouseBox, scr_ptr);
		mouseBox = storePixmap(mouseX+scr_width/2,mouseY+scr_height/2,20,20);
		drawPixmap(mouseX+scr_width/2, mouseY+scr_height/2, 20, 20, mousePixmap, scr_ptr);
	}
	//drawRect(ghostX+scr_width/2, ghostY+scr_height/2, 10, 10, 0xc41f42, ctx);
	//drawRect(mouseX+(scr_width/2), mouseY+(scr_height/2), 10, 10, 0xffffff, ctx);
}

