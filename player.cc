#include "player.h"

#include <cmath>

Player::Player(bool inverted) :
  chars_("chars.png", 4, kWidth, kHeight),
  x_(0), y_(0), vx_(0), vy_(0), ax_(0),
  grounded_(false), dead_(false), inverted_(inverted),
  facing_(Facing::Right)
#ifndef NDEBUG
  , xcol_({0, 0, 0, 0}), ycol_({0, 0, 0, 0})
#endif
{}

void Player::update(const Map& map, unsigned int elapsed) {
  updatex(map, elapsed);
  updatey(map, elapsed);

  vx_ *= kDampen;
}

void Player::draw(Graphics& graphics, int xo, int yo) const {
  const int x = x_ - xo - kHalfWidth;
  const int y = y_ - yo - (inverted_ ? 0 : kHeight);
  chars_.draw_ex(graphics, inverted_ ? 4 : 0, x, y, facing_ == Facing::Left, 0, 0, 0);

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

void Player::set_position(double x, double y) {
  x_ = x;
  y_ = y;
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
  if (grounded()) vy_ += kJumpSpeed * (inverted_ ? 1 : -1);
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
