#include "input.h"

pthread_mutex_t input_buffer_lock;

uint32_t input_buffer[8] = {0};

void init_input(void)
{
  pthread_mutex_init(&input_buffer_lock, NULL);
}

void clear_array(void)
{
  pthread_mutex_lock(&input_buffer_lock);
  int i = 0;
  for(; i < 8; i++) {
    input_buffer[i] = 0;
  }
  pthread_mutex_unlock(&input_buffer_lock);
}

void process_input(void)
{
  XEvent event;		/* the XEvent declaration !!! */
  KeySym key;		/* a dealie-bob to handle KeyPress Events */	

  bool pressed = false;
  int cur_x, cur_y;
 
  /* look for events forever... */
  while(1) {
    usleep(10000); // Render at most 30 FPS

    pthread_mutex_lock(&frame_buffer_lock);
    if(render_flag) {
      render_frame_buffer();
      render_flag = false;
    }
    pthread_mutex_unlock(&frame_buffer_lock);

    while(XPending(dis)) {
      XNextEvent(dis, &event); // Blocks if no touch event
      if (event.type == ButtonPress) {
      
	pressed = true;

	cur_x = (event.xbutton.x * 32) / width;
	cur_y = (event.xbutton.y * 8) / height;

      } else if(event.type == ButtonRelease) {
	pressed = false;
	clear_array(); // Discard touch events from last frame
      } else if(event.type == MotionNotify &&
	   event.xmotion.x < width && event.xmotion.y < height) {
      
	cur_x = (event.xmotion.x * 32) / width;
	cur_y = (event.xmotion.y * 8) / height;
      }

      if(pressed) {
	clear_array(); // Discard touch events from last frame
	pthread_mutex_lock(&input_buffer_lock);
	input_buffer[cur_y] = 1 << (31 - cur_x);
	pthread_mutex_unlock(&input_buffer_lock);
      }
    }
  }
}

bool is_pixel_touched(int i, int j)
{
  bool touched;
     
  pthread_mutex_lock(&input_buffer_lock);
  touched = input_buffer[j] & (1 << (31 - i));
  pthread_mutex_unlock(&input_buffer_lock);
 
  return touched;
}

void get_input_array(uint32_t* local_buffer)
{

}
