#ifndef __RENDER_H__
#define __RENDER_H__

#include <stdint.h>

// Physical Game Board Related Constants
#define PIXELS_WIDTH             32
#define PIXELS_HEIGHT             8

// Desktop emulation related constants
#define EMULATOR_WINDOW_WIDTH   640
#define EMULATOR_WINDOW_HEIGHT  160


typedef struct {
  uint8_t red, green, blue;
} Pixel;

extern Pixel display_frame_buffer[256];

#endif
