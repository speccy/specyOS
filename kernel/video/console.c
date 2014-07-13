/*	
 *	console.c
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *	Released under the MIT license
 */

#include <system.h>

extern uint16_t scr_height;
extern uint16_t scr_width;
extern uint16_t scr_pitch;

uint16_t row, col = 0;
uint16_t row_max = 800 / 16;
uint16_t col_max = 1280 / 8;

int fg_col = 0xffffff;

void init_console()
{
	row = 0;
	col = 0;
}

void clear(uint8_t* ctx)
{
	memset((void*) ctx, 0x00, scr_height*scr_pitch);
}

void scroll(uint8_t* ctx)
{
	uint8_t* src_ptr = (uint8_t*) ctx + ((16) * scr_pitch);
	uint8_t* dest_ptr = (uint8_t*) ctx;
	unsigned int num_bytes = (scr_pitch * scr_height) - (scr_pitch * (16));
	memcpy(dest_ptr,src_ptr, num_bytes);
	
	dest_ptr = (uint8_t*) ctx + (scr_pitch * scr_height) - (16 * scr_pitch);
	memset(dest_ptr, 0x00, (16 * scr_pitch));
}

void console_putch(char c, uint8_t* ctx)
{
	if (c == '\n') {
		col = 0;
		scroll(ctx);
	}
	else {
		if (col >= col_max){
			col = 0;
			row++;
		}
		if (row >= row_max){
			scroll(ctx);
			row--;
		}
		drawChar(col * 8, row * 16, c, fg_col, ctx);
		col++;
	}
}

void console_putstr(char *string, uint8_t* ctx)
{
	int a;
	for(a = 0; a < (int) strlen(string); a++)
	{
		console_putch(string[a], ctx);
	}
}
