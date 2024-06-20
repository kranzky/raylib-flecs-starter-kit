#include "managers/game.h"

//==============================================================================

int main(void)
{
  game_manager_init();
  game_manager_loop();
  game_manager_fini();
  return 0;
}
