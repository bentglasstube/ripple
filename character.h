#pragma once

#include "graphics.h"
#include "spritemap.h"

#include "map.h"
#include "rect.h"

class Character {
  public:

    Character(bool inverted);

    virtual void update(const Map& map, unsigned int elapsed) = 0;
    virtual void draw(Graphics& graphics, int xo, int yo) const;

    double x() const;
    double y() const;
    bool dead() const;

  protected:

    enum class Facing { Right, Left };

    double x_, y_;
    bool inverted_, dead_;
    Facing facing_;

    virtual int sprite() const = 0;
};
