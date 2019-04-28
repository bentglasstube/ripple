#pragma once

#include <string>

#include "graphics.h"
#include "spritemap.h"

#include "map.h"
#include "rect.h"

class Character {
  public:

    enum class Facing { Right, Left };

    Character(const std::string& file, int height, bool inverted, double x, double y);

    virtual void update(const Map& map, unsigned int elapsed) = 0;
    virtual void draw(Graphics& graphics, int xo, int yo) const;

    double x() const;
    double y() const;
    bool dead() const;

    virtual Rect hitbox() const;
    bool collision(const Rect& other) const;

    void flip();
    void kill();

  protected:

    int width_, height_;
    double x_, y_, vx_, vy_;
    SpriteMap sprites_;
    bool inverted_, dead_, grounded_;
    Facing facing_;

    virtual int drawx() const;
    virtual int drawy() const;
    virtual int sprite() const = 0;

    void bouncev(const Map::Tile& tile, double bounce);
    void bounceh(const Map::Tile& tile, double bounce);
};
