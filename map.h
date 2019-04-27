#pragma once

#include "spritemap.h"

#include "rect.h"

class Map {
  public:

    enum class TileType {
      Nothing, OutOfBounds,
      Empty, Spikes, DoorBottom, DoorTop,
      InvEmpty, InvSpikes, InvDoorBottom, InvDoorTop,
    };

    struct Tile {
      TileType type;
      double top, left, right, bottom;

      bool obstructs(bool inverted) const;
      int sprite() const;
    };

    Map();

    void load(const std::string& file);
    void draw(Graphics& graphics, int xo, int yo) const;

    Tile tile(double x, double y) const;
    Tile collision(Rect box, double dx, double dy, bool inverted) const;

    int pixel_width() const;
    int pixel_height() const;

    double startx() const;
    double starty() const;

  private:

    static constexpr int kTileSize = 16;
    static constexpr Tile kNullTile = {};

    SpriteMap tileset_;
    int width_, height_;
    TileType tiles_[128][1024];

    Tile itile(int x, int y) const;
    Tile check_tiles(int x1, int x2, int y1, int y2, bool inverted) const;
    int tile_sprite(const Tile& t) const;
};
