#include "fireball.h"

Fireball::Fireball(bool inverted, double x, double y, Facing direction) :
  Character("misc.png", 16, inverted, x, y)
{
    facing_ = direction;
}

void Fireball::update(const Map& map, unsigned int elapsed) {
  const double dx = kMoveSpeed * elapsed * (facing_ == Facing::Left ? -1 : 1);
  const double dy = kDropSpeed * elapsed * (inverted_ ? -1 : 1);

  const Map::Tile tx = map.tile(x_ + dx, y_);
  if (tx.type == Map::TileType::OutOfBounds) {
    dead_ = true;
  } else if (tx.obstructs(inverted_)) {
    x_ = facing_ == Facing::Left ? tx.right : tx.left;
    flip();
  } else {
    x_ += dx;
  }

  const Map::Tile ty = map.tile(x_, y_ + dy);
  if (ty.obstructs(inverted_)) {
    dead_ = true;
  } else {
    y_ += dy;
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
