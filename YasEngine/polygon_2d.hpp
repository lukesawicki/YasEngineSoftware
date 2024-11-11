#ifndef POLYGON_2D_HPP
#define POLYGON_2D_HPP
#include <SDL2/SDL_stdinc.h>

#include "vector_2d.hpp"
#include "vector_4d.hpp"

class Polygon2D {
  public:
  int state_;
  int number_of_vertices_;
  int x0_; // center of polygon
  int y0_; // center of polygon
  int x_velocity_; // initial velocity
  int y_velocity_; // initial velocity
  Vector4D<Uint8> color_;
  Vertex2Df* list_of_vertices;
};

typedef Polygon2D* Polygon2DPointer;

#endif //POLYGON_2D_HPP
