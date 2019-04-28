#pragma once

#include "character.h"

class Enemy : public Character {
  public:

    enum class Type { Goomba, Spark };

    Enemy(Type type, bool inverted, double x, double y);

    void update(const Map& map, unsigned int elapsed) override;

  private:

    static constexpr double kGoombaSpeed = 0.4;
    static constexpr double kSparkSpeed = 1.5;

    Type type_;
    int timer_;

    int sprite() const override;
};
