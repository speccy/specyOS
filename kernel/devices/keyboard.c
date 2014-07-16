/*	
 *	keyboard.c
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *	Released under the MIT license
 */
 
#include <system.h>

#define MAX_HANDLERS 32

extern uint8_t* scr_ptr;

int shift_flag, i;

kb_handler_t kb_handler[MAX_HANDLERS];

uint8_t kb_handlers = 0;


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

void keyboard_handler(registers_t *r)
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
			memcpy(layout, shift_keys, sizeof(layout));
		}
		else {
			memcpy(layout, keys, sizeof(layout));
		}
		
		for (i = 0; i < kb_handlers; i++) {
			kb_handler_t handler = kb_handler[i];
			handler(layout[scancode]);
		}
    }
}

void del_kb_handler(kb_handler_t handler)
{
	for (i = 0; i < kb_handlers; i++) {
		if (kb_handler[i] == handler)
			memmove(&kb_handler[i], &kb_handler[i+1], (sizeof(kb_handler[i]) * (kb_handlers - i)));
			kb_handlers--;
	}
}

void add_kb_handler(kb_handler_t handler)
{
	if (kb_handlers != MAX_HANDLERS) {
		kb_handler[kb_handlers] = handler;
		kb_handlers++;
	}
}

void init_kbd()
{
	install_irq_handler(33, &keyboard_handler);
}
