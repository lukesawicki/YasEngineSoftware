//
// Created by lukesawicki on 9/29/24.
//

#ifndef POINT_3D_HPP
#define POINT_3D_HPP
#include <SDL2/SDL_stdinc.h>

#include "vector_3d.hpp"
#include "vector_4d.hpp"

class Point3D {
  public:
  Vector3D<float> position_;
  Vector4D<Uint8> color_;
};

typedef Point3D* Point3Dpointer;

#endif //POINT_3D_HPP
