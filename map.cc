#include "map.h"

#include <iostream>
#include <fstream>
#include <cmath>

Map::Map() :
  tileset_("level.png", 4, kTileSize, kTileSize),
  width_(0), height_(0) {}

void Map::load(const std::string& file) {
  std::ifstream reader("content/" + file);
  width_ = height_ = 0;

  std::string line;
  while (reader) {
    std::getline(reader, line);
    const size_t l = line.length();
    if (width_ == 0) width_ = l;
    for (size_t x = 0; x < l; ++x) {
      TileType& t = tiles_[height_][x];

      switch (line[x]) {
        case ' ':
          t = TileType::Empty;
          break;

        case '.':
          t = TileType::InvEmpty;
          break;

        case '^':
          t = TileType::Spikes;
          break;

        case 'v':
          t = TileType::InvSpikes;
          break;

        case '+':
          t = itile(x, height_ - 1).type == TileType::DoorTop ? TileType::DoorBottom : TileType::DoorTop;
          break;

        case '-':
          t = itile(x, height_ - 1).type == TileType::InvDoorBottom ? TileType::InvDoorTop : TileType::InvDoorBottom;
          break;
      }
    }
    ++height_;
  }
}

void Map::draw(Graphics& graphics, int xo, int yo) const {
  for (int y = 0; y < height_; ++y) {
    const int gy = kTileSize * y - yo;
    if (gy < -kTileSize) continue;
    if (gy > graphics.height()) break;

    for (int x = 0; x < width_; ++x) {
      const int gx = kTileSize * x - xo;
      if (gx < -kTileSize) continue;
      if (gx > graphics.width()) break;

      tileset_.draw(graphics, itile(x, y).sprite(), gx, gy);
    }
  }
}

Map::Tile Map::tile(double x, double y) const {
  return itile((int)(x / kTileSize), (int)(y / kTileSize));
}

Map::Tile Map::collision(Rect r, double dx, double dy, bool inverted) const {
  if (dx != 0) {
    const int x = (int) std::floor(((dx < 0 ? r.left : r.right) + dx) / kTileSize);
    return check_tiles(x, x, (int) r.top / kTileSize, (int) r.bottom / kTileSize, inverted);
  }

  if (dy != 0) {
    const int y = (int) std::floor(((dy < 0 ? r.top : r.bottom) + dy) / kTileSize);
    return check_tiles((int) r.left / kTileSize, (int) r.right / kTileSize, y, y, inverted);
  }

  return kNullTile;
}

int Map::pixel_width() const {
  return width_ * kTileSize;
}

int Map::pixel_height() const {
  return height_ * kTileSize;
}

Map::Tile Map::itile(int x, int y) const {
  Tile tile;

  if (x < 0 || x >= width_ || y < 0 || y >= height_) {
    tile.type = TileType::OutOfBounds;
  } else {
    tile.type = tiles_[y][x];
  }

  tile.top = y * kTileSize;
  tile.left = x * kTileSize;
  tile.right = tile.left + kTileSize;
  tile.bottom = tile.top + kTileSize;

  return tile;
}

Map::Tile Map::check_tiles(int x1, int x2, int y1, int y2, bool inverted) const {
  for (int y = y1; y <= y2; ++y) {
    for (int x = x1; x <= x2; ++x) {
      const Tile t = itile(x, y);
      if (t.obstructs(inverted)) return t;
    }
  }

  return kNullTile;
}

bool Map::Tile::obstructs(bool inverted) const {
  switch (type) {
    case Map::TileType::OutOfBounds:    return true;
    case Map::TileType::Empty:          return inverted;
    case Map::TileType::Spikes:         return inverted;
    case Map::TileType::DoorBottom:     return inverted;
    case Map::TileType::DoorTop:        return inverted;
    case Map::TileType::InvEmpty:       return !inverted;
    case Map::TileType::InvSpikes:      return !inverted;
    case Map::TileType::InvDoorBottom:  return !inverted;
    case Map::TileType::InvDoorTop:     return !inverted;
    default: return false;
  }
}

int Map::Tile::sprite() const {
  switch (type) {
    case Map::TileType::Empty:          return 0;
    case Map::TileType::Spikes:         return 3;
    case Map::TileType::DoorBottom:     return 6;
    case Map::TileType::DoorTop:        return 2;
    case Map::TileType::InvEmpty:       return 12;
    case Map::TileType::InvSpikes:      return 15;
    case Map::TileType::InvDoorBottom:  return 10;
    case Map::TileType::InvDoorTop:     return 14;
    default: return 0;
  }
}
