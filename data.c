//data.c

#include "global.h"
#include "ball.h"
#include "data.h"
#include <math.h>

#define MAX_BALL_NUM	3

static T_BALL *ballArray[MAX_BALL_NUM]={NULL};
static gboolean running = TRUE;

static void collision(int ball0, int ball1)
{
	int x[2],y[2];
	T_SPEED s[2];

	ball_getPos(ballArray[ball0], &(x[0]), &(y[0]));
	ball_getPos(ballArray[ball1], &(x[1]), &(y[1]));

	s[0] = ball_getSpeed(ballArray[ball0]);
	s[1] = ball_getSpeed(ballArray[ball1]);

	printf("Collision[%d,%d]...\r\n",ball0,ball1);
}

static void scanRelation()
{
	int i,j;
	int x[2],y[2],r[2];
	long xl,yl,rl;

	for(i=0; i<MAX_BALL_NUM-1; i++)
	{
		for(j=i+1; j<MAX_BALL_NUM; j++)
		{
			r[0] = ball_getRadius(ballArray[i]);
			r[1] = ball_getRadius(ballArray[j]);
			ball_getPos(ballArray[i], &(x[0]), &(y[0]));
			ball_getPos(ballArray[j], &(x[1]), &(y[1]));

			xl = x[0] - x[1];
			yl = y[0] - y[1];
			rl = r[0] + r[1];
			//printf("xl=%ld,yl=%ld,rl=%ld\r\n",xl,yl,rl);

			if (xl*xl+yl*yl < rl*rl)
				collision(i,j);
		}
	}
}

void data_init()
{
	int i;

	for(i=0; i<MAX_BALL_NUM; i++)
	{
		if (!ballArray[i])
			ballArray[i] = ball_init(rand()%200, rand()%200, 10, CLR_RGB(rand()%256,rand()%256,rand()%256));
	}
}

gboolean refresh_all(cairo_t *pCR, int winWidth, int winHeight)
{
	int i;

	scanRelation();
	for(i=0; i<MAX_BALL_NUM; i++)
	{
		if (ballArray[i])
			ball_refresh(ballArray[i], pCR, winWidth, winHeight);
	}

	return running;
}

void data_destroy()
{
	int i;

	for(i=0; i<MAX_BALL_NUM; i++)
	{
		if (ballArray[i])
		{
			ball_destroy(ballArray[i]);	
			ballArray[i] = NULL;
		}
	}
}

gboolean on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	printf("Key pressed: %d\r\n", event->keyval);

	if (!ballArray[0])
		return FALSE;
	
	//press up
	if (event->keyval == 65362)
	   ball_addSpeed(ballArray[0], 0, -1);
	
	//press down
	if (event->keyval == 65364)
	   ball_addSpeed(ballArray[0], 0, 1);
	
	//press left
	if (event->keyval == 65361)
	   ball_addSpeed(ballArray[0], -1, 0);
	 
	//press right
	if (event->keyval == 65363)
	   ball_addSpeed(ballArray[0], 1, 0);

	if (event->keyval == 46)
		ball_setClr(ballArray[0], CLR_RGB(rand()%256,rand()%256,rand()%256));

	if (event->keyval == 113)
		running = FALSE;
	
	return TRUE; 
}

