#pragma once

#include "character.h"

class Fireball : public Character {
  public:

    Fireball(bool inverted, double x, double y, Facing direction);

    void update(const Map& map, unsigned int elapsed) override;
    Rect hitbox() const override;

  private:

    static constexpr double kMoveSpeed = 0.11;
    static constexpr double kDropSpeed = 0.04;

    int drawx() const override;
    int drawy() const override;
    int sprite() const override;
};
