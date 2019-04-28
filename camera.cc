#include "camera.h"

#include <algorithm>

Camera::Camera() : ox_(0), oy_(0) {}

void Camera::update(const Character& focus, const Map& map, unsigned int elapsed) {
  const double max = kMaxSpeed * elapsed;

  const double fx = focus.x();
  ox_ = std::clamp(fx - kWidth / 2, ox_ - max, ox_ + max);
  ox_ = std::clamp(ox_, 0.0, (double) map.pixel_width() - kWidth);

  const double fy = focus.y();
  if (focus.grounded()) oy_ = std::clamp(fy - kHeight * 5 / 6, oy_ - max, oy_ + max);
  oy_ = std::clamp(oy_, 0.0, (double) map.pixel_height() - kHeight);
}

double Camera::xoffset() const {
  return ox_;
}

double Camera::yoffset() const {
  return oy_;
}
