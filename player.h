#pragma once

#include "text.h"

#include "character.h"

class Player : public Character {
  public:

    Player(bool inverted, double x, double y);

    void update(const Map& map, unsigned int elapsed) override;
    void draw(Graphics& graphics, int xo, int yo) const override;

    bool grounded() const;
    bool done(const Map& map) const;

    void move_left();
    void move_right();
    void stop_moving();
    void jump();

    bool on_spikes(const Map& map) const;

    void kill();
    void grant_big_jump();

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
    double vx_, vy_, ax_;
    bool grounded_, big_jump_;
    int timer_, powerup_timer_;
    std::string powerup_text_;

#ifndef NDEBUG
    SDL_Rect xcol_, ycol_;
#endif

    void updatex(const Map& map, unsigned int elapsed);
    void updatey(const Map& map, unsigned int elapsed);
    Rect boxh() const;
    Rect boxv() const;
    int sprite() const override;
};
