#include "rect.h"

#include <cassert>

Rect::Rect(double l, double t, double r, double b) :
  top(t), left(l), right(r), bottom(b) {
    assert(t < b);
    assert(l < r);
  }
