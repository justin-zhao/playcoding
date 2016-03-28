//ball.h
#include <gtk/gtk.h>

typedef struct _BALL{
	float x, y, r;	//x position, y position, radious
	unsigned int width;		//line width
	T_SPEED speed;	//move speed
	unsigned int clr;			//color
} T_BALL;

void ball_setRadius(T_BALL *pBall, int r);
float ball_getRadius(T_BALL *pBall);

void ball_getPos(T_BALL *pBall, float *px, float *py);

T_SPEED ball_getSpeed(T_BALL *pBall);

void ball_setSpeed(T_BALL *pBall, T_SPEED speed);

//draw ball on the surface
void ball_show(T_BALL *pBall, cairo_t *pCR);

//refresh ball status
void ball_refresh(T_BALL *pBall, cairo_t *pCR, int winWidth, int winHeight);

//move ball to new position according to speed
void ball_move(T_BALL *pBall, int winWidth, int winHeight);

//add speed
void ball_addSpeed(T_BALL *pBall, float xSpeed, float ySpeed);

//initialize a ball
T_BALL *ball_init(float x, float y, float r, unsigned int clr);

//destroy a ball
void ball_destroy(T_BALL *pBall);

//set color for ball
void ball_setClr(T_BALL *pBall, unsigned int clr);

//get color from ball
unsigned ball_getClr(T_BALL *pBall);

//set line width for ball
void ball_setWidth(T_BALL *pBall, unsigned int width);
