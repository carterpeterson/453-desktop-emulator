/* X library headers */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

/* Normal C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* Application Specific C Headers */
#include "render.h"
#include "game.h"
#include "input.h"


int main(int argc, char** argv)
{  
  pthread_t task_thread;

  init_display(); // Setup X11
  init_input();   // Setup the input buffer locks

  // For now this spanws a thread that executes task routine,
  // will eventually spawn a thread for each task when I finish
  // ability to have multiple tasks
  pthread_create(&task_thread, NULL, (void *) &init_tasks, NULL);

  process_input();
}


