#include "game.h"

#include "game_state.h"
#include "title_screen.h"

int main(int, char**) {
  Game::Config config;

  config.graphics.title = "Ripple";
  config.graphics.width = 256;
  config.graphics.height = 240;
  config.graphics.scale = 3;
  config.graphics.fullscreen = false;

  Game game(config);
  GameState state;
  game.loop(new TitleScreen(state));

  return 0;
}
