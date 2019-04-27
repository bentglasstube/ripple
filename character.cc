#include "character.h"

Character::Character(bool inverted) :
  x_(0), y_(0),
  inverted_(inverted), dead_(false),
  facing_(Facing::Right) {}

void Character::draw(Graphics& graphics, int xo, int yo) const {
}

double Character::x() const {
  return x_;
}

double Character::y() const {
  return y_;
}

bool Character::dead() const {
  return dead_;
}
