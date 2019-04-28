#include "game_state.h"

#include <cassert>

GameState::GameState() : level_(1) {}

void GameState::next_level(bool normal, bool invert) {
  assert(normal || invert);

  ++level_;
  if (normal) ++saved_normal_;
  if (invert) ++saved_invert_;
}

void GameState::add_time(unsigned int elapsed) {
  elapsed_ += elapsed;
}

int GameState::level() const {
  return level_;
}

int GameState::saved_normal() const {
  return saved_normal_;
}

int GameState::saved_invert() const {
  return saved_invert_;
}

unsigned long GameState::time() const {
  return elapsed_;
}
