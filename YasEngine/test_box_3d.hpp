//
// Created by lukesawicki on 10/12/24.
//

#ifndef TEST_BOX_3D_HPP
#define TEST_BOX_3D_HPP
#include <vector>

#include "vector_2d.hpp"
#include "vector_4d.hpp"

class TestBox3D {
  public:
    Vector4D<float> position;
    Vector4D<float> v0;
    Vector4D<float> v1;
    Vector4D<float> v2;
    Vector4D<float> v3;
    Vector4D<float> v4;
    Vector4D<float> v5;
    Vector4D<float> v6;
    Vector4D<float> v7;
    std::vector<Vector4D<float>*> vertices;
    std::vector<Vector4D<float>*> rotatedvertices;
    std::vector<Vector4D<float>*> worldVertices;
    std::vector<Vector4D<float>*> cameraVertices;
    std::vector<Vector2D<float>*> vertices_in_2d_;
    std::vector<Vector4D<float>*> resultVertices;

    TestBox3D() {
      // vertices = new Vector4D<float>*[8];
      vertices.push_back(new Vector4D<float>(-20, 20, 20, 1));
      vertices.push_back(new Vector4D<float>(20, 20, 20, 1));
      vertices.push_back(new Vector4D<float>(20, 20, -20, 1));
      vertices.push_back(new Vector4D<float>(-20, 20, -20, 1));
      vertices.push_back(new Vector4D<float>(-20, -20, -20, 1));
      vertices.push_back(new Vector4D<float>(-20, -20, 20, 1));
      vertices.push_back(new Vector4D<float>(20, -20, 20, 1));
      vertices.push_back(new Vector4D<float>(20, -20, -20, 1));

      // worldVertices = new Vector4D<float>*[8];
      for (int i = 0; i < 8; i++) {
        worldVertices.push_back(new Vector4D<float>(0, 0, 0, 1));
      }

      // cameraVertices = new Vector4D<float>*[8];
      for (int i = 0; i < 8; i++) {
        cameraVertices.push_back(new Vector4D<float>(0, 0, 0, 1));
      }


      for (int i = 0; i < 8; i++) {
        rotatedvertices.push_back(new Vector4D<float>(0, 0, 0, 1));
      }
      // resultVertices = new Vector4D<float>*[8];
      for (int i = 0; i < 8; i++) {
        resultVertices.push_back(new Vector4D<float>(0, 0, 0, 1));
      }

      for(int i = 0; i < 8; i++) {
        vertices_in_2d_.push_back(new Vector2D<float>(0,0));
      }

      position = Vector4D<float>(0, 0, -120, 1);
    }
};

#endif //TEST_BOX_3D_HPP
