#pragma once

#include "character.h"

class Fireball : public Character {
  public:

    Fireball(bool inverted, double x, double y, Facing direction);

    void update(const Map& map, unsigned int elapsed) override;
    Rect hitbox() const override;

  private:

    static constexpr double kGravity = 0.0003;
    static constexpr double kMoveSpeed = 0.23;

    int drawx() const override;
    int drawy() const override;
    int sprite() const override;
};
