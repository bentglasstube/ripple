#include "game_state.h"

GameState::GameState() : level_(1) {}

void GameState::next_level() {
  ++level_;
}

int GameState::level() const {
  return level_;
}
