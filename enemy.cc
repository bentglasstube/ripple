#include "enemy.h"

#include <cassert>
#include <cmath>

Enemy::Enemy(Type type, bool inverted, double x, double y) :
  Character("enemies.png", 16, inverted, x, y), type_(type), timer_(0) {
    if (type == Type::Goomba) vx_ = -kGoombaSpeed;
    if (type == Type::Spark) vx_ = kSparkSpeed * (inverted_ ? -1 : 1);
  }

void Enemy::update(const Map& map, unsigned int elapsed) {
  timer_ += elapsed;

  if (type_ == Type::Goomba) {

    if (vx_ < 0) facing_ = Facing::Left;
    if (vx_ > 0) facing_ = Facing::Right;

    const double tx = x_ + elapsed * vx_;
    const double ty = y_ + (inverted_ ? -2 : 2);
    const Map::Tile t = map.tile(tx, ty);

    if (!t.obstructs(inverted_)) {
      bounceh(t, 1);
    } else {
      x_ += vx_ * elapsed;
    }

  } else if (type_ == Type::Spark) {

    const double dx = vx_ * elapsed;
    const double dy = vy_ * elapsed;

    if (dx != 0) {
      const Map::Tile turn = map.collision(colbox(), 0, dx, inverted_);
      if (!turn.obstructs(inverted_)) {
        x_ = std::round(x_);
        y_ += dx;
        vy_ = vx_;
        vx_ = 0;
      } else {
        const Map::Tile ahead = map.collision(colbox(), dx, 0, inverted_);
        if (ahead.obstructs(inverted_)) {
          x_ = std::round(x_);
          y_ -= dx;
          vy_ = -vx_;
          vx_ = 0;
        } else {
          x_ += dx;
        }
      }
    } else if (dy != 0) {
      const Map::Tile turn = map.collision(colbox(), -dy, 0, inverted_);
      if (!turn.obstructs(inverted_)) {
        x_ -= dy;
        y_ = std::round(y_);
        vx_ = -vy_;
        vy_ = 0;
      } else {
        const Map::Tile ahead = map.collision(colbox(), 0, dy, inverted_);
        if (ahead.obstructs(inverted_)) {
          x_ += dy;
          y_ = std::round(y_);
          vx_ = vy_;
          vy_ = 0;
        } else {
          y_ += dy;
        }
      }
    }
  }
}

void Enemy::draw(Graphics& graphics, int xo, int yo) const {
  Character::draw(graphics, xo, yo);

#ifndef NDEBUG
  Rect r = colbox();
  SDL_Rect sr = { (int) r.left - xo, (int) r.top - yo, (int) (r.right - r.left), (int) (r.bottom - r.top) };
  graphics.draw_rect(&sr, 0x0000ff80, false);
#endif
}

int Enemy::sprite() const {
  switch (type_) {
    case Type::Goomba:
      return (inverted_ ? 8 : 0) + (timer_ / 250) % 3;

    case Type::Spark:
      return (inverted_ ? 12 : 4) + (timer_ / 100) % 2;

    default:
      return 0;
  }
}

Rect Enemy::hitbox() const {
  switch (type_) {
    case Type::Goomba:
      return inverted_ ? Rect(x_ - 5, y_ + 1, x_ + 5, y_ + 11) :
        Rect(x_ - 5, y_ - 11, x_ + 5, y_ - 1);

    case Type::Spark:
      return inverted_ ? Rect(x_ - 3, y_ + 3, x_ + 3, y_ + 13) :
        Rect(x_ - 3, y_ - 13, x_ + 3, y_ - 3);

    default:
      return Character::hitbox();
  }
}

Rect Enemy::colbox() const {
  return inverted_ ? Rect(x_ - 8, y_, x_ + 7, y_ + 15) :
    Rect(x_ - 7, y_ - 15, x_ + 7, y_);
}
