#pragma once

#include <vector>

#include "text.h"

#include "character.h"
#include "fireball.h"

class Player : public Character {
  public:

    Player(bool inverted, double x, double y);

    void update(const Map& map, unsigned int elapsed) override;
    void draw(Graphics& graphics, int xo, int yo) const override;

    bool done(const Map& map) const;

    void move_left();
    void move_right();
    void stop_moving();
    void jump();
    void shoot();

    bool on_spikes(const Map& map) const;
    bool check_fireballs(const Rect& r) const;

    void grant_big_jump();
    void grant_fireballs();

    Rect hitbox() const override;

  private:

    static constexpr double kGravity = 0.003;
    static constexpr double kJumpSpeed = 0.5;
    static constexpr double kAccel = 0.002;
    static constexpr double kDampen = 0.75;

    static constexpr int kWidth = 16;
    static constexpr int kHalfWidth = kWidth / 2;
    static constexpr int kHeight = 32;

    Text text_;
    double ax_;
    bool big_jump_, fireballs_;
    int timer_, powerup_timer_;
    int fireball_cooldown_;
    std::string powerup_text_;
    std::vector<Fireball> bullets_;

#ifndef NDEBUG
    SDL_Rect xcol_, ycol_;
#endif

    void updatex(const Map& map, unsigned int elapsed);
    void updatey(const Map& map, unsigned int elapsed);
    void powerup(const std::string& description);

    Rect boxh() const;
    Rect boxv() const;
    int sprite() const override;
};
