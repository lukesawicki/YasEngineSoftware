//
// Created by lukesawicki on 9/21/24.
//

#ifndef YAS_APPLICATION_HPP
#define YAS_APPLICATION_HPP

#include <SDL.h>

#include "matrix_4_4.hpp"
#include "pixels_table.hpp"
#include "test_box_3d.hpp"
#include "time_picker.hpp"

class YasApplication {
  public:
    static constexpr int kScreenWidth = 640;
    static constexpr int kScreenHeight = 480;
    static constexpr int kFov = 60; // in Degree;
    static constexpr int kAspectRatio = static_cast<int>(kScreenWidth) / static_cast<int>(kScreenHeight);

    // int z_near_ = -10;
    // int z_far_ = -200;

    int z_near_ = 0.1f;
    int z_far_ = 100;

    static YasApplication *GetInstance() {
      if (instance_ != nullptr) {
        return instance_;
      }
      instance_ = new YasApplication();
      return instance_;
    }

    void Initialize();
    void Run();

  private:
    static YasApplication *instance_;
    int endianness_=-1;
    TimePicker timePicker;
    SDL_Window *window_ = nullptr;
    SDL_Renderer *sdl_renderer_ = nullptr;
    SDL_Texture *screen_texture_ = nullptr;
    PixelsTable *pixels_table_ = nullptr;
    // PixelsTable *back_pixels_table_ = nullptr;
    SDL_Event event_;
    bool quit_ = false;
    double time_ = 0.0;
    double new_time_ = 0.0;
    double delta_time_ = 0.0;
    double fps_ = 0.0;
    double fps_time_ = 0.0;
    unsigned int frames_ = 0.0;

    struct Input {
      bool left_; //  A
      bool right_; // D
      bool up_; // SPACE
      bool down_; // CTRL
      bool forward_; // W
      bool backward_; // S

      // PL -> Przechylenie
      bool rotate_counter_clockwise_roll_;  // Q IN DEFAULT CASE IN YASENGINE AROUND Z
      bool rotate_clockwise_roll_; // E IN DEFAULT CASE IN YASENGINE AROUND Z

      // PL -> Pochylenie
      bool rotate_down_pitch; // DOWN IN DEFAULT CASE IN YASENGINE AROUND X
      bool rotate_up_pitch; // UP IN DEFAULT CASE IN YASENGINE AROUND X

      // PL -> Odchylenie
      bool rotate_left_yaw; // LEFT IN DEFAULT CASE IN YASENGINE AROUND Y
      bool rotate_right_yaw;  // RIGHT IN DEFAULT CASE IN YASENGINE AROUND Y
    };

    struct MousePositionChangeInformation {
      double x_ = 0.0;
      double y_ = 0.0;
      bool mouse_moved_ = false;
      bool left_mouse_button_;
      bool right_mouse_button_;
    };

    Vector4D<float>* camera_position_;

    Matrix_4_4 local_to_world_matrix_;
    Matrix_4_4 world_to_camera_matrix_;
    Matrix_4_4 world_to_projected_world_matrix_;
    Matrix_4_4 rotation;

    Input* input_ = new Input();
    MousePositionChangeInformation* mouse_position_change_information_ = new MousePositionChangeInformation();
    float mouse_x_position_ = false;
    float mouse_y_position_ = false;

    Vector2D<float> test_dynamic_line_point_0;
    Vector2D<float> test_dynamic_line_point_1;
    Vector2D<float> test_static_line_point_0;
    Vector2D<float> test_static_line_point_1;

    TestBox3D test_box_3d;

    void CheckEndianness();

    void PrepareBasicSettings();

    void PrepareRenderingSettings();

    void PrepareWorldSettings();

    void PrepareTestStuff();

    void Update();

    void HandleTestStuff();

    void HandleInput();

    void HandleKeyboardInput();

    void HandleMouseInput();

    void HandleMouseMovement();

    void Render();

    void DrawHudElements();

    void DrawTestStuff();

    void RotateTestLineInToMouseDirection();

    void Clean();

    void EulerRotationInLocalSpace();

    void EulerRotationInLocalSpaceAroundX();

    void EulerRotationInLocalSpaceAroundY();

    void EulerRotationInLocalSpaceAroundZ();

    void LocalToWorldTestBoxTransform();

    void WorldToCameraTestBoxTransform();

    void PerspectiveProjectionTestBoxProcess();

    void Set2dVerticesForTestBox();

    void DrawBoxOnScreen();
};

#endif //YAS_APPLICATION_HPP
