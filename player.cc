#include "player.h"

#include <cmath>

Player::Player(bool inverted) :
  chars_("chars.png", 4, kWidth, kHeight),
  x_(0), y_(0), vx_(0), vy_(0), ax_(0),
  grounded_(false), dead_(false), inverted_(inverted),
  big_jump_(false),
  facing_(Facing::Right)
#ifndef NDEBUG
  , xcol_({0, 0, 0, 0}), ycol_({0, 0, 0, 0})
#endif
{}

void Player::init(double x, double y) {
  x_ = x;
  y_ = y;
  vx_ = vy_ = ax_ = 0;
  dead_ = false;
  grounded_ = false;
  big_jump_ = false;
  facing_ = Facing::Right;
  timer_ = 0;
}

void Player::update(const Map& map, unsigned int elapsed) {
  if (dead_) return;

  timer_ += elapsed;

  updatex(map, elapsed);
  updatey(map, elapsed);

  vx_ *= kDampen;
}

void Player::draw(Graphics& graphics, int xo, int yo) const {
  if (dead_) return;

  const int x = x_ - xo - kHalfWidth;
  const int y = y_ - yo - (inverted_ ? 0 : kHeight);
  chars_.draw_ex(graphics, sprite(), x, y, facing_ == Facing::Left, 0, 0, 0);

#ifndef NDEBUG
  const Rect h = boxh();
  const SDL_Rect hr {
      (int)(h.left - xo),
      (int)(h.top - yo),
      (int)(h.right - h.left),
      (int)(h.bottom - h.top)
      };
  graphics.draw_rect(&hr, 0x0000ffff, false);

  const Rect v = boxv();
  const SDL_Rect vr {
      (int)(v.left - xo),
      (int)(v.top - yo),
      (int)(v.right - v.left),
      (int)(v.bottom - v.top)
      };
  graphics.draw_rect(&vr, 0x0000ffff, false);

  const SDL_Rect crx { xcol_.x - xo, xcol_.y - yo, xcol_.w, xcol_.h };
  const SDL_Rect cry { ycol_.x - xo, ycol_.y - yo, ycol_.w, ycol_.h };
  graphics.draw_rect(&crx, 0xff0000ff, false);
  graphics.draw_rect(&cry, 0x800000ff, false);
#endif
}

double Player::x() const {
  return x_;
}

double Player::y() const {
  return y_;
}

bool Player::grounded() const {
  return grounded_;
}

bool Player::dead() const {
  return dead_;
}

bool Player::done(const Map& map) const {
  if (dead_) return true;
  const Map::Tile t = map.tile(x(), y() + (inverted_ ? 2 : -2));
  if (!inverted_ && t.type == Map::TileType::DoorBottom) return true;
  if (inverted_ && t.type == Map::TileType::InvDoorBottom) return true;
  return false;
}

void Player::move_left() {
  facing_ = Facing::Left;
  ax_ = -kAccel;
}

void Player::move_right() {
  facing_ = Facing::Right;
  ax_ = kAccel;
}

void Player::stop_moving() {
  ax_ = 0;
}

void Player::jump() {
  if (grounded()) {
    vy_ += kJumpSpeed * (inverted_ ? 1 : -1) * (big_jump_ ? 1.3 : 1);
  }
}

bool Player::on_spikes(const Map& map) const {
  const Map::Tile t = map.tile(x_, y_);
  if (t.type == Map::TileType::Spikes) return !inverted_;
  if (t.type == Map::TileType::InvSpikes) return inverted_;
  return false;
}

void Player::kill() {
  dead_ = true;
}

void Player::grant_big_jump() {
  big_jump_ = true;
}

void Player::updatex(const Map& map, unsigned int elapsed) {
  vx_ += ax_ * elapsed;

  Map::Tile tile = map.collision(boxh(), vx_ * elapsed, 0, inverted_);
  if (tile.obstructs(inverted_)) {
#ifndef NDEBUG
    xcol_ = {
      (int) tile.left,
      (int) tile.top,
      (int) (tile.right - tile.left),
      (int) (tile.bottom - tile.top)
    };
#endif
    x_ = vx_ > 0 ? tile.left - kHalfWidth : tile.right + kHalfWidth;
    vx_ = 0;
  } else {
    x_ += vx_ * elapsed;
  }
}

void Player::updatey(const Map& map, unsigned int elapsed) {
  vy_ += kGravity * elapsed * (inverted_ ? -1 : 1);
  grounded_ = false;

  Map::Tile tile = map.collision(boxv(), 0, vy_ * elapsed, inverted_);
  if (tile.obstructs(inverted_)) {
#ifndef NDEBUG
    ycol_ = {
      (int) tile.left,
      (int) tile.top,
      (int) (tile.right - tile.left),
      (int) (tile.bottom - tile.top)
    };
#endif
    if (inverted_) {
      if (vy_ > 0) {
        y_ = tile.top - kHeight;
      } else {
        y_ = tile.bottom;
        grounded_ = true;
      }
    } else {
      if (vy_ > 0) {
        y_ = tile.top;
        grounded_ = true;
      } else {
        y_ = tile.bottom + kHeight;
      }
    }
    vy_ = 0;
  } else {
    y_ += vy_ * elapsed;
  }
}

Rect Player::boxh() const {
  if (inverted_) {
    return Rect(x_ - kHalfWidth, y_ + 4, x_ + kHalfWidth, y_ + kHeight - 4);
  } else {
    return Rect(x_ - kHalfWidth, y_ - kHeight + 4, x_ + kHalfWidth, y_ - 4);
  }
}

Rect Player::boxv() const {
  if (inverted_) {
    return Rect(x_ - kHalfWidth + 2, y_, x_ + kHalfWidth - 2, y_ + kHeight);
  } else {
    return Rect(x_ - kHalfWidth + 2, y_ - kHeight, x_ + kHalfWidth - 2, y_);
  }
}

int Player::sprite() const {
  if (inverted_ && vy_ > 0) return 7;
  if (!inverted_ && vy_ < 0) return 3;

  if (std::abs(vx_) > 0.01) return (timer_ / 200) % 3 + (inverted_ ? 4 : 0);
  return inverted_ ? 4 : 0;
}
