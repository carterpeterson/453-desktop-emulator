#ifndef __GAME_H__
#define __GAME_H__

#include <stdint.h>

void demo_loop(void);

typedef struct {
  void (*loop)(void);
} Game;

static Game demo_game = {
  .loop = demo_loop
};

void bloom_animation(void);

#endif
