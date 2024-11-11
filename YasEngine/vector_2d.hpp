#ifndef VECTOR_2D_HPP
#define VECTOR_2D_HPP

#include <cmath>

template<typename Type>
class Vector2D {
  public:
    Type x_;
    Type y_;

    Vector2D() {
      this->x_ = 0;
      this->y_ = 0;
    }

    Vector2D(Type x, Type y) {
      this->x_ = x;
      this->y_ = y;
    }

    static void NormalizedVector(Vector2D<Type> &vector) {
      double magnitude = GetVectorMagnitude(vector);
      double x = vector.x_ / magnitude;
      double y = vector.y_ / magnitude;
      vector.x_ = static_cast<Type>(x);
      vector.y_ = static_cast<Type>(y);
    }

    static Type GetVectorMagnitude(const Vector2D<Type> &vector) {
      return static_cast<Type>(sqrt(pow(vector.x_, 2.0) + pow(vector.y_, 2.0)));
    }

    static Type GetVectorMagnitude(Type x0, Type y0, Type x1, Type y1) {
      return static_cast<Type>(sqrt(pow(x1 - x0, 2.0) + pow(y1 - y0, 2.0)));
    }

    static Type CrossProduct(const Vector2D<Type> &u, const Vector2D &v) {
      return u.x_ * v.y_ - u.y_ * v.x_;
    }

    static Type DotProduct(const Vector2D<Type> &u, const Vector2D &v) {
      return u.x_ * v.x_ + u.y_ * v.y_;
    }

    static Type
    AngleBetweenVectors(const Vector2D<Type> &u, const Vector2D &v) {
      return atan2(CrossProduct(u, v), DotProduct(u, v));
    }

    static void RotateVectorOverTheAngle(Vector2D<Type> *v, float angle) {
      float modifiedX = v->x_ * cos(angle) - v->y_ * sin(angle);
      float modifiedY = v->x_ * sin(angle) + v->y_ * cos(angle);

      v->x_ = modifiedX;
      v->y_ = modifiedY;
    }

    static void RotateVectorOverTheAngleOverPoint(
      Vector2D<Type> *u, float angle, Vector2D<Type> *point) {
      Vector2D<float> direction_vector_from_point_to_vector_u =
          DirectionVectorFromBoundVector(*point, *u);

      Vector2D<float> rotated_vector;
      rotated_vector.x_ = point->x_ + direction_vector_from_point_to_vector_u.x_
                          * cos(angle) - direction_vector_from_point_to_vector_u
                          .y_ * sin(angle);
      rotated_vector.y_ = point->y_ + direction_vector_from_point_to_vector_u.x_
                          * sin(angle) + direction_vector_from_point_to_vector_u
                          .y_ * cos(angle);

      u->x_ = rotated_vector.x_;
      u->y_ = rotated_vector.y_;
    }

    static Vector2D<Type> CreateUnitVectorFromBoundVector(
      const Vector2D<Type> &u,
      const Vector2D &v) {
      Vector2D<Type> w = Vector2D<Type>(v.x_ - u.x_, v.y_ - u.y_);
      Vector2D::NormalizedVector(w);
      return w;
    }

    static Vector2D<Type> DirectionVectorFromBoundVector(
      const Vector2D<Type> &u,
      const Vector2D &v) {
      Vector2D<Type> w = Vector2D<Type>(v.x_ - u.x_, v.y_ - u.y_);
      return w;
    }

    // minuend => odjemna  - subtrahed => odjemnik
    static void Substract(Vector2D<Type> *minued,
                          const Vector2D<Type> &subtrahed) {
      minued->x_ = minued->x_ - subtrahed.x_;
      minued->y_ = minued->y_ - subtrahed.y_;
    }

    static Vector2D<Type> Substract(const Vector2D<Type> &minued,
                                    const Vector2D<Type> &subtrahed) {
      Vector2D<Type> result;
      result.x_ = minued.x_ - subtrahed.x_;
      result.y_ = minued.y_ - subtrahed.y_;
      return result;
    }

    static void MultiplyByScalar(Vector2D<Type> *v, Type scalar) {
      v->x_ = v->x_ * scalar;
      v->y_ = v->y_ * scalar;
    }
};

typedef Vector2D<float> Vertex2Df;
typedef Vector2D<int> Vertex2Di;
typedef Vector2D<float>* Vertex2DfPointer;
typedef Vector2D<int>* Vertex2DiPointer;

#endif //VECTOR_2D_HPP
