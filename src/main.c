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


int main(int argc, char** argv) {  
  init_display();
  
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

    demo_game.loop();
  }
}
