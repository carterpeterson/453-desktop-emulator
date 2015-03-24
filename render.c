/* X library headers */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

/* Normal C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Application Specific C Headers */
#include "render.h"
#include "game.h"

/* here are our X variables */
Display *dis;
int screen;
Window win;
GC gc;

/* here are our X routines declared! */
void init_x();
void close_x();
void redraw();
void create_colormap();
long resolve_pixel_color(Pixel p);
void render_frame_buffer();

int width, height, pixel_width, pixel_height;
Pixel display_frame_buffer[256];

int main(int argc, char** argv) {
	XEvent event;		/* the XEvent declaration !!! */
	KeySym key;		/* a dealie-bob to handle KeyPress Events */	
	char text[255];		/* a char buffer for KeyPress Events */
	int sleep_time = 1000000 / 30;
	
	init_x();

	int i = 0;
	for(; i < PIXELS_WIDTH * PIXELS_HEIGHT; i++) {
	  display_frame_buffer[i].red = 255;
	  display_frame_buffer[i].green = 255;
	  display_frame_buffer[i].blue = 255;
	}

	demo_game.setup();

	/* look for events forever... */
	while(1) {
	  /*XNextEvent(dis, &event);
	  if (event.type == ButtonPress) {
	    
	    int x_loc = (event.xbutton.x * 32) / width;
	    int y_loc = (event.xbutton.y * 8) / height;
	    
	    int input[8];
	    input[y_loc] = 1 << (31 - x_loc);

	    demo_game.handle_input(input);
	  } if(event.type == MotionNotify &&
	       event.xmotion.x < width && event.xmotion.y < height) {
	    
	    int x_loc = (event.xmotion.x * 32) / width;
	    int y_loc = (event.xmotion.y * 8) / height;
	    
	    int input[8];
	    input[y_loc] = 1 << (31 - x_loc);

	    demo_game.handle_input(input);
	  }*/

	  usleep(sleep_time);
	  demo_game.loop();
	  render_frame_buffer();
	}
}

void init_x() {
/* get the colors black and white (see section for details) */        
	unsigned long black,white;

	dis=XOpenDisplay((char *)0);
   	screen=DefaultScreen(dis);
	black=BlackPixel(dis,screen),
	white=WhitePixel(dis, screen);
   	win=XCreateSimpleWindow(dis,DefaultRootWindow(dis),0,0,	
		EMULATOR_WINDOW_WIDTH, EMULATOR_WINDOW_HEIGHT, 0, black, white);
	XSetStandardProperties(dis, win, "GameTable Emulator",
			       "GameTable Emulator", None, NULL, 0, NULL);
	XSelectInput(dis, win, ExposureMask|ButtonPressMask|Button1MotionMask);
        gc=XCreateGC(dis, win, 0,0);
	XSetBackground(dis,gc,white);
	XSetForeground(dis,gc,black);

	XSizeHints hints;
	hints.flags = PMinSize | PMaxSize;
	hints.max_width = EMULATOR_WINDOW_WIDTH;
	hints.min_width = EMULATOR_WINDOW_WIDTH;
	hints.max_height = EMULATOR_WINDOW_HEIGHT;
	hints.min_height = EMULATOR_WINDOW_HEIGHT;
	XSetWMNormalHints(dis, win, &hints);
	
	XClearWindow(dis, win);
	XMapRaised(dis, win);

	XWindowAttributes win_attr;
	XGetWindowAttributes(dis, win, &win_attr);
	width = win_attr.width;
	height = win_attr.height;
	pixel_width = width / 32;
	pixel_height = height / 8;
}

void close_x() {
	XFreeGC(dis, gc);
	XDestroyWindow(dis,win);
	XCloseDisplay(dis);	
	exit(1);				
}

void redraw() {
	XClearWindow(dis, win);
}

long resolve_pixel_color(Pixel p) {
  return (long) (p.red << 16) + (p.green << 8) + p.blue;
}

void render_frame_buffer() {
  
  int i = 0;
  for(; i < PIXELS_WIDTH * PIXELS_HEIGHT; i++) {
    
    XSetForeground(dis, gc, resolve_pixel_color(display_frame_buffer[i]));
    XFillRectangle(dis, win, gc, (i % PIXELS_WIDTH) * pixel_width,
		   (i / PIXELS_WIDTH) * pixel_height, pixel_width, pixel_height);
  }
  
  XFlush(dis);
}
