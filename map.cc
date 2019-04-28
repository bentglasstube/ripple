#include "map.h"

#include <cmath>

Map::Map() : tileset_("level.png", 4, kTileSize, kTileSize), width_(0), height_(0) {}

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
  return itile((int)std::floor(x / kTileSize), (int)std::floor(y / kTileSize));
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

void Map::set_size(int width, int height) {
  width_ = width;
  height_ = height;
}

void Map::set_tile(int x, int y, TileType type) {
  if (y < 0 || y > 128) return;
  if (x < 0 || x > 1024) return;

  if (type == TileType::DoorTop || type == TileType::InvDoorBottom) {
    if (itile(x, y - 1).type == type) {
      if (type == TileType::DoorTop) type = TileType::DoorBottom;
      if (type == TileType::InvDoorBottom) type = TileType::InvDoorTop;
    }
  }

  tiles_[y][x] = type;
}

void Map::toggle_blocks() {
  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      TileType& t = tiles_[y][x];
      if (t == TileType::BlockOff) t = TileType::BlockOn;
      else if (t == TileType::BlockOn) t = TileType::BlockOff;
      else if (t == TileType::InvBlockOff) t = TileType::InvBlockOn;
      else if (t == TileType::InvBlockOn) t = TileType::InvBlockOff;
    }
  }
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
    case Map::TileType::Bricks:         return true;
    case Map::TileType::BlockOn:        return true;
    case Map::TileType::InvBlockOn:     return true;

    case Map::TileType::Neutral:        return false;

    case Map::TileType::Empty:          return inverted;
    case Map::TileType::Spikes:         return inverted;
    case Map::TileType::DoorBottom:     return inverted;
    case Map::TileType::DoorTop:        return inverted;
    case Map::TileType::BlockOff:       return inverted;
    case Map::TileType::Switch:         return inverted;

    case Map::TileType::InvEmpty:       return !inverted;
    case Map::TileType::InvSpikes:      return !inverted;
    case Map::TileType::InvDoorBottom:  return !inverted;
    case Map::TileType::InvDoorTop:     return !inverted;
    case Map::TileType::InvBlockOff:    return !inverted;
    case Map::TileType::InvSwitch:      return !inverted;
    default: return false;
  }
}

int Map::Tile::sprite() const {
  switch (type) {
    case Map::TileType::Empty:          return 0;
    case Map::TileType::BlockOff:       return 1;
    case Map::TileType::DoorTop:        return 2;
    case Map::TileType::Spikes:         return 3;
    case Map::TileType::Bricks:         return 4;
    case Map::TileType::BlockOn:        return 5;
    case Map::TileType::DoorBottom:     return 6;
    case Map::TileType::Switch:         return 7;
    case Map::TileType::Neutral:        return 8;
    case Map::TileType::InvBlockOn:     return 9;
    case Map::TileType::InvDoorBottom:  return 10;
    case Map::TileType::InvSwitch:      return 11;
    case Map::TileType::InvEmpty:       return 12;
    case Map::TileType::InvBlockOff:    return 13;
    case Map::TileType::InvDoorTop:     return 14;
    case Map::TileType::InvSpikes:      return 15;
    default: return 0;
  }
}
