#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <vector>
#include <iostream>
#include "edge_2d.hpp"
#include "vector_2d.hpp"
#include "vector_4d.hpp"

class GraphicObject {
 public:
  Vector4D<float> position_;
  Vector4D<float> v0_;
  Vector4D<float> v1_;
  Vector4D<float> v2_;
  Vector4D<float> v3_;
  Vector4D<float> v5_;
  Vector4D<float> v6_;
  Vector4D<float> v7_;
  std::vector<Vector4D<float>*> vertices_;
  std::vector<Vector4D<float>*> rotated_vertices_;
  std::vector<Vector4D<float>*> world_vertices_;
  std::vector<Vector4D<float>*> translated_as_a_result_of_camera_translation_vertices_;
  std::vector<Vector4D<float>*> rotated_as_a_result_of_camera_rotation_vertices_;

  std::vector<Vector2D<float>*> vertices_in_2d_;
  std::vector<Vector4D<float>*> resultVertices_;

  std::vector<Edge2D<float>> edges_in_2d;
  // DODAC LISTE INDEKSOW TROJKATOW ONE MAJA SIE ODNOSIC DO resultVertices_
  GraphicObject() {
    // vertices_ = new Vector4D<float>*[8];
    std::cout << "CALLED CONSTRUCTOR  ON GraphicsObject"
              << "\n";
    vertices_.push_back(new Vector4D<float>(-20, 20, 20, 1));
    vertices_.push_back(new Vector4D<float>(20, 20, 20, 1));
    vertices_.push_back(new Vector4D<float>(20, 20, -20, 1));
    vertices_.push_back(new Vector4D<float>(-20, 20, -20, 1));
    vertices_.push_back(new Vector4D<float>(-20, -20, -20, 1));
    vertices_.push_back(new Vector4D<float>(-20, -20, 20, 1));
    vertices_.push_back(new Vector4D<float>(20, -20, 20, 1));
    vertices_.push_back(new Vector4D<float>(20, -20, -20, 1));

    // world_vertices_ = new Vector4D<float>*[8];
    for (int i = 0; i < 8; i++) {
      world_vertices_.push_back(new Vector4D<float>(0, 0, 0, 1));
    }

    // translated_as_a_result_of_camera_translation_vertices_ = new Vector4D<float>*[8];
    for (int i = 0; i < 8; i++) {
      translated_as_a_result_of_camera_translation_vertices_.push_back(new Vector4D<float>(0, 0, 0, 1));
    }

    for (int i = 0; i < 8; i++) {
      rotated_as_a_result_of_camera_rotation_vertices_.push_back(new Vector4D<float>(0, 0, 0, 1));
    }

    for (int i = 0; i < 8; i++) {
      rotated_vertices_.push_back(new Vector4D<float>(0, 0, 0, 1));
    }
    // resultVertices_ = new Vector4D<float>*[8];
    for (int i = 0; i < 8; i++) {
      resultVertices_.push_back(new Vector4D<float>(0, 0, 0, 1));
    }

    for (int i = 0; i < 8; i++) {
      vertices_in_2d_.push_back(new Vector2D<float>(0, 0));
    }

    for (int i = 0; i < 12; i++) {
      edges_in_2d.push_back(Edge2D<float>());
    }

    position_.Set(0, 0, -120, 1);
  }
};

#endif //OBJECT_HPP
