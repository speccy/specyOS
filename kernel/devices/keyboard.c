/*	
 *	keyboard.c
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *	Released under the MIT license
 */
 
#include <system.h>
#include <console.h>

extern uint8_t* scr_ptr;

int shift_flag;

unsigned char layout[128];

unsigned char keys[128] =
{
	0,'\033','1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
	'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,'a','s',
	'd','f','g','h','j','k','l',';','\'','`',0,'\\','z','x','c','v',
	'b','n','m',',','.','/',0,'*',0,' ',0,0,0,0,0,0,
	0,0,0,0,0,0,0,'7','8','9','-','4','5','6','+','1',
	'2','3','0','.',0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

/* shift */
unsigned char shift_keys[128] =
{
	0,0,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t',
	'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,'A','S',
	'D','F','G','H','J','K','L',':','\"','~',0,'|','Z','X','C','V',
	'B','N','M','<','>','?',0,0,0,' ',0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

void keyboard_handler(struct regs *r)
{
    unsigned char scancode;

    scancode = inb(0x60);

    if (scancode & 0x80) {
		if(scancode - 0x80 == 42 || scancode - 0x80 == 54)
			shift_flag = 0;					
    }
    else {
		if(scancode == 42 || scancode == 54) {
	      shift_flag = 1;
	      return;
	    }
	    
	    if (shift_flag == 1) {
			console_putch_wrapper(shift_keys[scancode], scr_ptr);
			console_getch(shift_keys[scancode]);
			shell_handle(shift_keys[scancode], scr_ptr);
		}
		else {
			console_putch_wrapper(keys[scancode], scr_ptr);
			console_getch(keys[scancode]);
			shell_handle(shift_keys[scancode], scr_ptr);
		}
    }
}

void init_kbd()
{
	install_irq_handler(33, &keyboard_handler);
}
