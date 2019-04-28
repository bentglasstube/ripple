#pragma once

#include "character.h"

class Enemy : public Character {
  public:

    enum class Type { Goomba, Spark };

    Enemy(Type type, bool inverted, double x, double y);

    void update(const Map& map, unsigned int elapsed) override;
    void draw(Graphics& graphics, int xo, int yo) const override;
    Rect hitbox() const override;

  private:

    static constexpr double kGoombaSpeed = 0.04;
    static constexpr double kSparkSpeed = 0.08;

    Type type_;
    int timer_;

    int sprite() const override;

    Rect colbox() const;
};
