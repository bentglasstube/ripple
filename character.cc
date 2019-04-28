#include "character.h"

Character::Character(const std::string& file, int height, bool inverted, double x, double y) :
  sprites_(file, 4, 16, height),
  height_(height), x_(x), y_(y),
  inverted_(inverted), dead_(false),
  facing_(Facing::Right) {}

void Character::draw(Graphics& graphics, int xo, int yo) const {
  if (dead_) return;
  sprites_.draw_ex(graphics, sprite(), drawx() - xo, drawy() - yo, facing_ == Facing::Left, 0, 0, 0);

#ifndef NDEBUG
  const Rect hb = hitbox();
  const SDL_Rect r = { (int) hb.left, (int) hb.top, (int) (hb.right - hb.left), (int) (hb.bottom - hb.top) };
  graphics.draw_rect(&r, 0xffff0080, false);
#endif
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

Rect Character::hitbox() const {
  if (inverted_) {
    return Rect(x_ - 8, y_, x_ + 8, y_ + height_);
  } else {
    return Rect(x_ - 8, y_ - height_, x_ + 8, y_);
  }
}

bool Character::collision(const Rect& other) const {
  return hitbox().intersect(other);
}

void Character::flip() {
  facing_ = facing_ == Facing::Left ? Facing::Right : Facing::Left;
}

int Character::drawx() const {
  return x_ - 8;
}

int Character::drawy() const {
  return y_ - (inverted_ ? 0 : height_);
}
