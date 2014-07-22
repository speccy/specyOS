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

uint32_t* win_read;

extern mouseX;
extern mouseY;
extern ghostX;
extern ghostY;

int ghostwX, ghostwY;

int tab = 0;

extern bit;

int gui;

uint8_t* backbuffer;

window_t desktop;
window_t console;

window_t windows[MAX_WINDOWS];
uint8_t window_number = 0;

void update_console(char c);
void keySpawnEnter(char c);

int x, y, i;

int bitcheck = 0;

int thickness;

void init_compositor_old(uint8_t* ctx)
{
	backbuffer = (uint8_t*)kmalloc(scr_pitch * scr_height);
	//memset(backbuffer, 0xFF, scr_pitch * scr_height);
	
	gui = 1;
	
	ctx = backbuffer;
	
	static window_t window;
	window.name = "Desktop";
	window.x = 0;
	window.y = 0;
	window.z = 0;
	window.width = scr_width;
	window.height = scr_height;
	window.id = 0;
	window.parentid = 0;
	
	window.data = (uint32_t*)kmalloc(scr_pitch * scr_height);
	
	drawRect(window.x, window.y, window.width, window.height , 0xc41f42, window.data);
	
	drawString(window.width * 0.5 - 8 * 8, window.height * 0.1, "muh minimalism tho", 0xffffff, 8, 0, window.data);
	drawString(window.width * 0.9 - 6 * 8, window.height * 0.95, "specyOS 0.0.2", 0x222222, 8, 0, window.data);
	
	writeBuffer(0, 0, scr_width, scr_height, (uint32_t*)window.data);
	
	desktop = window;
	
	win_read = (uint32_t*)kmalloc(640*340*4);
	readBuffer(200, 200, 600, 340, win_read);
	
	add_kb_handler(keySpawnEnter);

	/*
	console = spawnWindow("console", 200, 200, 600, 340, 1, desktop);
	init_console(console);
	add_kb_handler(update_console);
	void update_console(char c)
	{
		writeBuffer(console.x, console.y, console.width, console.height, console.data);
	}*/
	
	//init_shell(console.data);

	//memcpy(scr_ptr, backbuffer, scr_pitch*scr_height);
}

window_t spawnWindow(char* name, int x, int y, int width, int height, int priority, window_t parent)
{
	static window_t window;
	window.name = name;
	window.x = x;
	window.y = y;
	window.z = priority;
	window.width = width;
	window.height = height;
	window.parentid = parent.id;
	window.id = 1;
	window.data = (uint32_t*)kmalloc((width * height) * (4));
	memset(window.data, 0x00, (width * height) * (4));

	//drawRect(x-2, y-2, width+4, height+4, 0xC98F9C, scr_ptr);
	writeBuffer(x, y, width, height, (uint32_t*)window.data);
	
	windows[window_number] = window;
	window_number++;
	
	//memcpy(scr_ptr, backbuffer, scr_pitch*scr_height);
	return window;
}

void update_console(char c)
{
	writeBuffer(console.x, console.y, console.width, console.height, console.data);
}

void keySpawnEnter(char c)
{
	if (c == ' ' && tab != 1 ) {
		console = spawnWindow("console", 200, 200, 540, 300, 1, desktop);
		add_mouse_handler(window_movement_handler);
		init_console(console);
		init_shell(console.data);
		add_kb_handler(update_console);
		void update_console(char c)
		{
			writeBuffer(console.x, console.y, console.width, console.height, console.data);
		}
		tab = 1;
	}
}

void winCursor(int mx, int my, window_t window)
{
	window.x += mx;
	window.y -= my;
}

void window_movement_handler(click_t click)
{
	for(i=0; i<window_number; i++) {
		if (
			click.mb & 0x1 && 
			click.x > windows[i].x-scr_width/2 && 
			click.x < (windows[i].x+windows[i].width)-scr_width/2 && 
			click.y > windows[i].y-scr_height/2 && 
			click.y < (windows[i].y+windows[i].height)-scr_height/2 &&
			bitcheck % 2 == 0
			) { 		
			ghostwX = windows[i].x;
			ghostwY = windows[i].y;
			
			drawBorder(windows[i], 0xc41f42, thickness, backbuffer);
			drawBorder(windows[i], 0xc41f42, thickness, scr_ptr);
			
			windows[i].x += x*2;
			windows[i].y -= y*2;
			
			if (windows[i].x - ghostwX > 0) {
				writeBuffer(ghostwX, ghostwY, (windows[i].x - ghostwX), windows[i].height + thickness, backbuffer);
			}
			if (windows[i].x - ghostwX < 0) {
				writeBuffer(windows[i].x+windows[i].width, ghostwY, (ghostwX - windows[i].x), windows[i].height, backbuffer);
			}
			if (windows[i].y - ghostwY > 0) {
				writeBuffer(ghostwX, ghostwY, windows[i].width, (windows[i].y - ghostwY), backbuffer);					
			}
			if (windows[i].y - ghostwY < 0) {
				writeBuffer(windows[i].x, windows[i].y+windows[i].height, windows[i].width - (windows[i].x - ghostwX), (ghostwY - windows[i].y), backbuffer);
			}
			
			drawBorder(windows[i], 0xC98F9C, thickness, backbuffer);
			drawBorder(windows[i], 0xC98F9C, thickness, scr_ptr);

			bitcheck = 0;
			
			writeBuffer(windows[i].x, windows[i].y, windows[i].width, windows[i].height, (uint32_t*)windows[i].data);
		} 
	}
	bitcheck++;
}



/////////////////////////////////// REWRITE /////////////////////////////////// 

void init_compositor(uint8_t* ctx)
{
	backbuffer = (uint8_t*)kmalloc(scr_pitch * scr_height); 
	
	/* sets thickness of window borders */
	thickness = 3;
	
	/* creates a new window to act as the desktop */
	static window_t window;
	window.name = "Desktop";
	window.x = 0;
	window.y = 0;
	window.z = 0;
	window.width = scr_width;
	window.height = scr_height;
	window.id = 0;
	window.parentid = 0;
	
	window.data = (uint32_t*)kmalloc(scr_pitch * scr_height);
	
	/* draws the background */
	drawRect(window.x, window.y, window.width, window.height , 0xc41f42, window.data);
	
	/* draws strings on top of the background */
	drawString(window.width * 0.5 - 8 * 8, window.height * 0.1, "muh minimalism tho", 0xffffff, 8, 0, window.data);
	drawString(window.width * 0.9 - 6 * 8, window.height * 0.95, "specyOS 0.0.2", 0x222222, 8, 0, window.data);
	
	/* writes to desktop to screen and backbuffer */
	writeBuffer(0, 0, scr_width, scr_height, (uint32_t*)window.data);
	
	/* draws four test windows */
	window_t one = spawnWindow("one", 50, 100, 400, 250, 1, desktop);
	window_t two = spawnWindow("two", 800, 300, 400, 250, 1, desktop);
	window_t three = spawnWindow("three", 400, 500, 150, 100, 1, desktop);
	window_t four = spawnWindow("four", 500, 200, 200, 100, 1, desktop);
	drawBorder(three, 0xC98F9C, thickness, scr_ptr);

	/* adds mouse handler for dragging */
	add_mouse_handler(window_movement_handler);
}

void drawBorder(window_t window, int fill, char thickness, uint8_t* ctx)
{
	if (thickness != 0) {
		drawRect(window.x, window.y - thickness, window.width + thickness, thickness, fill, ctx); // draws top border
		drawRect(window.x + window.width, window.y, thickness, window.height + thickness, fill, ctx); // draws right border
		drawRect(window.x - thickness, window.y + window.height, window.width + thickness, thickness, fill, ctx); // draws bottom border
		drawRect(window.x - thickness, window.y - thickness, thickness, window.height + thickness, fill, ctx); // draws left border
	}
}


