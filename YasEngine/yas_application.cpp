#include <bit>
#include <iostream>

#include "yas_application.hpp"

#include <ranges>

#include "renderer.hpp"

YasApplication* YasApplication::instance_ = nullptr;

void YasApplication::Initialize() {
  PrepareBasicSettings();
  PrepareRenderingSettings();
  PrepareWorldSettings();
  PrepareTestStuff();
}

void YasApplication::CheckEndianness() {
  if constexpr (std::endian::native == std::endian::big) {
    std::cout << "BIG ENDIAN" << std::endl;
    endianness_ = 0;
  }
  else {
    if (std::endian::native == std::endian::little) {
      std::cout << "LITTLE ENDIAN" << std::endl;
      endianness_ = 1;
    }
    else {
      std::cout << "MIXED ENDIAN" << std::endl;
      endianness_ = 2;
    }
  }
}

void YasApplication::PrepareBasicSettings() {
  CheckEndianness();

  if (SDL_Init( SDL_INIT_EVENTS | SDL_INIT_AUDIO |
    SDL_INIT_VIDEO) != 0) {
    std::cout << "SDL_Init failed with error: " << SDL_GetError() << std::endl;
  }
  else {
    std::cout << "SDL_Init finished successfully" << std::endl;
  }

  SDL_DisplayID display_id = SDL_GetPrimaryDisplay();
  const SDL_DisplayMode* display_mode = SDL_GetCurrentDisplayMode(display_id);

  Uint32 windowFlags = SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_RESIZABLE |
    SDL_WINDOW_OPENGL; // SDL_WINDOW_ALWAYS_ON_TOP // SDL_WINDOW_BORDERLESS

  window_ =
      SDL_CreateWindow("YasEngine", kScreenWidth, kScreenHeight, windowFlags);

  SDL_SetWindowMinimumSize(window_, kScreenWidth, kScreenHeight);
  SDL_HideCursor();
}

void YasApplication::PrepareRenderingSettings() {
  pixels_table_ = new PixelsTable(kScreenWidth, kScreenHeight, kBlack);
  sdl_renderer_ = SDL_CreateRenderer(window_, NULL);
  SDL_SetRenderLogicalPresentation(sdl_renderer_, kScreenWidth, kScreenHeight,
                                   SDL_LOGICAL_PRESENTATION_DISABLED);

  screen_texture_ = SDL_CreateTexture(sdl_renderer_, SDL_PIXELFORMAT_RGBX32,
                                      SDL_TEXTUREACCESS_STREAMING, kScreenWidth,
                                      kScreenHeight);
}

void YasApplication::PrepareWorldSettings() {
  camera_.position_.Set(20.0F, 64.0F, 0.0F, 1.0F);

  for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
    test_box_3d.rotated_vertices_[i]->Set(test_box_3d.vertices_[i]);
  }

  test_box_3d.edges_in_2d.at(0).AddPoints(test_box_3d.vertices_in_2d_[0], test_box_3d.vertices_in_2d_[1]);
  test_box_3d.edges_in_2d.at(1).AddPoints(test_box_3d.vertices_in_2d_[1], test_box_3d.vertices_in_2d_[2]);
  test_box_3d.edges_in_2d.at(2).AddPoints(test_box_3d.vertices_in_2d_[2], test_box_3d.vertices_in_2d_[3]);
  test_box_3d.edges_in_2d.at(3).AddPoints(test_box_3d.vertices_in_2d_[3], test_box_3d.vertices_in_2d_[0]);

  test_box_3d.edges_in_2d.at(4).AddPoints(test_box_3d.vertices_in_2d_[4], test_box_3d.vertices_in_2d_[5]);
  test_box_3d.edges_in_2d.at(5).AddPoints(test_box_3d.vertices_in_2d_[5], test_box_3d.vertices_in_2d_[6]);
  test_box_3d.edges_in_2d.at(6).AddPoints(test_box_3d.vertices_in_2d_[6], test_box_3d.vertices_in_2d_[7]);
  test_box_3d.edges_in_2d.at(7).AddPoints(test_box_3d.vertices_in_2d_[7], test_box_3d.vertices_in_2d_[4]);

  test_box_3d.edges_in_2d.at(8).AddPoints(test_box_3d.vertices_in_2d_[5], test_box_3d.vertices_in_2d_[0]);
  test_box_3d.edges_in_2d.at(9).AddPoints(test_box_3d.vertices_in_2d_[6], test_box_3d.vertices_in_2d_[1]);
  test_box_3d.edges_in_2d.at(10).AddPoints(test_box_3d.vertices_in_2d_[7], test_box_3d.vertices_in_2d_[2]);
  test_box_3d.edges_in_2d.at(11).AddPoints(test_box_3d.vertices_in_2d_[4], test_box_3d.vertices_in_2d_[3]);


}

void YasApplication::PrepareTestStuff() {
  test_dynamic_line_point_1.x_ = 80;
  test_dynamic_line_point_1.y_ = 80;
  test_dynamic_line_point_0.x_ = 160;
  test_dynamic_line_point_0.y_ = 160;

  test_static_line_point_0.x_ = 0;
  test_static_line_point_0.y_ = 0;
  test_static_line_point_1.x_ = kScreenWidth;
  test_static_line_point_1.y_ = kScreenHeight;
}

void YasApplication::Update() {
  HandleTestStuff();

  if (input_->left_) {
    test_box_3d.position_.x_ -= 0.05;
  }

  if (input_->right_) {
    test_box_3d.position_.x_ += 0.05;
  }

  if (input_->up_) {
    test_box_3d.position_.y_ += 0.05;
  }

  if (input_->down_) {
    test_box_3d.position_.y_ -= 0.05;
  }

    if (input_->forward_) {
    test_box_3d.position_.z_ -= 0.05;
  }

  if (input_->backward_) {
    test_box_3d.position_.z_ += 0.05;
  }

  HandleCameraMoving();

  if (input_->camera_rotate_down_pitch) {
    camera_.angle_x_ += 1.0F;
  }

  if (input_->camera_rotate_up_pitch) {
    camera_.angle_x_ -= 1.0F;
  }



  if (input_->camera_rotate_left_yaw) {
    camera_.angle_y_ -= 1.0F;
  }

  if (input_->camera_rotate_right_yaw) {
    camera_.angle_y_ += 1.0F;
  }



  if (input_->camera_rotate_counter_clockwise_roll_) {
    camera_.angle_z_ -= 1.0F;
  }

  if (input_->camera_rotate_clockwise_roll_) {
    camera_.angle_z_ += 1.0F;
  }



  EulerRotationInLocalSpace();

  LocalToWorldTestBoxTransform();
  WorldToCameraTranslationTestBoxTransform();
  WorldToCameraRotationTestBoxTransform();

  PerspectiveProjectionTestBoxProcess();
  Set2dVerticesForTestBox();
}

void YasApplication::HandleTestStuff() {
  if (mouse_position_change_information_->mouse_moved_) {
    RotateTestLineInToMouseDirection();
  }
}

void YasApplication::HandleCameraMoving() {
  if (input_->camera_left_) {
    camera_.position_.x_ -= 0.05;
  }

  if (input_->camera_right_) {
    camera_.position_.x_ += 0.05;
  }

  if (input_->camera_up_) {
    camera_.position_.y_ += 0.05;
  }

  if (input_->camera_down_) {
    camera_.position_.y_ -= 0.05;
  }

  if (input_->camera_forward_) {
    camera_.position_.z_ -= 0.05;
  }

  if (input_->camera_backward_) {
    camera_.position_.z_ += 0.05;
  }
}

void YasApplication::HandleInput() {
  if (event_.type == SDL_EVENT_QUIT) {
    quit_ = true;
  }
  else {
    HandleKeyboardInput();
    HandleMouseInput();
  }
}

void YasApplication::HandleKeyboardInput() {
  if (event_.type == SDL_EVENT_KEY_DOWN) {
    switch (event_.key.key) {
    case SDLK_ESCAPE:
      quit_ = true;
      break;
    case SDLK_W:
      input_->forward_ = true;
      break;
    case SDLK_S:
      input_->backward_ = true;
      break;
    case SDLK_A:
      input_->left_ = true;
      break;
    case SDLK_D:
      input_->right_ = true;
      break;
    case SDLK_Q:
      input_->rotate_counter_clockwise_roll_ = true;
      break;
    case SDLK_E:
      input_->rotate_clockwise_roll_ = true;
      break;
    case SDLK_UP:
      input_->rotate_up_pitch = true;
      break;
    case SDLK_DOWN:
      input_->rotate_down_pitch = true;
      break;
    case SDLK_LEFT:
      input_->rotate_left_yaw = true;
      break;
    case SDLK_RIGHT:
      input_->rotate_right_yaw = true;
      break;
    case SDLK_SPACE:
      input_->up_ = true;
      break;
    case SDLK_LCTRL:
      input_->down_ = true;
      break;

    case SDLK_T:
      input_->camera_forward_ = true;
      break;
    case SDLK_G:
      input_->camera_backward_ = true;
      break;
    case SDLK_F:
      input_->camera_left_ = true;
      break;
    case SDLK_H:
      input_->camera_right_ = true;
      break;
    case SDLK_R:
      input_->camera_rotate_counter_clockwise_roll_ = true;
      break;
    case SDLK_Y:
      input_->camera_rotate_clockwise_roll_ = true;
      break;
    case SDLK_I:
      input_->camera_rotate_up_pitch = true;
      break;
    case SDLK_K:
      input_->camera_rotate_down_pitch = true;
      break;
    case SDLK_J:
      input_->camera_rotate_left_yaw = true;
      break;
    case SDLK_L:
      input_->camera_rotate_right_yaw = true;
      break;
    case SDLK_M:
      input_->camera_up_ = true;
      break;
    case SDLK_N:
      input_->camera_down_ = true;
      break;
    case SDLK_RETURN:
      ;
      break;
    case SDLK_TAB:
      ;
      break;
    default:
      ;
    }
  }

  if (event_.type == SDL_EVENT_KEY_UP) {
    switch (event_.key.key) {
      case SDLK_W:
        input_->forward_ = false;
        break;
      case SDLK_S:
        input_->backward_ = false;
        break;
      case SDLK_A:
        input_->left_ = false;
        break;
      case SDLK_D:
        input_->right_ = false;
        break;
      case SDLK_Q:
        input_->rotate_counter_clockwise_roll_ = false;
        break;
      case SDLK_E:
        input_->rotate_clockwise_roll_ = false;
        break;
      case SDLK_UP:
        input_->rotate_up_pitch = false;
        break;
      case SDLK_DOWN:
        input_->rotate_down_pitch = false;
        break;
      case SDLK_LEFT:
        input_->rotate_left_yaw = false;
        break;
      case SDLK_RIGHT:
        input_->rotate_right_yaw = false;
        break;
      case SDLK_SPACE:
        input_->up_ = false;
        break;
      case SDLK_LCTRL:
        input_->down_ = false;
        break;

      case SDLK_T:
        input_->camera_forward_ = false;
        break;
      case SDLK_G:
        input_->camera_backward_ = false;
        break;
      case SDLK_F:
        input_->camera_left_ = false;
        break;
      case SDLK_H:
        input_->camera_right_ = false;
        break;
      case SDLK_R:
        input_->camera_rotate_counter_clockwise_roll_ = false;
        break;
      case SDLK_Y:
        input_->camera_rotate_clockwise_roll_ = false;
        break;
      case SDLK_I:
        input_->camera_rotate_up_pitch = false;
        break;
      case SDLK_K:
        input_->camera_rotate_down_pitch = false;
        break;
      case SDLK_J:
        input_->camera_rotate_left_yaw = false;
        break;
      case SDLK_L:
        input_->camera_rotate_right_yaw = false;
        break;
      case SDLK_M:
        input_->camera_up_ = false;
        break;
      case SDLK_N:
        input_->camera_down_ = false;
        break;
    default:
      ;
    }
  }
}

void YasApplication::HandleMouseInput() {
  if (event_.type == SDL_EVENT_MOUSE_MOTION) {
    HandleMouseMovement();
  }
  if (event_.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
      event_.button.button ==
    SDL_BUTTON_LEFT) {
    ;
  }

  if (event_.type == SDL_EVENT_MOUSE_BUTTON_UP &&
      event_.button.button ==
    SDL_BUTTON_LEFT) {
    ;
  }
}

void YasApplication::HandleMouseMovement() {
  float x;
  float y;
  SDL_GetMouseState(&x, &y);
  mouse_position_change_information_->mouse_moved_ = true;
  mouse_position_change_information_->x_ = x;
  mouse_position_change_information_->y_ = y;
  mouse_x_position_ = static_cast<float>(mouse_position_change_information_->
    x_);
  mouse_y_position_ = static_cast<float>(mouse_position_change_information_->
    y_);

  WindowPositionToCartesianPosition(mouse_x_position_, mouse_y_position_,
                                    kScreenWidth, kScreenHeight);
}

void YasApplication::Render() {
  pixels_table_->ClearColor(kBlack);

  DrawHudElements();
  DrawGraphicObjects();
  SDL_UpdateTexture(screen_texture_, NULL, pixels_table_->pixels_,
                    kScreenWidth * 4);
  SDL_RenderTexture(sdl_renderer_, screen_texture_, NULL, NULL);
  SDL_RenderPresent(sdl_renderer_);
}

void YasApplication::DrawHudElements() {
  DrawCrossHair(mouse_x_position_, mouse_y_position_, *pixels_table_, false,
                kGreen);
  DrawTestStuff();
}

void YasApplication::DrawTestStuff() {
  DrawLine(test_dynamic_line_point_0, test_dynamic_line_point_1, *pixels_table_,
           kYellow);
  DrawLine(test_static_line_point_0, test_static_line_point_1, *pixels_table_,
           kRed);
}

void YasApplication::RotateTestLineInToMouseDirection() {
  Vector2D<float> currentMousePosition = Vector2D<float>(
    mouse_x_position_, mouse_y_position_);

  Vector2D<float> mouse_direction_from_start_start_line_point = Vector2D<
    float>::DirectionVectorFromBoundVector(test_dynamic_line_point_0,
                                           currentMousePosition);

  Vector2D<float> line_direction_vector = Vector2D<
    float>::DirectionVectorFromBoundVector(test_dynamic_line_point_0,
                                           test_dynamic_line_point_1);

  float angleBetweenCurrentAndMouse = Vector2D<float>::AngleBetweenVectors(
    line_direction_vector, mouse_direction_from_start_start_line_point);

  Vector2D<float>::RotateVectorOverTheAngleOverPoint(&test_dynamic_line_point_1,
                                                     angleBetweenCurrentAndMouse, &test_dynamic_line_point_0);
}

void YasApplication::Run() {
  TimePicker timePicker = TimePicker();
  time_ = timePicker.getSeconds();
  fps_time_ = 0.0F;
  frames_ = 0;

  while (!quit_) {
    while (SDL_PollEvent(&event_)) {
      HandleInput();
    }

    new_time_ = timePicker.getSeconds();
    delta_time_ = new_time_ - time_;
    time_ = new_time_;

    ++frames_;
    fps_time_ = fps_time_ + delta_time_;
    if (fps_time_ >= 1.0F) {
      fps_ = frames_ / fps_time_;
      frames_ = 0;
      fps_time_ = 0.0F;
    }

    Update();
    Render();
  }

  Clean();
}

void YasApplication::Clean() {
  delete pixels_table_;
  SDL_DestroyTexture(screen_texture_);
  SDL_DestroyRenderer(sdl_renderer_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

void YasApplication::EulerRotationInLocalSpace() {
  EulerRotationInLocalSpaceAroundX();
  EulerRotationInLocalSpaceAroundY();
  EulerRotationInLocalSpaceAroundZ();
}

void YasApplication::EulerRotationInLocalSpaceAroundX() {
  if (input_->rotate_down_pitch) {
    Matrix_4_4::RotationAroundX(rotation, -1.0f);
    for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
      Matrix_4_4::MultiplyByVector4D(rotation, test_box_3d.vertices_[i],
                                     test_box_3d.rotated_vertices_[i]);
    }
  }

  if (input_->rotate_up_pitch) {
    Matrix_4_4::RotationAroundX(rotation, 1.0f);
    for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
      Matrix_4_4::MultiplyByVector4D(rotation, test_box_3d.vertices_[i],
                                     test_box_3d.rotated_vertices_[i]);
    }
  }

  for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
    test_box_3d.vertices_[i]->Set(test_box_3d.rotated_vertices_[i]);
  }
}

void YasApplication::EulerRotationInLocalSpaceAroundY() {
  if (input_->rotate_left_yaw) {
    Matrix_4_4::RotationAroundY(rotation, 1.0f);
    for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
      Matrix_4_4::MultiplyByVector4D(rotation, test_box_3d.vertices_[i],
                                     test_box_3d.rotated_vertices_[i]);
    }
  }

  if (input_->rotate_right_yaw) {
    Matrix_4_4::RotationAroundY(rotation, -1.0f);
    for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
      Matrix_4_4::MultiplyByVector4D(rotation, test_box_3d.vertices_[i],
                                     test_box_3d.rotated_vertices_[i]);
    }
  }

  for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
    test_box_3d.vertices_[i]->Set(test_box_3d.rotated_vertices_[i]);
  }
}

void YasApplication::EulerRotationInLocalSpaceAroundZ() {
  if (input_->rotate_counter_clockwise_roll_) {
    Matrix_4_4::RotationAroundZ(rotation, 1.0f);
    for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
      Matrix_4_4::MultiplyByVector4D(rotation, test_box_3d.vertices_[i],
                                     test_box_3d.rotated_vertices_[i]);
    }
  }

  if (input_->rotate_clockwise_roll_) {
    Matrix_4_4::RotationAroundZ(rotation, -1.0f);
    for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
      Matrix_4_4::MultiplyByVector4D(rotation, test_box_3d.vertices_[i],
                                     test_box_3d.rotated_vertices_[i]);
    }
  }

  for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
    test_box_3d.vertices_[i]->Set(test_box_3d.rotated_vertices_[i]);
  }
}

void YasApplication::LocalToWorldTestBoxTransform() {
  Matrix_4_4::TranslationMatrix(local_to_world_matrix_, test_box_3d.position_.x_, test_box_3d.position_.y_,
                                test_box_3d.position_.z_);
  for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
    Matrix_4_4::MultiplyByVector4D(local_to_world_matrix_, test_box_3d.vertices_[i],
                                   test_box_3d.world_vertices_[i]);
  }
}

void YasApplication::WorldToCameraTranslationTestBoxTransform() {
  Matrix_4_4::TranslationMatrix(world_to_camera_matrix_, -camera_.position_.x_, -camera_.position_.y_,
                                -camera_.position_.z_);
  for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
    Matrix_4_4::MultiplyByVector4D(world_to_camera_matrix_, test_box_3d.world_vertices_[i],
                                   test_box_3d.translated_as_a_result_of_camera_translation_vertices_[i]);
  }
}
///////////////////////////////////////////////////////////////////////////////
void YasApplication::EulerCameraRotationAroundX() {

    // TUTAJ ZMIENIC ZE JEST NIE -1.0f A JEST KAT OBROTU KAMERY Z OBIEKTU KAMERA
    Matrix_4_4::RotationAroundX(camera_rotation_around_x_, camera_.angle_x_);
    for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
      Matrix_4_4::MultiplyByVector4D_V2(
          camera_rotation_around_x_,
          test_box_3d.translated_as_a_result_of_camera_translation_vertices_[i],
          test_box_3d.rotated_as_a_result_of_camera_rotation_vertices_[i]);
          // test_box_3d.translated_as_a_result_of_camera_translation_vertices_[i]);
    }
  
    for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
    test_box_3d.translated_as_a_result_of_camera_translation_vertices_[i]->Set(
        test_box_3d.rotated_as_a_result_of_camera_rotation_vertices_[i]);
  }
}

void YasApplication::EulerCameraRotationAroundY() {
  
    Matrix_4_4::RotationAroundY(camera_rotation_around_y_, camera_.angle_y_);
    for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
      Matrix_4_4::MultiplyByVector4D_V2(
          camera_rotation_around_y_,
          test_box_3d.translated_as_a_result_of_camera_translation_vertices_[i],
          test_box_3d.rotated_as_a_result_of_camera_rotation_vertices_[i]);
          // test_box_3d.translated_as_a_result_of_camera_translation_vertices_[i]);
    }

  for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
    test_box_3d.translated_as_a_result_of_camera_translation_vertices_[i]->Set(
        test_box_3d.rotated_as_a_result_of_camera_rotation_vertices_[i]);
  }
}

void YasApplication::EulerCameraRotationAroundZ() {
    Matrix_4_4::RotationAroundZ(camera_rotation_around_z_, camera_.angle_z_);
    for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
      Matrix_4_4::MultiplyByVector4D_V2(
          camera_rotation_around_z_,
          test_box_3d.translated_as_a_result_of_camera_translation_vertices_[i],
          test_box_3d.rotated_as_a_result_of_camera_rotation_vertices_[i]);
          // test_box_3d.translated_as_a_result_of_camera_translation_vertices_[i]);
    }

  for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
    test_box_3d.translated_as_a_result_of_camera_translation_vertices_[i]->Set(
        test_box_3d.rotated_as_a_result_of_camera_rotation_vertices_[i]);
  }
}
///////////////////////////////////////////////////////////////////////////////
void YasApplication::WorldToCameraRotationTestBoxTransform() {
  for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
    test_box_3d.rotated_as_a_result_of_camera_rotation_vertices_[i]->Set(
        test_box_3d.translated_as_a_result_of_camera_translation_vertices_[i]);
  }
  EulerCameraRotationAroundX();
  EulerCameraRotationAroundY();
  EulerCameraRotationAroundZ();
}

void YasApplication::PerspectiveProjectionTestBoxProcess() {
  Matrix_4_4::ProjectionMatrix(world_to_projected_world_matrix_, kFov, kAspectRatio, z_near_, z_far_);
  for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
    Matrix_4_4::MultiplyByVector4D(
        world_to_projected_world_matrix_,
        // test_box_3d.translated_as_a_result_of_camera_translation_vertices_[i],
        test_box_3d.rotated_as_a_result_of_camera_rotation_vertices_[i],
                                   test_box_3d.resultVertices_[i]);

  }
}


void YasApplication::Set2dVerticesForTestBox() {
  for (int i = 0; i < test_box_3d.vertices_.size(); i++) {
    float xndc = test_box_3d.resultVertices_[i]->x_ / test_box_3d.resultVertices_[i]->w_;
    float yndc = test_box_3d.resultVertices_[i]->y_ / test_box_3d.resultVertices_[i]->w_;
    test_box_3d.vertices_in_2d_[i]->x_ = (kScreenWidth * 0.5f) * (xndc + 1);
    test_box_3d.vertices_in_2d_[i]->y_ = (kScreenHeight * 0.5f) * (1 - yndc);
  }
}

void YasApplication::DrawGraphicObjects() {
  for (int i = 0; i < test_box_3d.edges_in_2d.size(); i++) {
    DrawLineV1(*test_box_3d.edges_in_2d.at(i)._point0,
               *test_box_3d.edges_in_2d.at(i)._point1, *pixels_table_, kYellow);
  }
}

void YasApplication::DrawNumbersAsGroupOfLines(const std::vector<Vector2D<float>*> &vertices,
  const Vector4D<Uint8>& color, bool are_lines_continuous) {
  int step = 1;
  if (!are_lines_continuous) {
    step = 2;
  }
  if (vertices.size() > 1) {
    if (vertices.size() <= 3) {
      DrawLineV1(*vertices[0], *vertices[1], *pixels_table_, color);
    } else {
      for (int i = 0; i < vertices.size() - 1; i += step) {
        DrawLineV1(*vertices[i], *vertices[i + 1], *pixels_table_, color);
      }
    }
  }
}
