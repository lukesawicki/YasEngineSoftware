#ifndef VECTOR_4D_HPP
#define VECTOR_4D_HPP

template<typename Type>
class Vector4D {
public:
  Type x_;
  Type y_;
  Type z_;
  Type w_;

  Vector4D() {
    this->x_ = 0;
    this->y_ = 0;
    this->z_ = 0;
    this->w_ = 0;
  }

  Vector4D(Type x, Type y, Type z, Type w) {
    this->x_ = x;
    this->y_ = y;
    this->z_ = z;
    this->w_ = w;
  }

  void Set(Type x, Type y, Type z, Type w) {
    this->x_ = x;
    this->y_ = y;
    this->z_ = z;
    this->w_ = w;
  }

  void Set(Vector4D<Type>* vector_4d) {
    this->x_ = vector_4d->x_;
    this->y_ = vector_4d->y_;
    this->z_ = vector_4d->z_;
    this->w_ = vector_4d->w_;
  }

};

#endif //VECTOR_4D_HPP
