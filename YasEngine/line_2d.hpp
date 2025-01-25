#ifndef LINE_2D_HPP
#define LINE_2D_HPP

#include <cmath>

template <typename Type>
class Line2D {
 public:
  Vector2D<Type> point_0_;
  Vector2D<Type> point_1_;

  Line2D() {
  }

  Line2D(const Vector2D<Type>& point_0, const Vector2D<Type>& point_1) {
    this.point_0_.x = point_0.x;
    this.point_0_.y = point_0.y;

    this.point_1_.x = point_1.x;
    this.point_1_.y = point_1.y;
  }

};

#endif
