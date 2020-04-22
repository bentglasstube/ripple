#include "camera.h"

double clamp(double val, double min, double max) {
  if (val < min) return min;
  if (val > max) return max;
  return val;
}

Camera::Camera() : ox_(0), oy_(0) {}

void Camera::update(const Character& focus, const Map& map, unsigned int elapsed) {
  const double max = kMaxSpeed * elapsed;

  const double fx = focus.x();
  ox_ = clamp(fx - kWidth / 2, ox_ - max, ox_ + max);
  ox_ = clamp(ox_, 0.0, (double) map.pixel_width() - kWidth);

  const double fy = focus.y();
  if (focus.grounded()) oy_ = clamp(fy - kHeight * 5 / 6, oy_ - max, oy_ + max);
  oy_ = clamp(oy_, 0.0, (double) map.pixel_height() - kHeight);
}

double Camera::xoffset() const {
  return ox_;
}

double Camera::yoffset() const {
  return oy_;
}
