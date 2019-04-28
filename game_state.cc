#include "game_state.h"

GameState::GameState() : level_(3) {}

void GameState::next_level() {
  ++level_;
}

int GameState::level() const {
  return level_;
}
