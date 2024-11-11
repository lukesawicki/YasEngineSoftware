#ifndef PROJECTOR_HPP
#define PROJECTOR_HPP
#include "matrix_4_4.hpp"
#include "test_box_3d.hpp"

class Projector {
  public:
  Matrix_4_4 projection_;
  Matrix_4_4 worldToCameraMatrix_;
  Matrix_4_4 boxTranslationMatrix_;
  float camera_position_x_=0;
  float camera_position_y_=0;
  float camera_position_z_=0;
  float fov_;
  float aspect_;
  float zNear_;
  float zFar_;
  Projector(float fov, float aspect, float zNear, float zFar, float camera_position_x, float camera_position_y, float camera_position_z) {
    this->fov_ = fov;
    this->aspect_ = aspect;
    this->zNear_ = zNear;
    this->zFar_ = zFar;
    this->camera_position_x_ = camera_position_x;
    this->camera_position_y_ = camera_position_y;
    this->camera_position_z_ = camera_position_z;
    Matrix_4_4::ProjectionMatrix(projection_, fov, aspect, zNear, zFar);
    Matrix_4_4::WorldToCameraMatrix(worldToCameraMatrix_, camera_position_x_, camera_position_y_, camera_position_z_);

  }
  ModifyBoxVertices(Object& box) {

  }

  // create 3 methods:
  // 0 -> to multiply box localVertices by boxTranslationMatrix_ and save to worldVertices
  // 1 -> to multiply box worldVertices by worldToCameraMatrix_ and save to cameraVertices
  // 3 -> to multiply box cameraVertices by projection_ and save in resultVertices
  // 4 -> calculate box vertices in 2d space and save in save vertices_in_2d_
  // 5 ->  draw lines between appropriate 2d vertices from vertices_in_2d_

  ~Projector();
};

#endif //PROJECTOR_HPP
