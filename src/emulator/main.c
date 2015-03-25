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
#include "input.h"


int main(int argc, char** argv) {  
  init_display();
  init_tasks();

  process_input();
}
