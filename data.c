//data.c

#include <stdlib.h>
#include <math.h>
#include "global.h"
#include "ball.h"
#include "data.h"

#define MAX_BALL_NUM	20	

static char defname[32] = {"Yuan"};
static T_BALL *ballArray[MAX_BALL_NUM]={NULL};
static gboolean running = TRUE;
static unsigned int selClr=0;
static int selBall = 0;
static double block = 0.0001;
//static double block = 0;

void data_init()
{
	int i;

	for(i=0; i<MAX_BALL_NUM; i++)
	{
		if (!ballArray[i])
			//ballArray[i] = ball_init(rand()%200, rand()%200, 50, CLR_RGB(rand()%256,rand()%256,rand()%256));
			ballArray[i] = ball_init(rand()%200, rand()%200, rand()%30+5, CLR_RGB(rand()%256,rand()%256,rand()%256));
	}

	selClr = ball_getClr(ballArray[selBall]);
	ball_setName(ballArray[selBall], defname);
	ball_setClr(ballArray[selBall], CLR_RGB(255,0,0));
}

#if 1
static void collision(int ball0, int ball1, int winWidth, int winHeight)
{
	int mode, i;
	float x[2],y[2],r[2];
	T_SPEED s[2], cs[2];
	double xx,yy,rr,cx,px,cy,py,xc,xp,yc,yp,tmp;

	r[0] = ball_getRadius(ballArray[ball0]);
	r[1] = ball_getRadius(ballArray[ball1]);
	ball_getPos(ballArray[ball0], &(x[0]), &(y[0]));
	ball_getPos(ballArray[ball1], &(x[1]), &(y[1]));

	s[0] = ball_getSpeed(ballArray[ball0]);
	s[1] = ball_getSpeed(ballArray[ball1]);

	xx = abs(x[0] - x[1]);
	yy = abs(y[0] - y[1]);
	rr = pow(xx*xx+yy*yy,0.5);

	if ((x[0]-x[1]>0 && y[0]-y[1]>0)
		|| (x[0]-x[1]<0 && y[0]-y[1]<0))
		mode = 1;
	else
		mode = -1;

	//transform to collision coordinate system
	for(i=0; i<2; i++)
	{
		cx = xx*s[i].xSpeed/rr;
		cy = mode*yy*s[i].ySpeed/rr;
		px = yy*s[i].xSpeed/rr;
		py = -mode*xx*s[i].ySpeed/rr;

		cs[i].xSpeed = cx + cy;
		cs[i].ySpeed = px + py;
	}

	//exchange collision speed
	tmp = cs[0].xSpeed;
	cs[0].xSpeed = cs[1].xSpeed;
	cs[1].xSpeed = tmp;

	//transform to normal coordinate system
	for(i=0; i<2; i++)
	{
		xc = xx*cs[i].xSpeed/rr;
		xp = yy*cs[i].ySpeed/rr;
		yc = mode*yy*cs[i].xSpeed/rr;
		yp = -mode*xx*cs[i].ySpeed/rr;

		s[i].xSpeed = xc + xp;
		s[i].ySpeed = yc + yp;
	}

	//set new speed
	ball_setSpeed(ballArray[ball0], s[0]);
	ball_setSpeed(ballArray[ball1], s[1]);

	//To avoid dead loop
	i = 0;
	while(rr<r[0]+r[1] && i<6)
	{
		ball_move(ballArray[ball0], winWidth, winHeight);
		ball_move(ballArray[ball1], winWidth, winHeight);

		r[0] = ball_getRadius(ballArray[ball0]);
		r[1] = ball_getRadius(ballArray[ball1]);
		ball_getPos(ballArray[ball0], &(x[0]), &(y[0]));
		ball_getPos(ballArray[ball1], &(x[1]), &(y[1]));
		
		xx = abs(x[0] - x[1]);
		yy = abs(y[0] - y[1]);
		rr = pow(xx*xx+yy*yy,0.5);

		i++;
	}
}
#else
static void collision(int ball0, int ball1, int winWidth, int winHeight)
{
	int mode, i;
	float x[2],y[2],r[2];
	T_SPEED s[2], cs, ps;
	double xx,yy,rr,cs0,ps0,cs1,ps1,cx0,cx1,px0,px1,cy0,cy1,py0,py1,tmp;

	r[0] = ball_getRadius(ballArray[ball0]);
	r[1] = ball_getRadius(ballArray[ball1]);
	ball_getPos(ballArray[ball0], &(x[0]), &(y[0]));
	ball_getPos(ballArray[ball1], &(x[1]), &(y[1]));

	s[0] = ball_getSpeed(ballArray[ball0]);
	s[1] = ball_getSpeed(ballArray[ball1]);

	xx = abs(x[0] - x[1]);
	yy = abs(y[0] - y[1]);
	//printf("pow(%f,0.5)", xx*xx+yy*yy);
	rr = pow(xx*xx+yy*yy,0.5);

	if ((x[0]-x[1]>0 && y[0]-y[1]>0)
		|| (x[0]-x[1]<0 && y[0]-y[1]<0))
		mode = 0;
	else
		mode = 1;

//transform to collision coordinate system
	//caculate ball0
	cx0 = xx*s[0].xSpeed/rr;
	if (mode)
	{
		px0 = yy*s[0].xSpeed/rr;
		cy0 = -yy*s[0].ySpeed/rr;
	}
	else
	{
		px0 = -yy*s[0].xSpeed/rr;
		cy0 = yy*s[0].ySpeed/rr;
	}
	py0 = xx*s[0].ySpeed/rr;
	cs0 = cx0 + cy0;
	ps0 = px0 + py0;
	//caculate ball1
	cx1 = xx*s[1].xSpeed/rr;
	if (mode)
	{
		px1 = yy*s[1].xSpeed/rr;
		cy1 = -yy*s[1].ySpeed/rr;
	}
	else
	{
		px1 = -yy*s[1].xSpeed/rr;
		cy1 = yy*s[1].ySpeed/rr;
	}
	py1 = xx*s[1].ySpeed/rr;
	cs1 = cx1 + cy1;
	ps1 = px1 + py1;
	//exchange collision speed
	tmp = cs0;
	cs0 = cs1;
	cs1 = tmp;

//transform to normal coordinate system
	//caculate ball0
	if (mode)
	{
		cs.xSpeed = xx*cs0/rr;
		cs.ySpeed = -yy*cs0/rr;
		ps.xSpeed = yy*ps0/rr;
		ps.ySpeed = xx*ps0/rr;
	}
	else
	{
		cs.xSpeed = xx*cs0/rr;
		cs.ySpeed = yy*cs0/rr;
		ps.xSpeed = -yy*ps0/rr;
		ps.ySpeed = xx*ps0/rr;
	}
	s[0].xSpeed = cs.xSpeed + ps.xSpeed;
	s[0].ySpeed = cs.ySpeed + ps.ySpeed;
	ball_setSpeed(ballArray[ball0], s[0]);
	//caculate ball1
	if (mode)
	{
		cs.xSpeed = xx*cs1/rr;
		cs.ySpeed = -yy*cs1/rr;
		ps.xSpeed = yy*ps1/rr;
		ps.ySpeed = xx*ps1/rr;
	}
	else
	{
		cs.xSpeed = xx*cs1/rr;
		cs.ySpeed = yy*cs1/rr;
		ps.xSpeed = -yy*ps1/rr;
		ps.ySpeed = xx*ps1/rr;
	}
	s[1].xSpeed = cs.xSpeed + ps.xSpeed;
	s[1].ySpeed = cs.ySpeed + ps.ySpeed;
	ball_setSpeed(ballArray[ball1], s[1]);

	//To avoid dead loop
	i = 0;
	while(rr<r[0]+r[1] && i<10)
	{
		ball_move(ballArray[ball0], winWidth, winHeight);
		ball_move(ballArray[ball1], winWidth, winHeight);

		r[0] = ball_getRadius(ballArray[ball0]);
		r[1] = ball_getRadius(ballArray[ball1]);
		ball_getPos(ballArray[ball0], &(x[0]), &(y[0]));
		ball_getPos(ballArray[ball1], &(x[1]), &(y[1]));
		
		xx = abs(x[0] - x[1]);
		yy = abs(y[0] - y[1]);
		//printf("2.pow(%f,0.5)", xx*xx+yy*yy);
		rr = pow(xx*xx+yy*yy,0.5);
		//printf("Adjustin collision position,rr=%d,r[0]=%d,r[1]=%d,x[0]=%d,x[1]=%d,y[0]=%d,y[1]=%d.\r\n",(int)rr,r[0],r[1],x[0],x[1],y[0],y[1]);

		i++;
	}

	//printf("Collision[%d,%d]...\r\n",ball0,ball1);
}
#endif

static void scanRelation(int winWidth, int winHeight)
{
	int i,j;
	float x[2],y[2],r[2];
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
				collision(i, j, winWidth, winHeight);
		}
	}
}

gboolean refresh_all(cairo_t *pCR, int winWidth, int winHeight)
{
	int i;

	scanRelation(winWidth, winHeight);
	for(i=0; i<MAX_BALL_NUM; i++)
	{
		if (ballArray[i])
		{
			T_SPEED speed;
			
			speed = ball_getSpeed(ballArray[i]);
			if (speed.xSpeed > 0)
				speed.xSpeed = speed.xSpeed - block*speed.xSpeed*speed.xSpeed;
			else
				speed.xSpeed = speed.xSpeed + block*speed.xSpeed*speed.xSpeed;
			if (speed.ySpeed > 0)
				speed.ySpeed = speed.ySpeed - block*speed.ySpeed*speed.ySpeed;
			else
				speed.ySpeed = speed.ySpeed + block*speed.ySpeed*speed.ySpeed;

			ball_setSpeed(ballArray[i], speed);
			ball_refresh(ballArray[i], pCR, winWidth, winHeight);
		}
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
	if (!ballArray[selBall])
		return FALSE;

	if (event->keyval == 65289)
	{
		ball_setClr(ballArray[selBall], selClr);
		ball_setName(ballArray[selBall], "");

		selBall++;
		selBall = selBall % MAX_BALL_NUM;

		selClr = ball_getClr(ballArray[selBall]);
		ball_setClr(ballArray[selBall], CLR_RGB(255,0,0));
		ball_setName(ballArray[selBall], defname);
	}

	printf("Key pressed[%d]: %d\r\n", selBall, event->keyval);

	//press up
	if (event->keyval == 65362)
	   ball_addSpeed(ballArray[selBall], 0, -1);
	
	//press down
	if (event->keyval == 65364)
	   ball_addSpeed(ballArray[selBall], 0, 1);
	
	//press left
	if (event->keyval == 65361)
	   ball_addSpeed(ballArray[selBall], -1, 0);
	 
	//press right
	if (event->keyval == 65363)
	   ball_addSpeed(ballArray[selBall], 1, 0);

	//press +
	if (event->keyval == 43)
		block = 2*block;

	//press -
	if (event->keyval == 45)
		block = block/2.0;
/*
	if (event->keyval == 46)
		ball_setClr(ballArray[0], CLR_RGB(rand()%256,rand()%256,rand()%256));
*/

	if (event->keyval == 113)
		running = FALSE;
	
	return TRUE; 
}

