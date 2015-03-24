#ifndef __GAME_H__
#define __GAME_H__

#include <stdint.h>

/* Defining the Game API:

   - setup: Called once when game becomes active game,
   ideally used for malloc and other one time initialization things.
   - teardown: Called once when the game is going to not be active
   anymore. Use it to dealloc any persisting objects and close active
   peripheral connections
   - handleInput: Will be called when the current state of user input
   on the board chages.
   - loop: Your main game loop, use this to perform your main game logic.
   Always exit this loop, the background program will re-enter every time.

 */
void demo_setup(void);
void demo_teardown(void);
void demo_handle_input(int input[]);
void demo_loop(void);

typedef struct {
  void (*setup)(void);
  void (*teardown)(void);
  void (*handle_input)(int input[]);
  void (*loop)(void);
} Game;

static Game demo_game = {
  .setup = demo_setup,
  .teardown = demo_teardown,
  .handle_input = demo_handle_input,
  .loop = demo_loop
};

#endif
