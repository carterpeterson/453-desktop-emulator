
// General C includes
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// Project specific includes
#include "game.h"
#include "render.h"
#include "input.h"

void shift_frame_buffer(void)
{
  int i, j;
  Pixel wrap_hold[8];
  
  for(i = 0; i < PIXELS_WIDTH; i++) {
    for(j = 0; j < PIXELS_HEIGHT; j++) {
      if(i == 0) {
	wrap_hold[j] = display_frame_buffer[j * PIXELS_WIDTH];
	display_frame_buffer[j * PIXELS_WIDTH] = display_frame_buffer[(j * PIXELS_WIDTH) + 1];
      } else if(i == 31){
	display_frame_buffer[(j * PIXELS_WIDTH) + i] = wrap_hold[j];
      } else {
	display_frame_buffer[(j * PIXELS_WIDTH) + i] = display_frame_buffer[((j * PIXELS_WIDTH) + i) + 1];
      }
    }
  }
}

// Below demo_setup is for old demo animation

/*void demo_setup(void)
{
  /*uint8_t plexus_logo[256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1,
    1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1,
    1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1,
    1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };

  
  Pixel p;
  p.red = 255;
  p.green = 0;
  p.blue = 0;

  int i = 0;
  for(; i < PIXELS_WIDTH * PIXELS_HEIGHT; i++) {
    if(plexus_logo[i] == 1) {
      display_frame_buffer[i] = p;
    }
    }



  int green_step = 255 / PIXELS_WIDTH;
  int blue_step = 255 / PIXELS_HEIGHT;
  int i, j;

  for(i = 0; i < PIXELS_WIDTH; i++) {
    for(j = 0; j < PIXELS_HEIGHT; j++) {
      Pixel p;
      p.red = 0;
      p.green = (i % PIXELS_WIDTH) * green_step;
      p.blue = j * blue_step;

      display_frame_buffer[(j * PIXELS_WIDTH) + i] = p;
    }
  }
  
}*/

void demo_setup(void)
{
  // Do nothing
}

void demo_teardown(void)
{
  // Currently do nothing here, would deallocate objects if needed
}

void demo_handle_input(int input[])
{ 
  //  shift_frame_buffer();
}

// This demo loop is for the old animation

/*int red_offset = 0;
bool ascending = true;

void demo_loop(void)
{
  int green_step = 255 / PIXELS_WIDTH;
  int blue_step = 255 / PIXELS_HEIGHT;
  int i, j;

  for(i = 0; i < PIXELS_WIDTH; i++) {
    for(j = 0; j < PIXELS_HEIGHT; j++) {
      Pixel p;
      p.red = red_offset;
      p.green = (i % PIXELS_WIDTH) * green_step;// + red_offset;
      p.blue = j * blue_step;

      display_frame_buffer[(j * PIXELS_WIDTH) + i] = p;
    }
  }

  if(ascending == true) {
    red_offset += 5;
    if(red_offset == 255)
      ascending = false;
  } else {
    red_offset -= 5;
    if(red_offset == 0)
      ascending = true;
  }
  }*/

#define BLOOM_RADIUS 3
#define FULL_RADIUS_STEP 100

uint16_t current_color_base = 0;
double MAX_RADIUS;

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
      
      display_frame_buffer[(j * 32) + i] = p;
    }
    i = temp;
  } 
}



void demo_loop(void)
{
  bloom_animation();
}


