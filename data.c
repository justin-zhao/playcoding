//data.c

#include "global.h"
#include "ball.h"
#include "data.h"
#include <math.h>

static T_BALL *pBall = NULL;

void data_init()
{
	if (!pBall)
		pBall = ball_init(100, 100, 10, CLR_RGB(random()%256,random()%256,random()%256));
}

void refresh_all(cairo_t *pCR, int winWidth, int winHeight)
{
	ball_refresh(pBall, pCR, winWidth, winHeight);
}

void data_destroy()
{
	if (pBall)
		ball_destroy(pBall);	
	pBall = NULL;
}

gboolean on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	printf("Key pressed: %d\r\n", event->keyval);
	
	//press up
	if (event->keyval == 65362)
	   ball_addSpeed(pBall, 0, -1);
	
	//press down
	if (event->keyval == 65364)
	   ball_addSpeed(pBall, 0, 1);
	
	//press left
	if (event->keyval == 65361)
	   ball_addSpeed(pBall, -1, 0);
	 
	//press right
	if (event->keyval == 65363)
	   ball_addSpeed(pBall, 1, 0);
	
	return TRUE; 
}

