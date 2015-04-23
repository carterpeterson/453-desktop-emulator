// General C includes
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// Project specific includes
#include "tasks.h"

#define BLOOM_RADIUS 3
#define FULL_RADIUS_STEP 100

static uint16_t current_color_base = 0;
static double MAX_RADIUS;

Pixel get_bloom_pixel(int i, int j)
{
  uint16_t current_color = current_color_base;

  current_color += (hypot((double) i, (double) j) / MAX_RADIUS) * FULL_RADIUS_STEP;
  current_color = current_color % 768;
      
  Pixel p;
  if(current_color < 256) {
    p.red = (255 - current_color);
    p.green = current_color;
    p.blue = 0;
  } else if(current_color < 512) {
    p.red = 0;
    p.green = (511 - current_color);
    p.blue = (current_color - 256);
  } else {
    p.red = (current_color - 512);
    p.green = 0;
    p.blue = 767 - current_color ;
  }

  return p;
}

void bloom_animation(void)
{
  MAX_RADIUS = hypot(BLOOM_RADIUS, BLOOM_RADIUS);
  current_color_base += 1;
  int i, j;
  Pixel white_pixel;
  white_pixel.red = 255;
  white_pixel.green = 255;
  white_pixel.blue = 255;

  if(current_color_base > (256 * 3))
    current_color_base = 0;

  for(i = 0; i < 32; i++) {
    int temp = i;
    
    for(j = 0; j < 8; j++) {
      
      Pixel p;
      if((i % 6) < 3 && (j % 6) < 3) {
	p = get_bloom_pixel((i % 3), (j % 3));
      } else if ((i % 6) >= 3 && (j % 6) < 3) {
	p = get_bloom_pixel(3 - (i % 3), (j % 3));	
      } else if ((i % 6) < 3 && (j % 6) >= 3) {
	p = get_bloom_pixel((i % 3), 3 - (j % 3));	
      } else {
	p = get_bloom_pixel(3 - (i % 3), 3 - (j % 3));	
      }
      
      if(is_pixel_touched(i, j))
	frame_buffer[(j * 32) + i] = white_pixel;
      else
	frame_buffer[(j * 32) + i] = p;
    }
    i = temp;
  }

  render();
}

/*void demo_task(void)
{
  while(1) {
    usleep(33333);
    bloom_animation();
  }
  }*/
