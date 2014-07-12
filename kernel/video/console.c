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

void clear(uint8_t* ctx)
{
	memset((void*) ctx, 0x00, scr_height*scr_pitch);
}

void scroll(uint8_t* ctx)
{
	uint8_t* src_ptr = (uint8_t*) ctx + ((21) * scr_pitch);;
	uint8_t* dest_ptr = (uint8_t*) ctx;
	unsigned int num_bytes = (scr_pitch * scr_height) - (scr_pitch * (21));
	memcpy(dest_ptr,src_ptr, num_bytes);
	
	dest_ptr = (uint8_t*) ctx + (scr_pitch * scr_height) - (21 * scr_pitch);
	memset(dest_ptr, 0x00, (21 * scr_pitch));
}
