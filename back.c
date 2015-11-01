#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

#define MYRGB(R,G,B) (unsigned int)((R&0xFF)<<16|(G&0xFF)<<8|(B&0xFF))
#define FETCHR(clr) (float)(((clr>>16)&0xFF)/255.0)
#define FETCHG(clr) (float)(((clr>>8)&0xFF)/255.0)
#define FETCHB(clr) (float)((clr&0xFF)/255.0)

typedef struct _BALL{
  int x, y, r;
  int width;
  unsigned int clr;
} T_BALL;

static GtkWidget *pDraw = NULL;
static T_BALL *pBall1 = NULL;
static int scr_width = 0;
static int scr_height = 0;
static int speedx=5;
static int speedy=3;

/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;

static void
clear_surface (void)
{
  cairo_t *cr;

  cr = cairo_create (surface);

  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);

  cairo_destroy (cr);
}

/* Create a new surface of the appropriate size to store our scribbles */
static gboolean
configure_event_cb (GtkWidget         *widget,
                    GdkEventConfigure *event,
                    gpointer           data)
{
  if (surface)
    cairo_surface_destroy (surface);

  scr_width = gtk_widget_get_allocated_width (widget);
  scr_height = gtk_widget_get_allocated_height (widget);
  surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                               CAIRO_CONTENT_COLOR,
                                               scr_width,
                                               scr_height);

  /* Initialize the surface to white */
  clear_surface ();

  /* We've handled the configure event, no need for further processing. */
  return TRUE;
}

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean
draw_cb (GtkWidget *widget,
         cairo_t   *cr,
         gpointer   data)
{
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);

  return FALSE;
}

/* Draw a rectangle on the surface at the given position */
static void
draw_brush (GtkWidget *widget,
            gdouble    x,
            gdouble    y)
{
  cairo_t *cr;

  /* Paint to the surface, where we store our state */
  cr = cairo_create (surface);

  //cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
  cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);

  //cairo_move_to (cr, x-3, y-3);

  cairo_set_line_width (cr, 2);

  //cairo_save (cr);
  //cairo_scale (cr, 0.1, 1);
  cairo_arc (cr, x, y, 10, 0, 2 * M_PI);
  cairo_stroke (cr);

  //cairo_rectangle (cr, x - 3, y - 3, 6, 6);
  //cairo_fill (cr);

  cairo_destroy (cr);

  /* Now invalidate the affected region of the drawing area. */
  gtk_widget_queue_draw_area (widget, x - 40, y - 40, 80, 80);
}

/* Handle button press events by either drawing a rectangle
 * or clearing the surface, depending on which button was pressed.
 * The ::button-press signal handler receives a GdkEventButton
 * struct which contains this information.
 */
static gboolean
button_press_event_cb (GtkWidget      *widget,
                       GdkEventButton *event,
                       gpointer        data)
{
  /* paranoia check, in case we haven't gotten a configure event */
  if (surface == NULL)
    return FALSE;

  printf("Button=%d\r\n", event->button);
  if (event->button == GDK_BUTTON_PRIMARY)
    {
      draw_brush (widget, event->x, event->y);
    }
  else if (event->button == GDK_BUTTON_SECONDARY)
    {
      clear_surface ();
      gtk_widget_queue_draw (widget);
    }

  /* We've handled the event, stop processing */
  return TRUE;
}

/* Handle motion events by continuing to draw if button 1 is
 * still held down. The ::motion-notify signal handler receives
 * a GdkEventMotion struct which contains this information.
 */
static gboolean
motion_notify_event_cb (GtkWidget      *widget,
                        GdkEventMotion *event,
                        gpointer        data)
{
  /* paranoia check, in case we haven't gotten a configure event */
  if (surface == NULL)
    return FALSE;

  if (event->state & GDK_BUTTON1_MASK)
    draw_brush (widget, event->x, event->y);

  /* We've handled it, stop processing */
  return TRUE;
}

static void
close_window (void)
{
  if (surface)
    cairo_surface_destroy (surface);

  gtk_main_quit ();
}

static gboolean
on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
  printf("Key pressed: %d\r\n", event->keyval);
  //clear_surface ();
  //gtk_widget_queue_draw (widget);
  if (event->keyval == 65362)
    speedy--;
  if (event->keyval == 65364)
    speedy++;

  if (event->keyval == 65361)
    speedx--;
  if (event->keyval == 65363)
    speedx++;

  return TRUE; 
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *frame;
  GtkWidget *drawing_area;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Drawing Area");

  g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);
  g_signal_connect (window, "key_press_event", G_CALLBACK (on_key_press), NULL);

  gtk_container_set_border_width (GTK_CONTAINER (window), 4);

  frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
  gtk_container_add (GTK_CONTAINER (window), frame);

  drawing_area = gtk_drawing_area_new ();
  /* set a minimum size */
  gtk_widget_set_size_request (drawing_area, 800, 600);

  gtk_container_add (GTK_CONTAINER (frame), drawing_area);

  /* Signals used to handle the backing surface */
  g_signal_connect (drawing_area, "draw",
                    G_CALLBACK (draw_cb), NULL);
  g_signal_connect (drawing_area,"configure-event",
                    G_CALLBACK (configure_event_cb), NULL);

  /* Event signals */
  g_signal_connect (drawing_area, "motion-notify-event",
                    G_CALLBACK (motion_notify_event_cb), NULL);
  g_signal_connect (drawing_area, "button-press-event",
                    G_CALLBACK (button_press_event_cb), NULL);  
  pDraw = drawing_area;

  /* Ask to receive events the drawing area doesn't normally
   * subscribe to. In particular, we need to ask for the
   * button press and motion notify events that want to handle.
   */

  gtk_widget_set_events (drawing_area, gtk_widget_get_events (drawing_area)
                                     | GDK_BUTTON_PRESS_MASK
                                     | GDK_POINTER_MOTION_MASK);

  gtk_widget_show_all (window);
}

void ball_show(T_BALL *pBall)
{
  if (!pDraw || !pBall || !surface)
    return;

  cairo_t *cr;

  /* Paint to the surface, where we store our state */
  cr = cairo_create (surface);

  cairo_set_source_rgb(cr, FETCHR(pBall->clr), FETCHG(pBall->clr), FETCHB(pBall->clr));
  cairo_set_line_width (cr, pBall->width);

  cairo_arc (cr, pBall->x, pBall->y, pBall->r, 0, 2 * M_PI);
  cairo_stroke (cr);

  cairo_destroy (cr);

  /* Now invalidate the affected region of the drawing area. */
  gtk_widget_queue_draw_area (pDraw, pBall->x - pBall->r - pBall->width, pBall->y - pBall->r - pBall->width, 
                               2*(pBall->r + pBall->width), 2*(pBall->r + pBall->width));
}

void ball_move(T_BALL *pBall, int x, int y)
{
  unsigned int oldclr;

  if (!pBall)
    return;

  oldclr = pBall->clr;
  pBall->clr = MYRGB(255,255,255);  
  pBall->width = pBall->width + 1;
  ball_show(pBall);

  pBall->x = x;
  pBall->y = y;
  pBall->width = pBall->width - 1;
  pBall->clr = oldclr;
  ball_show(pBall);
}

T_BALL *ball_init(int x, int y, int r, unsigned int clr)
{
  T_BALL *pBall;

  pBall = malloc(sizeof(T_BALL));

  pBall->x = x;
  pBall->y = y;
  pBall->r = r;
  pBall->clr = clr;
  pBall->width = 2;

  return pBall;
}

void ball_destroy(T_BALL *pBall)
{
  free(pBall);
}

void sigroutine(int signo)
{
  int x, y;

  if (scr_width < 1 || scr_height < 1)
    return;

  switch(signo)
  {
  case SIGALRM:
    if (pBall1)
    {
      x = pBall1->x + speedx;
      y = pBall1->y + speedy;
      if (x > scr_width)
      {
        speedx = 0-speedx;
        x = scr_width - x%scr_width;
      }

      if (x < 0)
      {
        speedx = 0-speedx;
        x = -x;
      }

      if (y > scr_height)
      {
        speedy = 0-speedy;
        y = scr_height - y%scr_height;
      }

      if (y < 0)
      {
        speedy = 0-speedy;
        y = -y;
      }

      ball_move(pBall1, x, y);
    }
  break;
  }
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;
  T_BALL *temp;
  struct itimerval timerv, timero;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

  pBall1 = ball_init(100, 100, 10, MYRGB(255,0,0));
  signal(SIGALRM, sigroutine);
  timerv.it_value.tv_sec = 0;
  timerv.it_value.tv_usec = 40000;
  timerv.it_interval.tv_sec = 0;
  timerv.it_interval.tv_usec = 40000;
  setitimer(ITIMER_REAL, &timerv, &timero);

  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  temp = pBall1;
  pBall1 = NULL;
  sleep(1);
  ball_destroy(temp);

  return status;
}
