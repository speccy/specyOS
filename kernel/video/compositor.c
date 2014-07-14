#include <system.h>
//#include <font.h>

extern uint16_t scr_height;
extern uint16_t scr_width;
extern uint16_t scr_pitch;

int x, y;

void init_compositor(uint8_t* ctx)
{
	for(y = 0; y < scr_height; y++)
	{
		for(x = 0; x < scr_width; x++)
		{
			drawPixel(x,y,0xc41f42, ctx);
		}
	}
	x = scr_width * 0.5 - 8 * 8;
	y = scr_height * 0.1;
	drawString(x, y, "muh minimalism tho", 0xffffff, 8, 0, ctx);
	x = scr_width * 0.85 - 6 * 8;
	y = scr_height * 0.95;
    drawString(x, y, "specyOS 0.0.2", 0x222222, 8, 0, ctx);
}
