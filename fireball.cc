#include "fireball.h"

Fireball::Fireball(bool inverted, double x, double y, Facing direction) :
  Character("misc.png", 16, inverted, x, y)
{
    vx_ = kMoveSpeed * (direction == Facing::Left ? -1 : 1);
    width_ = 4;
}

void Fireball::update(const Map& map, unsigned int elapsed) {
  vy_ += kGravity * elapsed * (inverted_ ? -1 : 1);

  const Map::Tile tx = map.collision(hitbox(), vx_ * elapsed, 0, inverted_);
  if (tx.type == Map::TileType::OutOfBounds) {
    dead_ = true;
  } else if (tx.obstructs(inverted_)) {
    bounceh(tx, 0.8);
  } else {
    x_ += vx_ * elapsed;
  }

  const Map::Tile ty = map.collision(hitbox(), 0, vy_ * elapsed, inverted_);
  if (ty.type == Map::TileType::OutOfBounds) {
    dead_ = true;
  } else if (ty.obstructs(inverted_)) {
    bouncev(ty, 0.6);
  } else {
    y_ += vy_ * elapsed;
  }

}

Rect Fireball::hitbox() const {
  return Rect(x_ - 2, y_ - 2, x_ + 2, y_ + 2);
}

int Fireball::drawx() const {
  return x_ - 8;
}

int Fireball::drawy() const {
  return y_ - 8;
}

int Fireball::sprite() const {
  return inverted_ ? 8 : 0;
}
