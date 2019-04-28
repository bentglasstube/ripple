#include "character.h"

Character::Character(const std::string& file, int height, bool inverted, double x, double y) :
  width_(16), height_(height), x_(x), y_(y), vx_(0), vy_(0),
  sprites_(file, 4, width_, height),
  inverted_(inverted), dead_(false), grounded_(false),
  facing_(Facing::Right) {}

void Character::draw(Graphics& graphics, int xo, int yo) const {
  if (dead_) return;
  sprites_.draw_ex(graphics, sprite(), drawx() - xo, drawy() - yo, facing_ == Facing::Left, 0, 0, 0);

#ifndef NDEBUG
  const Rect hb = hitbox();
  const SDL_Rect r = { (int) hb.left - xo, (int) hb.top - yo, (int) (hb.right - hb.left), (int) (hb.bottom - hb.top) };
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

bool Character::grounded() const {
  return grounded_;
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

void Character::kill() {
  dead_ = true;
}

int Character::drawx() const {
  return x_ - 8;
}

int Character::drawy() const {
  return y_ - (inverted_ ? 0 : height_);
}

void Character::bouncev(const Map::Tile& tile, double bounce) {
  if (inverted_) {
    if (vy_ > 0) {
      y_ = tile.top - height_;
    } else {
      y_ = tile.bottom;
      grounded_ = true;
    }
  } else {
    if (vy_ > 0) {
      y_ = tile.top;
      grounded_ = true;
    } else {
      y_ = tile.bottom + height_;
    }
  }
  vy_ *= -bounce;
}

void Character::bounceh(const Map::Tile& tile, double bounce) {
  x_ = vx_ > 0 ? tile.left - width_ / 2: tile.right + width_ / 2;
  vx_ *= -bounce;
}
