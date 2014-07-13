/*	
 *	font.c 
 * 	Copyright (C) 2014, spectrum
 * 	Part of the specyOS kernel
 *  Released under the MIT license
 */

#include <system.h>
#include <8x8.h>
#include <screen.h>

extern uint16_t scr_height;
extern uint16_t scr_width;
extern uint16_t scr_pitch;

char* font;

void drawPixel(int x, int y, int RGB, uint8_t* ctx)
{
  if (x > scr_width)
      x = scr_width;
  if (y > scr_height)
      y = scr_height;
  /*calculates the offset for a specific point on screen*/
  
  int offset = x * (32 / 8) + y * (scr_width * (32 / 8));

  ctx[offset + 0] = RGB & 0xff;           //BLUE
  ctx[offset + 1] = (RGB >> 8) & 0xff;    //GREEN
  ctx[offset + 2] = (RGB >> 16) & 0xff;   //RED

}

void drawChar(int x, int y, unsigned char character, int fill, uint8_t* ctx)
{
	int i, l;
	int j = x;
	int h = y;
	int f;
	
	getfontchar(character);
	
	for (l = 0; l < 16; l++)
	{
		for (i = 8; i > 0; i--)
		{
            j++;
			if ((font[l] & (1<<(i-1))))
			{
				f = fill;
				drawPixel(j, h, f, ctx);
            }
		}
		h++;
		j = x;
	}

}

void drawCharLong(int x, int y, unsigned char character, int fill, uint8_t* ctx)
{
	int i, l, p;
	int j = x;
	int h = y;
	int f;
	
	getfontchar(character);
	for (p = 0; p < 2; p++){
	for (l = 0; l < 8; l++)
	{
		for (i = 8; i > 0; i--)
		{
            j++;
			if ((font[l] & (1<<(i-1))))
			{
				f = fill;
				drawPixel(j, h, f, ctx);
            }
		}
		h+=2;
		j = x;
	}
	j=x;
	h=y+1;
}
}

void drawString(int x, int y, char *string, int color, int fontSize, int tall, uint8_t* ctx)
{
  int length = strlen(string), moveX = fontSize;

  int a;
  for(a = 0; a < length; a++)
  {
	if (tall == 1)
		drawCharLong(x, y, string[a], color, ctx);
    else
		drawChar(x, y, string[a], color, ctx);

    x = x + moveX;
  }
}
/*
void writeChar(int x, int y, unsigned char character, int fill, door_t ctx)
{
	int i, l;
	int j = x;
	int h = y;
	int f;
	
	getfontchar(character);
	
	for (l = 0; l < 8; l++)
	{
		for (i = 8; i > 0; i--)
		{
            j++;
			if ((font[l] & (1<<(i-1))))
			{
				f = fill;
				writePixel(j, h, ctx.width, ctx.height, f, ctx.data);
            }
		}
		h++;
		j = x;
	}

}

void writeCharLong(int x, int y, unsigned char character, int fill, door_t ctx)
{
	int i, l, p;
	int j = x;
	int h = y;
	int f;
	
	getfontchar(character);
	for (p = 0; p < 2; p++){
	for (l = 0; l < 8; l++)
	{
		for (i = 8; i > 0; i--)
		{
            j++;
			if ((font[l] & (1<<(i-1))))
			{
				f = fill;
				writePixel(j, h, ctx.width, ctx.height, f, ctx.data);
            }
		}
		h+=2;
		j = x;
	}
	j=x;
	h=y+1;
}
}

void writeString(int x, int y, char *string, int color, int fontSize, int tall, door_t ctx)
{
  int length = k_strlen(string), moveX = fontSize;

  int a;
  for(a = 0; a < length; a++)
  {
	if (tall == 1)
		writeCharLong(x, y, string[a], color, ctx);
    else
		writeChar(x, y, string[a], color, ctx);

    x = x + moveX;
  }
}

*/
void getfontchar(unsigned char character)
{
    switch(character)
	{
         case 'A':
            font = font_8x8_A;
         	break;
         case 'B':
            font = font_8x8_B;
         	break;
         case 'C':
            font = font_8x8_C;
         	break;
         case 'D':
            font = font_8x8_D;
         	break;
         case 'E':
            font = font_8x8_E;
         	break;
         case 'F':
            font = font_8x8_F;
         	break;
         case 'G':
            font = font_8x8_G;
         	break;
         case 'H':
            font = font_8x8_H;
         	break;
         case 'I':
            font = font_8x8_I;
         	break;
         case 'J':
            font = font_8x8_J;
         	break;
         case 'K':
            font = font_8x8_K;
         	break;
         case 'L':
            font = font_8x8_L;
         	break;
         case 'M':
            font = font_8x8_M;
         	break;
         case 'N':
            font = font_8x8_N;
         	break;
         case 'O':
            font = font_8x8_O;
         	break;
         case 'P':
            font = font_8x8_P;
         	break;
         case 'Q':
            font = font_8x8_Q;
         	break;
         case 'R':
            font = font_8x8_R;
         	break;
         case 'S':
            font = font_8x8_S;
         	break;
         case 'T':
            font = font_8x8_T;
         	break;
         case 'U':
            font = font_8x8_U;
         	break;
         case 'V':
            font = font_8x8_V;
         	break;
         case 'W':
            font = font_8x8_W;
         	break;
         case 'X':
            font = font_8x8_X;
         	break;
         case 'Y':
            font = font_8x8_Y;
         	break;
         case 'Z':
            font = font_8x8_Z;
         	break;
         case 'a':
            font = font_8x8_a;
         	break;
         case 'b':
            font = font_8x8_b;
         	break;
         case 'c':
            font = font_8x8_c;
         	break;
         case 'd':
            font = font_8x8_d;
         	break;
         case 'e':
            font = font_8x8_e;
         	break;
         case 'f':
            font = font_8x8_f;
         	break;
         case 'g':
            font = font_8x8_g;
         	break;
         case 'h':
            font = font_8x8_h;
         	break;
         case 'i':
            font = font_8x8_i;
         	break;
         case 'j':
            font = font_8x8_j;
         	break;
         case 'k':
            font = font_8x8_k;
         	break;
         case 'l':
            font = font_8x8_l;
         	break;
         case 'm':
            font = font_8x8_m;
         	break;
         case 'n':
            font = font_8x8_n;
         	break;
         case 'o':
            font = font_8x8_o;
         	break;
         case 'p':
            font = font_8x8_p;
         	break;
         case 'q':
            font = font_8x8_q;
         	break;
         case 'r':
            font = font_8x8_r;
         	break;
         case 's':
            font = font_8x8_s;
         	break;
         case 't':
            font = font_8x8_t;
         	break;
         case 'u':
            font = font_8x8_u;
         	break;
         case 'v':
            font = font_8x8_v;
         	break;
         case 'w':
            font = font_8x8_w;
         	break;
         case 'x':
            font = font_8x8_x;
         	break;
         case 'y':
            font = font_8x8_y;
         	break;
         case 'z':
            font = font_8x8_z;
         	break;
         case '0':
            font = font_8x8_0;
         	break;
         case '1':              
            font = font_8x8_1;
         	break;
         case '2':
            font = font_8x8_2;
         	break;
         case '3':
            font = font_8x8_3;
         	break;
         case '4':
            font = font_8x8_4;
         	break;
         case '5':
            font = font_8x8_5;
         	break;
         case '6':
            font = font_8x8_6;
         	break;
         case '7':
            font = font_8x8_7;
         	break;
         case '8':
            font = font_8x8_8;
         	break;
         case '9':
            font = font_8x8_9;
         	break;
         case ' ':
            font = font_8x8_space;
         	break;
         case '@':
            font = font_8x8_at;
         	break;
         case '.':
            font = font_8x8_period;
         	break;
         case '[':
            font = font_8x8_left_square_brace;
         	break;
         case ']':
            font = font_8x8_right_square_brace;
         	break;
         case '$':
            font = font_8x8_dollar;
         	break;
         case '!':
            font = font_8x8_exclamation_mark;
         	break;
     /*    case '\n':
         //     g_csr_y+=10;
         //     g_csr_x=0;
         //     g_csr_x-=9;
         //     shell_csr_y+=10;
         //     shell_csr_x=1;
         //     shell_csr_x-=9;
         //   font = font_8x8_space;
         	break;
         case '\b':
          //    if(shell_csr_x - 9 < 1 || g_csr_x - 9 < 1){
          //    }else{
          //         g_csr_x-=9;
          //         shell_csr_x-=9;
              }
            font = font_8x8_bspace;
         	break;
         case '\t':
        //      shell_csr_x+=64;
        //     g_csr_x+=0;
        //  font = font_8x8_bspace;
         	break;
         default:
            font = font_8x8_question_mark;//Question mark for unknown
         	break;*/
    }
}
