#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "vector_3d.hpp"
#include "vector_4d.hpp"

class Camera {
 public:
  Vector4D<float> position_;
  float angle_x_;
  float angle_y_;
  float angle_z_;
  Camera() { position_.Set(0.0F, 0.0F, 0.0F, 1.0F);
  }
};

#endif  // CAMERA_HPP
