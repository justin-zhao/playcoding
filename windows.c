//windows.c

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "windows.h"
#include "data.h"

static GtkWidget *pDraw = NULL;
/* Surface to store current scribbles */
static cairo_surface_t *pSurface = NULL;
int winWidth = 0, winHeight = 0;
static gint timer = 0;

void clear_surface (void)
{
	cairo_t *cr;

	if (!pSurface)
		return;
	
	cr = cairo_create (pSurface);
	
	cairo_set_source_rgb (cr, 1, 1, 1);
	cairo_paint (cr);
	
	cairo_destroy (cr);
}

/* Create a new surface of the appropriate size to store our scribbles */
static gboolean configure_event_cb (GtkWidget         *widget,
                    GdkEventConfigure *event,
                    gpointer           data)
{
	if (pSurface)
		cairo_surface_destroy (pSurface);

	winWidth = gtk_widget_get_allocated_width (pDraw);
	winHeight = gtk_widget_get_allocated_height (pDraw);
	
	pSurface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
	                                            CAIRO_CONTENT_COLOR,
												winWidth,
												winHeight);
	
	/* Initialize the surface to white */
	clear_surface ();
	
	/* We've handled the configure event, no need for further processing. */
	return TRUE;
}

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean draw_cb (GtkWidget *widget,
         cairo_t   *cr,
         gpointer   data)
{
	if (!pSurface)
		return FALSE;

	if (winWidth < 1 || winHeight < 1)
	  return FALSE;

	cairo_set_source_surface (cr, pSurface, 0, 0);
//	refresh_all(cr, winWidth, winHeight);
	cairo_paint (cr);
	
	return FALSE;
}

/* Draw a rectangle on the surface at the given position */
static void draw_brush (GtkWidget *widget,
            gdouble    x,
            gdouble    y)
{
	cairo_t *cr;

	return;

	/* Paint to the surface, where we store our state */
	cr = cairo_create (pSurface);
	
	cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);
	cairo_rectangle (cr, x - 3, y - 3, 6, 6);
	cairo_fill (cr);
	
	cairo_destroy (cr);
	
	/* Now invalidate the affected region of the drawing area. */
	gtk_widget_queue_draw (widget);
	//gtk_widget_queue_draw_area (widget, x - 3, y - 3, 6, 6);

	return;
}

/* Handle button press events by either drawing a rectangle
 * or clearing the surface, depending on which button was pressed.
 * The ::button-press signal handler receives a GdkEventButton
 * struct which contains this information.
 */
static gboolean button_press_event_cb (GtkWidget      *widget, GdkEventButton *event,
                       gpointer        data)
{
	return TRUE;

	/* paranoia check, in case we haven't gotten a configure event */
	if (!pSurface)
		return FALSE;
	
	if (event->button == GDK_BUTTON_PRIMARY)
	{
		draw_brush (widget, event->x, event->y);
	}
	else if (event->button == GDK_BUTTON_SECONDARY)
	{
		clear_surface();
		gtk_widget_queue_draw (widget);
	}
	
	/* We've handled the event, stop processing */
	return TRUE;
}

/* Handle motion events by continuing to draw if button 1 is
 * still held down. The ::motion-notify signal handler receives
 * a GdkEventMotion struct which contains this information.
 */
static gboolean motion_notify_event_cb (GtkWidget      *widget,
                        GdkEventMotion *event,
                        gpointer        data)
{
	/* paranoia check, in case we haven't gotten a configure event */
	if (!pSurface)
		return FALSE;
	
	if (event->state & GDK_BUTTON1_MASK)
		draw_brush (widget, event->x, event->y);
	
	/* We've handled it, stop processing */
	return TRUE;
}

static void close_window (void)
{
	usleep(100);

	if (pSurface)
		cairo_surface_destroy (pSurface);
	pSurface = NULL;

	gtk_main_quit();
}

//static void sigroutine(int signo)
gint sigroutine(gpointer data)
{
	cairo_t *cr;

	if (!pSurface)
		return;
/*
	if (signo != SIGALRM)
		return;
*/
	
	/* Paint to the surface, where we store our state */
	cr = cairo_create (pSurface);
	cairo_set_source_rgb (cr, 1, 1, 1);
	cairo_paint (cr);
	refresh_all(cr, winWidth, winHeight);

	cairo_destroy (cr);

	gtk_widget_queue_draw ((GtkWidget *)data);
	//gtk_widget_queue_draw (pDraw);

	return TRUE;
}

void activate (GtkApplication *app, gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *frame;
// 	struct itimerval timerv, timero;
	
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Play Coding");
	
	g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);
	g_signal_connect (window, "key_press_event", G_CALLBACK (on_key_press), NULL);
	
	gtk_container_set_border_width (GTK_CONTAINER (window), 4);
	
	frame = gtk_frame_new (NULL);
	gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
	gtk_container_add (GTK_CONTAINER (window), frame);
	
	pDraw = gtk_drawing_area_new ();
	/* set a minimum size */
	gtk_widget_set_size_request (pDraw, 800, 600);
	
	gtk_container_add (GTK_CONTAINER (frame), pDraw);
	
	/* Signals used to handle the backing surface */
	g_signal_connect (pDraw, "draw",
	                  G_CALLBACK (draw_cb), NULL);
	g_signal_connect (pDraw,"configure-event",
	                  G_CALLBACK (configure_event_cb), NULL);
	
	/* Event signals */
	g_signal_connect (pDraw, "motion-notify-event",
	                  G_CALLBACK (motion_notify_event_cb), NULL);
	g_signal_connect (pDraw, "button-press-event",
	                  G_CALLBACK (button_press_event_cb), NULL);  
	
	/* Ask to receive events the drawing area doesn't normally
	 * subscribe to. In particular, we need to ask for the
	 * button press and motion notify events that want to handle.
	 */
	
	gtk_widget_set_events (pDraw, gtk_widget_get_events (pDraw)
	                                   | GDK_BUTTON_PRESS_MASK
	                                   | GDK_POINTER_MOTION_MASK);
/*
	signal(SIGALRM, sigroutine);
	timerv.it_value.tv_sec = 0;
	timerv.it_value.tv_usec = 40000;
	timerv.it_interval.tv_sec = 0;
	timerv.it_interval.tv_usec = 40000;
	setitimer(ITIMER_REAL, &timerv, &timero);
*/
	timer = g_timeout_add (40, sigroutine, pDraw);
	
	gtk_widget_show_all (window);
}
