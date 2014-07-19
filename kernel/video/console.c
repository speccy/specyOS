/*	
 *	console.c
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *	Released under the MIT license
 */

#include <system.h>
#include <console.h>
#include <graphics.h>
#include <compositor.h>

#define FONT_X 8
#define FONT_Y 16

#define CURSOR_POS 14

extern uint32_t mem_size;

extern uint16_t scr_height;
extern uint16_t scr_width;
extern uint16_t scr_pitch;

extern uint8_t* scr_ptr;
extern uint8_t* backbuffer;

window_t win;

uint16_t row, col = 0;
uint16_t row_max;
uint16_t col_max;

int fg_col;
int bg_col;

int prevx, prevy, x, y;

int ghostx, ghosty;

int read_buf;
char* input;

char* prompt = "spex-console:> ";

void init_console(window_t window)
{
	win = window;
	clear(win.data);

	fg_col = 0xffffff;
	bg_col = 0x000000;
	
	row = 0;
	col = 0;
	
	ghostx = 0;
	ghosty = 0;
	
	read_buf = 0;
	
	row_max = window.height / FONT_Y;
	col_max = window.width / FONT_X;
		
	
	for (x = 0; x < FONT_X; x++) 
		writePixel(x, CURSOR_POS, window.width, window.height, bg_col, window.data); // init cursor at 0,0,
		
	add_kb_handler(console_putch_wrapper);
	add_kb_handler(console_getch);
	add_kb_handler(shell_handle);
}

void clear(uint8_t* ctx)
{
	memset((void*) ctx, 0x00, win.height * win.width * (scr_pitch / scr_width));
}

void scroll(uint8_t* ctx)
{
	uint8_t* src_ptr = (uint8_t*) ctx + ((FONT_Y) * (win.width) * (scr_pitch / scr_width));
	uint8_t* dest_ptr = (uint8_t*) ctx;
	unsigned int num_bytes = (win.width * win.height * (scr_pitch / scr_width)) - ((win.width) * (scr_pitch/scr_width)) * (FONT_Y);
	memcpy(dest_ptr,src_ptr, num_bytes);
	
	dest_ptr = (uint8_t*) ctx + (win.width * win.height * (scr_pitch/scr_width)) - (FONT_Y * (win.width) * (scr_pitch/scr_width));
	memset(dest_ptr, 0x00, (FONT_Y * scr_pitch));
}

void console_putch(char c, uint8_t* ctx)
{
	prevx = (col + row * col_max - 1) % col_max;
	prevy = (col == 0 && row != 0) ? (row - 1) : row;
			
	if (c != 0) {
		for (x = 0; x < FONT_X; x++) 
			writePixel(ghostx * FONT_X + x, ghosty * FONT_Y + CURSOR_POS, win.width, win.height, bg_col, ctx); //clears cursor's last position

		if (c == '\n') {
			col = 0;
			row++;
			if (row >= row_max){
				scroll(ctx);
				row--;
			}
		}
		
		else if (c == '\b') {
			if (col + row * col_max > 0) {				
				for (x = 0; x < FONT_X; x++)
					for (y = 0; y < FONT_Y; y++)
						writePixel(prevx * FONT_X + x, prevy * FONT_Y + y, win.width, win.height, bg_col, ctx);
						
				col = prevx;
				row = prevy;
			}
		}
		
		else if (c == '\t') {
			col = (col + 8) & ~(8-1); // increment to point which is divisible by 8
		}
		
		else {
			writeChar(col * FONT_X, row * FONT_Y, c, fg_col, win);
			col++;
		}
		
		if (col >= col_max){
				col = 0;
				row++;
		}
		
		if (row >= row_max){
			scroll(ctx);
			row--;
		}
		
		for (x = 0; x < FONT_X; x++)
			writePixel(col * FONT_X + x, row * FONT_Y + CURSOR_POS, win.width, win.height, fg_col, ctx);
		
		ghostx = col;
		ghosty = row;
	}
}

void console_putstr(char *string, uint8_t* ctx)
{
	int a;
	for(a = 0; a < (int) strlen(string); a++) {
		console_putch(string[a], ctx);
	}
}

void console_putstr_dec(uint32_t n, uint8_t* ctx)
{

    if (n == 0)
    {
        console_putch('0', ctx);
        return;
    }

    int acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc % 10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    console_putstr(c2, ctx);

}

void console_putstr_hex(uint32_t n, uint8_t* ctx)
{
    signed int val;

    console_putstr("0x", ctx);

    char noz = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        val = (n >> i) & 0xF;
        if (val == 0 && noz != 0)
        {
            continue;
        }
    
        if (val >= 0xA)
        {
            noz = 0;
            console_putch(val - 0xA + 'a', ctx);
        }
        else
        {
            noz = 0;
            console_putch(val + '0', ctx);
        }
    }
  
    val = n & 0xF;
    if (val >= 0xA)
    {
        console_putch(val - 0xA + 'a', ctx);
    }
    else
    {
        console_putch(val + '0', ctx);
    }
}

void init_shell(uint8_t* ctx)
{
	console_putstr(prompt, ctx);
}


/* keyboard wrapper functions*/

void shell_handle(char c)
{
	if (c == '\n') {
		if (strcmp("help", input) == 0) {
			console_putstr("Currently supported commands: help, clear, memsize, xinit\n\n", win.data);
			console_putstr(prompt, win.data);
		}
		
		else if (strcmp("clear", input) == 0) {
			clear(win.data);
			row = 0;
			console_putstr(prompt, win.data);
		}
		
		else if (strcmp("memsize", input) == 0) {
			console_putstr("Memory Size: ", win.data);
			console_putstr_dec(mem_size / 1024, win.data);
			console_putstr(" kB\n\n", win.data);

			console_putstr(prompt, win.data);
		}
		
		else if (strcmp("xinit", input) == 0) {
			del_kb_handler(console_putch_wrapper);
			del_kb_handler(console_getch);
			del_kb_handler(shell_handle);

			init_mouse();
			init_compositor(win.data);
		}
		
		else if (strcmp("", input) == 0) {
			console_putstr(prompt, win.data);
		}
		
		else {
			console_putstr(input, win.data);
			console_putstr(": command not found\n\n", win.data);
			console_putstr(prompt, win.data);
		}		
	}
}

void console_putch_wrapper(char c)
{
	if (col <= strlen(prompt))
		col = strlen(prompt);
	 
	console_putch(c, win.data);
}

void console_getch(char c)
{
	if (c == '\n') {
		input[read_buf] = '\0';
		read_buf = 0;
	}
	else {
		input[read_buf] = c;
		read_buf++;
	}
}

