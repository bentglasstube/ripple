#pragma once

#include "map.h"
#include "character.h"

class Camera {
  public:

    Camera();

    void update(const Character& focus, const Map& map, unsigned int elapsed);
    double xoffset() const;
    double yoffset() const;

  private:

    static constexpr int kWidth = 256;
    static constexpr int kHeight = 240;
    static constexpr int kBuffer = 16;
    static constexpr double kMaxSpeed = 0.3;

    double ox_, oy_;
};
