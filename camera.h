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
    static constexpr double kMaxSpeed = 0.7;

    double ox_, oy_;
};
