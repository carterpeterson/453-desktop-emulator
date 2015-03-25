#ifndef __INPUT_H__
#define __INPUT_H__

/* X library headers */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

/* Normal C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

/* Project Headers */
#include "render.h"

void init_input(void);
void process_input(void);
bool is_pixel_touched(int i, int j);
void get_input_array(uint32_t* local_buffer);

#endif
