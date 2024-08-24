#ifndef COLLIDER_HPP
#define COLLIDER_HPP
#include "vector_2d.hpp"

class Collider {
 public:
  float radius_ = 0.0F;
  float x_ = 0.0F;
  float y_ = 0.0F;
  Collider();
  Collider(float radius, float x, float y);
  bool is_in_collision_ = false;

  static bool isCollision(const Collider& object0, const Collider& object1);
  static bool isCollidingWithWall(const Collider& object0,
                                  const Vector2D<int>& worldSizes);
  static bool isCollidingWithCustomWalls(const Collider& object0,
                                         const Vector2D<int>& worldSizes);

 private:
};

#endif