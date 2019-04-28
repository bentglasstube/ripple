#include "rect.h"

#include <cassert>

Rect::Rect(double l, double t, double r, double b) :
  top(t), left(l), right(r), bottom(b) {
    // causes all kinds of problems if you fuck up the top/bottom left/right
    assert(t < b);
    assert(l < r);
  }

bool Rect::intersect(const Rect& other) const {
  return left < other.right && right > other.left && top < other.bottom && bottom > other.top;
}
