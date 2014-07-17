/*	
 *	mouse.c
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *  Released under the MIT license
 */

#include <system.h>

extern uint8_t* scr_ptr;
uint16_t scr_height;
uint16_t scr_width;

signed int mouseX, mouseY =0;
signed char mx, my;
int ghostX = 0;
int ghostY = 0;

int x,y; 
unsigned char mouse_cycle = 0;
char mouse_bytes[3];

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
			
			if (mouse_bytes[0] & 0x4)
				drawString(10,100,"Middle button", 0x000000, 8, 0, scr_ptr);
			if (mouse_bytes[0] & 0x2)
				drawString(10,200,"Right button", 0x000000, 8, 0, scr_ptr);
			if (mouse_bytes[0] & 0x1)
				drawString(10,300,"Left button", 0x000000, 8, 0, scr_ptr);
			
			drawCursor(mouseX,mouseY,ghostX,ghostY, scr_ptr);
			ghostX = mouseX;
			ghostY = mouseY;
			break;
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

