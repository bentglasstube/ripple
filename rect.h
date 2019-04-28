#pragma once

struct Rect {
  Rect(double l, double t, double r, double b);
  double top, left, right, bottom;
  bool intersect(const Rect& other) const;
};
