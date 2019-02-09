#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "windows.h"
#include <curses.h>

int main (int argc, char **argv)
{
	GtkApplication *app;
	int i,j,k,l,status;

#if (0)
	char ch;
	char *pMatrix=NULL;

	initscr();

	pMatrix = (char *)malloc(LINES*COLS*sizeof(char));

	for(k=0;k<LINES;k++)
		for(l=0;l<COLS;l++)
			*(pMatrix+k*COLS+l) = 0;

	i = 0;
	j = 0;
	while(1)
	{
		//i++;

		timeout(100);
		ch = getch();
		if (ch==66)
	    	i=i+1;
		if (ch==65)
			i=i-1;
		if (ch==67)
			j=j+1;
		if (ch==68)
			j=j-1;
		if (i<0)
			i=0;
		if (i>=LINES)
			i=LINES-1;
		if (j<0)
			j=0;
		if (j>=COLS)
			j=COLS-1;

		*(pMatrix+i*COLS+j) = 1;
		//matrix[i][j]=1;
		/*
		if (ch==-1)
		{
			//printf("#");
		}
		else
			printf("ch=%d.\r\n", (int)ch);
		k = sin(i/5.0)*80;
		k = k+100;
		for(j=0;j<k;j++)
			printf(" ");
		*/
		//move(LINES/2, COLS/2);
		/*
		j=rand()%LINES;
		k=rand()%COLS;
		l=rand()%10+3;
		*/
//			move(i,i*19);
/*
		if(i>=LINES)
			move(LINES-1, k);
		else
			move(i, k);
*/
			//clear();
			erase();
		for(k=0;k<LINES;k++)
			for(l=0;l<COLS;l++)
			{
				if (1==*(pMatrix+k*COLS+l))
				{
					move(k,l);
					waddstr(stdscr, "*");
				}
			}
			//printf("*");


		//waddstr(stdscr, "Hello, world!");
		//move(LINES,COLS);
		//printf("\r\n");
		refresh();
//		if(i>=LINES) printf("\r\n");
		//usleep(10000);
/*
		printf("qing shu ru an jian:");
		ch = getchar();

		printf("ni shu ru de shi:%c\r\n", ch);

		if (ch == 'g')
			printf("wo shi zhao zhi yuan\r\n");	
		else
			printf("wo shi zhao zhi xiang\r\n");	

		getchar();
*/
	}
	endwin();
	free(pMatrix);

	return 0;
#endif

	app = gtk_application_new ("justin.gtk", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	
	data_init();

	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);
	
	data_destroy();

	
	return status;
	}
