// global.h

#define CLR_RGB(R,G,B) (unsigned int)((R&0xFF)<<16|(G&0xFF)<<8|(B&0xFF))
#define CLR_FETCHR(clr) (float)(((clr>>16)&0xFF)/255.0)
#define CLR_FETCHG(clr) (float)(((clr>>8)&0xFF)/255.0)
#define CLR_FETCHB(clr) (float)((clr&0xFF)/255.0)

typedef struct _SPEED{
	float xSpeed, ySpeed;
} T_SPEED;
