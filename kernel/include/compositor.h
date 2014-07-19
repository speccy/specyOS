/*	
 *	compositor.h
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *	Released under the MIT license
 */

#ifndef COMPOSITOR
#define COMPOSITOR

#include <system.h>
#include <graphics.h>


typedef struct window
{
  char *name;
  int id;
  int parentid;
  int x;
  int y;
  int z;
  int width;
  int height;
  uint32_t *data;

} window_t;


/* font drawing functions defined in font.c */
void drawString(int x, int y, char *string, int color, int fontSize, int tall, uint8_t* ctx);
void drawChar(int x, int y, unsigned char character, int fill, uint8_t* ctx);
void writeChar(int x, int y, unsigned char character, int fill, window_t ctx);
void writeString(int x, int y, char *string, int color, int fontSize, int tall, window_t ctx);


void init_compositor(uint8_t* ctx);

void drawCursor(signed int mouseX,signed int mouseY, int ghostX, int ghostY, uint8_t* ctx);
void updateCursor();
void winCursor(int mx, int my, window_t window);


window_t spawnWindow(char* name, int x, int y, int width, int height, int priority, window_t parent);



#endif 
