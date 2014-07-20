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

int tab = 0;

extern bit;

int gui;

uint8_t* backbuffer;

window_t desktop;
window_t console;

void update_console(char c);
void keySpawnEnter(char c);

int x, y;

void init_compositor(uint8_t* ctx)
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


