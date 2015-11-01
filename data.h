//data.c

#include <gtk/gtk.h>

void data_init();
void data_destroy();

void refresh_all(cairo_t *pCR, int winWidth, int winHeight);
gboolean on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data);
