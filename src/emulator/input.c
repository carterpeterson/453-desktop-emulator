#include "input.h"

pthread_mutex_t input_buffer_lock;

uint32_t input_buffer[8] = {0};

void init_input(void)
{
  pthread_mutex_init(&input_buffer_lock, NULL);
}

void process_input(void)
{
  XEvent event;		/* the XEvent declaration !!! */
  KeySym key;		/* a dealie-bob to handle KeyPress Events */	

  /* look for events forever... */
  while(1) {
    XNextEvent(dis, &event);
    if (event.type == ButtonPress) {
      
      int x_loc = (event.xbutton.x * 32) / width;
      int y_loc = (event.xbutton.y * 8) / height;
      
      pthread_mutex_lock(&input_buffer_lock);
      input_buffer[y_loc] = 1 << (31 - x_loc);
      pthread_mutex_unlock(&input_buffer_lock);

    } if(event.type == MotionNotify &&
	 event.xmotion.x < width && event.xmotion.y < height) {
      
      int x_loc = (event.xmotion.x * 32) / width;
      int y_loc = (event.xmotion.y * 8) / height;

      pthread_mutex_lock(&input_buffer_lock);
      input_buffer[y_loc] = 1 << (31 - x_loc);
      pthread_mutex_unlock(&input_buffer_lock);
    }
  }
}

bool is_pixel_touched(int i, int j)
{
  bool touched;
     
  pthread_mutex_lock(&input_buffer_lock);
  touched = input_buffer[(j * 32) + i];
  pthread_mutex_unlock(&input_buffer_lock);
 
  return touched;
}

void get_input_array(uint32_t* local_buffer)
{

}
