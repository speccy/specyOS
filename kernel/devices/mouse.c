/*	
 *	mouse.c
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *  Released under the MIT license
 */

#include <system.h>
#include <compositor.h>

#define MAX_HANDLERS 32

extern uint8_t* scr_ptr;
uint16_t scr_height;
uint16_t scr_width;
extern uint8_t* backbuffer;

unsigned char mouse_cycle = 0;
signed int mouseX, mouseY =0;
signed char mx, my;
signed int ghostX = 0;
signed int ghostY = 0;
char mouse_bytes[3];

mouse_handler_t mouse_handler_n[MAX_HANDLERS];
uint8_t mouse_handlers = 0;
click_t click;

int i = 0;
int x,y; 

long mouseBox;

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

//Mouse functions

void mouse_handler(struct regs *r)
{
	
	switch(mouse_cycle)
	{
		case 0:
			mouse_bytes[0]=inb(0x60);
			if((mouse_bytes[0] & 0x08) != 0)
				mouse_cycle++;                // checks if in sync
			break;
		case 1:
			mouse_bytes[1]=inb(0x60);
			mouse_cycle++;
			break;
		case 2:
			mouse_bytes[2]=inb(0x60);
			x=mouse_bytes[1];
			y=mouse_bytes[2];
			mouse_cycle=0;
			
			mouseX += x;
			if (mouseX >= scr_width/2) mouseX = scr_width/2-1;
			if (mouseX <= -scr_width/2) mouseX = -scr_width/02;
			
			mouseY -= y;
			if (mouseY >= scr_height/2) mouseY = scr_height/2-1;
			if (mouseY <= -scr_height/2) mouseY = -scr_height/2;
			
			/*if (mouse_bytes[0] & 0x4)
				drawString(10,100,"Middle button", 0x000000, 8, 0, scr_ptr);
			if (mouse_bytes[0] & 0x2)
				drawString(10,200,"Right button", 0x000000, 8, 0, scr_ptr);
			if (mouse_bytes[0] & 0x1)
				drawString(10,300,"Left button", 0x000000, 8, 0, scr_ptr); */
									
			click.x = mouseX;
			click.y = mouseY;
			click.gx = ghostX;
			click.gy = ghostY;
			click.mb = mouse_bytes[0];
			
			for (i = 0; i < mouse_handlers; i++) {
				mouse_handler_t handler = mouse_handler_n[i];
				handler(click);
			}
			
			drawCursor(mouseX, mouseY, ghostX, ghostY, scr_ptr);			

			ghostX = mouseX;
			ghostY = mouseY;
			
			break;
	} 
}

void del_mouse_handler(mouse_handler_t handler)
{
	for (i = 0; i < mouse_handlers; i++) {
		if (mouse_handler_n[i] == handler)
			memmove(&mouse_handler_n[i], &mouse_handler_n[i+1], (sizeof(mouse_handler_n[i]) * (mouse_handlers - i)));
			mouse_handlers--;
	}
}

void add_mouse_handler(mouse_handler_t handler)
{
	if (mouse_handlers != MAX_HANDLERS) {
		mouse_handler_n[mouse_handlers] = handler;
		mouse_handlers++;
	}
}

void mouse_wait(unsigned char type)
{
	unsigned int _time_out=100000;
	if(type==0)
	{
		while(_time_out--) //Data
		{
			if((inb(0x64) & 1)==1)
			{
				return;
			}
		}
		return;
	}
	else
	{
		while(_time_out--) //Signal
		{
			if((inb(0x64) & 2)==0)
			{
				return;
			}
		}
		return;
	}
}

void mouse_write(unsigned char a_write)
{
	//Wait to send a command
	mouse_wait(1);
	//Tell the mouse 
	outb(0x64, 0xD4);
	//Wait 
	mouse_wait(1);
	//Write
	outb(0x60, a_write);
}

unsigned char mouse_read()
{
	mouse_wait(0);
	return inb(0x60);
}

void init_mouse()
{
	mouse_wait(1);
	outb(0x64,0xA8);
	
	mouse_wait(1);
	outb(0x64,0x20);
	
	unsigned char status_byte;
	mouse_wait(0);
	status_byte = (inb(0x60) | 2);
	
	mouse_wait(1);
	outb(0x64, 0x60);
	
	mouse_wait(1);
	outb(0x60, status_byte);
	mouse_write(0xF6);
	mouse_read();
	mouse_write(0xF4);
	mouse_read();
	install_irq_handler(44, &mouse_handler);
}

void drawCursor(signed int mouseX, signed int mouseY, int ghostX, int ghostY, uint8_t* ctx)
{
	if (mouseBox == 0){
		mouseBox = storePixmap(ghostX+scr_width/2,ghostY+scr_height/2,20,20, ctx);
		drawPixmap(mouseX+scr_width/2, mouseY+scr_height/2, 20, 20, mousePixmap, ctx);
	}
	else {
		drawPixmap(ghostX+scr_width/2,ghostY+scr_height/2,20,20,mouseBox, ctx);
		mouseBox = storePixmap(mouseX+scr_width/2,mouseY+scr_height/2,20,20, ctx);
		drawPixmap(mouseX+scr_width/2, mouseY+scr_height/2, 20, 20, mousePixmap, ctx);
	}
}

