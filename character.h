#pragma once

#include <string>

#include "graphics.h"
#include "spritemap.h"

#include "map.h"
#include "rect.h"

class Character {
  public:

    Character(const std::string& file, int height, bool inverted, double x, double y);

    virtual void update(const Map& map, unsigned int elapsed) = 0;
    virtual void draw(Graphics& graphics, int xo, int yo) const;

    double x() const;
    double y() const;
    bool dead() const;

  protected:

    enum class Facing { Right, Left };

    SpriteMap sprites_;
    int height_;
    double x_, y_;
    bool inverted_, dead_;
    Facing facing_;

    virtual int drawx() const;
    virtual int drawy() const;
    virtual int sprite() const = 0;
};
