#include "enemy.h"

Enemy::Enemy(Type type, bool inverted, double x, double y) :
  Character("enemies.png", 16, inverted, x, y), type_(type), timer_(0) {}

void Enemy::update(const Map& map, unsigned int elapsed) {
  timer_ += elapsed;

  if (type_ == Type::Goomba) {

    const double tx = x_ + kGoombaSpeed * (facing_ == Facing::Left ? -1 : 1);
    const double ty = y_ + (inverted_ ? -2 : 2);
    const Map::Tile t = map.tile(tx, ty);

    if (!t.obstructs(inverted_)) flip();
    x_ = tx;

  } else if (type_ == Type::Spark) {

  }
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
