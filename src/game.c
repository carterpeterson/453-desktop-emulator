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

#define SLEEP_30_FPS 33333

void demo_loop(void)
{
  printf("-- Entered Game Control --\n\r");
  while(1) {
    usleep(SLEEP_30_FPS);
    bloom_animation();
  }
}
