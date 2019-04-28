#include "player.h"

#include <algorithm>
#include <cmath>

Player::Player(bool inverted, double x, double y) :
  Character("chars.png", kHeight, inverted, x, y), text_("text.png"),
  ax_(0), big_jump_(false), fireballs_(false),
  timer_(0), powerup_timer_(0),
  powerup_text_("")
#ifndef NDEBUG
  , xcol_({0, 0, 0, 0}), ycol_({0, 0, 0, 0})
#endif
{}

void Player::update(const Map& map, unsigned int elapsed) {
  timer_ += elapsed;
  if (powerup_timer_ > 0) powerup_timer_ -= elapsed;
  if (fireball_cooldown_ > 0) fireball_cooldown_ -= elapsed;

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

  for (const auto& b : bullets_) {
    b.draw(graphics, xo, yo);
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

void Player::shoot() {
  if (dead_ || !fireballs_ || fireball_cooldown_ > 0) return;
  bullets_.emplace_back(inverted_, x_ + (facing_ == Facing::Left ? -6 : 6), y_ + (inverted_ ? 20 : -20), facing_);
  fireball_cooldown_ = 150;
}

bool Player::on_spikes(const Map& map) const {
  if (dead_) return false;

  const Map::Tile t = map.tile(x_, y_);
  if (t.type == Map::TileType::Spikes) return !inverted_;
  if (t.type == Map::TileType::InvSpikes) return inverted_;
  return false;
}

bool Player::check_fireballs(const Rect& r) const {
  for (auto& b : bullets_) {
    if (b.collision(r)) return true;
  }
  return false;
}

void Player::grant_big_jump() {
  if (big_jump_) return;
  big_jump_ = true;
  powerup("Big Jump");
}

void Player::grant_fireballs() {
  if (fireballs_) return;
  fireballs_ = true;
  powerup("Fireballs");
}

Rect Player::hitbox() const {
  return inverted_ ? Rect(x_ - 4, y_, x_ + 4, y_ + 26) :
    Rect(x_ - 4, y_ - 26, x_ + 4, y_);
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
    bounceh(tile, 0);
  } else {
    x_ += vx_ * elapsed;
  }

  for (auto& fb : bullets_) {
    fb.update(map, elapsed);
  }

  bullets_.erase(std::remove_if(
        bullets_.begin(), bullets_.end(),
        [](const Fireball& b){ return b.dead(); }),
      bullets_.end());
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
    bouncev(tile, 0);
  } else {
    y_ += vy_ * elapsed;
  }
}

void Player::powerup(const std::string& description) {
  powerup_timer_ = 2500;
  powerup_text_ = description;
}

Rect Player::boxh() const {
  return inverted_ ? Rect(x_ - kHalfWidth, y_ + 4, x_ + kHalfWidth, y_ + kHeight - 4) :
    Rect(x_ - kHalfWidth, y_ - kHeight + 4, x_ + kHalfWidth, y_ - 4);
}

Rect Player::boxv() const {
  return inverted_ ? Rect(x_ - kHalfWidth + 4, y_, x_ + kHalfWidth - 4, y_ + kHeight) :
    Rect(x_ - kHalfWidth + 4, y_ - kHeight, x_ + kHalfWidth - 4, y_);
}

int Player::sprite() const {
  if (inverted_ && vy_ > 0) return 7;
  if (!inverted_ && vy_ < 0) return 3;

  if (std::abs(vx_) > 0.01) return (timer_ / 200) % 3 + (inverted_ ? 4 : 0);
  return inverted_ ? 4 : 0;
}
