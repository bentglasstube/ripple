#include "character.h"

Character::Character(const std::string& file, int height, bool inverted, double x, double y) :
  sprites_(file, 4, 16, height),
  height_(height), x_(x), y_(y),
  inverted_(inverted), dead_(false),
  facing_(Facing::Right) {}

void Character::draw(Graphics& graphics, int xo, int yo) const {
  if (dead_) return;
  sprites_.draw_ex(graphics, sprite(), drawx(), drawy(), facing_ == Facing::Left, 0, 0, 0);
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

int Character::drawx() const {
  return x_ - 8;
}

int Character::drawy() const {
  return y_ - (inverted_ ? 0 : height_);
}
