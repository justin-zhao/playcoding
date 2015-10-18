#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vga.h>
#include <vgagl.h>
 
void set_font(color)
{
	void *font = malloc(256 * 8 * 8 * BYTESPERPIXEL);
    gl_expandfont(8, 8, color, gl_font8x8, font);
    gl_setfont(8, 8, font);
}

typedef struct  _CIRCLE {
	int x, y, r;
	int color;
} T_CIRCLE;

void circle_show(T_CIRCLE *pCircle)
{
	gl_circle(pCircle->x, pCircle->y, pCircle->r, pCircle->color);
}

void circle_move(T_CIRCLE *pCircle, int x, int y)
{
	int color;

	if (pCircle->x == x && pCircle->y == y)
		return;

	color = pCircle->color;
	pCircle->color = 0;	
	circle_show(pCircle);

	pCircle->x = x;
	pCircle->y = y;
	pCircle->color = color;	
	circle_show(pCircle);
}

T_CIRCLE *circle_init(int x, int y, int r, int clr)
{
	T_CIRCLE *pRet;

	pRet = (T_CIRCLE *)malloc(sizeof(T_CIRCLE));
	pRet->x = x;
	pRet->y = y;
	pRet->r = r;
	pRet->color = clr;

	circle_show(pRet);

	return pRet;
}

void circle_destroy(T_CIRCLE *pCircle)
{
	free(pCircle);
}

int main(void)
{
	char key, dbg[81];
	int stop=0;
	int times;
	int x, y;
	T_CIRCLE *pCC;

    vga_init();
    vga_setmode(G1024x768x256);
	gl_setcontextvga(G1024x768x256);
	vga_clear();

    vga_setcolor(4);
	vga_drawpixel(20,10);
	vga_drawline(10,10,100,100);
	set_font(5);
	gl_write(20,20, "Hello, world!");
	gl_circle(30,60, 20, 0);

	
	x = 50;
	y = 80;
	pCC = circle_init(x, y, 10, 5);
	times = 0;
	while(!stop)
	{
		key = vga_getkey();
		switch(key)
		{
		case 'q':
		case 'Q':
			stop = 1;
			break;
		case 68:
			x--;
			break;
		case 65:
			y--;
			break;
		case 67:
			x++;
			break;
		case 66:
			y++;
			break;
		default:
			break;
		}

		snprintf(dbg, 80, "key=%d, stop=%d, x=%d, y=%d", key, stop, x, y);
//		gl_write(10,20, dbg);

		circle_move(pCC, x, y);
//		usleep(100000);
		times++;
	}

	circle_destroy(pCC);
    vga_setmode(TEXT);

    return EXIT_SUCCESS;
}
