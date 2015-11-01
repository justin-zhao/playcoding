//ball source code

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "global.h"
#include "ball.h"

//show ball on surface
void ball_show(T_BALL *pBall, cairo_t *pCR)
{
	if (!pBall || !pCR)
		return;
	
	cairo_set_source_rgb(pCR, CLR_FETCHR(pBall->clr), CLR_FETCHG(pBall->clr), CLR_FETCHB(pBall->clr));
	cairo_set_line_width (pCR, pBall->width);
	
	cairo_arc (pCR, pBall->x, pBall->y, pBall->r, 0, 2 * M_PI);
	cairo_stroke (pCR);
}

//refresh ball status according to speed
void ball_refresh(T_BALL *pBall, cairo_t *pCR, int winWidth, int winHeight)
{
	ball_move(pBall, winWidth, winHeight);
	ball_show(pBall, pCR);
}

//move ball to new position
void ball_move(T_BALL *pBall, int winWidth, int winHeight)
{
	unsigned int oldclr;
	
	if (!pBall)
		return;
	
	pBall->x += pBall->speed.xSpeed;
	if (pBall->x < pBall->r)
	{
		pBall->x = 2*pBall->r - pBall->x;
		pBall->speed.xSpeed = -pBall->speed.xSpeed;
	}

	if (pBall->x > winWidth - pBall->r)
	{
		pBall->x = 2*winWidth - pBall->x - 2*pBall->r;
		pBall->speed.xSpeed = -pBall->speed.xSpeed;
	}

	pBall->y += pBall->speed.ySpeed;
	if (pBall->y < pBall->r)
	{
		pBall->y = 2*pBall->r - pBall->y;
		pBall->speed.ySpeed = -pBall->speed.ySpeed;
	}

	if (pBall->y > winHeight - pBall->r)
	{
		pBall->y = 2*winHeight - pBall->y - 2*pBall->r;
		pBall->speed.ySpeed = -pBall->speed.ySpeed;
	}
}

//add speed
void ball_addSpeed(T_BALL *pBall, float xSpeed, float ySpeed)
{
	pBall->speed.xSpeed += xSpeed;
	pBall->speed.ySpeed += ySpeed;
}

//initialize a ball
T_BALL *ball_init(int x, int y, int r, unsigned int clr)
{
	T_BALL *pBall;
	
	pBall = malloc(sizeof(T_BALL));
	
	pBall->x = x;
	pBall->y = y;
	pBall->r = r;
	pBall->clr = clr;
	pBall->width = 2;
	pBall->speed.xSpeed = 2;
	pBall->speed.ySpeed = 2;
	
	return pBall;
}

//destroy a ball
void ball_destroy(T_BALL *pBall)
{
	free(pBall);
}

//set color for ball
void ball_setClr(T_BALL *pBall, unsigned int clr)
{
	pBall->clr = clr;
}

//set line width for ball
void ball_setWidth(T_BALL *pBall, unsigned int width)
{
	pBall->width = width;
}
