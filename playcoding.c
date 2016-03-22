#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "windows.h"

int main (int argc, char **argv)
{
	GtkApplication *app;
	int status;
	
	app = gtk_application_new ("justin.gtk", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	
	data_init();

	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);
	
	sleep(1);
	data_destroy();
	
	return status;
}
