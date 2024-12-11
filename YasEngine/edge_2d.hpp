#ifndef EDGE_4D_HPP
#define EDGE_4D_HPP

template <typename Type>
class Edge2D {
  public:
  Vector2D<Type>* _point0;
  Vector2D<Type>* _point1;

  Edge2D() {
    _point0 = nullptr;
    _point1 = nullptr;
  }

  void AddPoints(Vector2D<Type>* point0, Vector2D<Type>* point1) {
    _point0 = point0;
    _point1 = point1;
  }
};

#endif  // VECTOR_4D_HPP
