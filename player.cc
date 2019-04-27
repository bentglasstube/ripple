#include "player.h"

#include <cmath>

Player::Player(bool inverted, double x, double y) :
  Character("chars.png", kHeight, inverted), text_("text.png"),
  vx_(0), vy_(0), ax_(0),
  grounded_(false), big_jump_(false),
  timer_(0), powerup_timer_(0),
  powerup_text_("")
#ifndef NDEBUG
  , xcol_({0, 0, 0, 0}), ycol_({0, 0, 0, 0})
#endif
{
  x_ = x;
  y_ = y;
}

void Player::update(const Map& map, unsigned int elapsed) {
  if (powerup_timer_ > 0) powerup_timer_ -= elapsed;
  timer_ += elapsed;

  if (dead_) return;

  updatex(map, elapsed);
  updatey(map, elapsed);

  vx_ *= kDampen;
}

void Player::draw(Graphics& graphics, int xo, int yo) const {
  if (dead_) return;

  Character::draw(graphics, xo, yo);

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

  if (powerup_timer_ > 0) {
    text_.draw(graphics, powerup_text_, 128, inverted_ ? 200 : 24, Text::Alignment::Center);
  }
}

bool Player::grounded() const {
  return grounded_;
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
  if (dead_) return false;

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
  powerup_timer_ = 2500;
  powerup_text_ = "Big Jump";
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
