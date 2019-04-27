#pragma once

#include "graphics.h"
#include "spritemap.h"

#include "map.h"
#include "rect.h"

class Player {
  public:

    Player(bool inverted);

    void init(double x, double y);
    void update(const Map& map, unsigned int elapsed);
    void draw(Graphics& graphics, int xo, int yo) const;

    double x() const;
    double y() const;
    bool grounded() const;
    bool dead() const;
    bool done(const Map& map) const;

    void move_left();
    void move_right();
    void stop_moving();
    void jump();

    bool on_spikes(const Map& map) const;

    void kill();
    void grant_big_jump();

  private:

    static constexpr double kGravity = 0.003;
    static constexpr double kJumpSpeed = 0.5;
    static constexpr double kAccel = 0.002;
    static constexpr double kDampen = 0.75;

    static constexpr int kWidth = 16;
    static constexpr int kHalfWidth = kWidth / 2;
    static constexpr int kHeight = 32;

    enum class Facing { Right, Left };

    SpriteMap chars_;
    double x_, y_, vx_, vy_, ax_;
    bool grounded_, dead_, inverted_, big_jump_;
    Facing facing_;
    int timer_;

#ifndef NDEBUG
    SDL_Rect xcol_, ycol_;
#endif

    void updatex(const Map& map, unsigned int elapsed);
    void updatey(const Map& map, unsigned int elapsed);
    Rect boxh() const;
    Rect boxv() const;
    int sprite() const;
};
