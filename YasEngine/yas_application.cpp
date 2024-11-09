//
// Created by lukesawicki on 9/21/24.
//

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

  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_AUDIO |
    SDL_INIT_VIDEO) != 0) {
    std::cout << "SDL_Init failed with error: " << SDL_GetError() << std::endl;
  }
  else {
    std::cout << "SDL_Init finished successfully" << std::endl;
  }

  SDL_DisplayID display_id = SDL_GetPrimaryDisplay();
  const SDL_DisplayMode* display_mode = SDL_GetCurrentDisplayMode(display_id);

  Uint32 windowFlags = SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_BORDERLESS | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_RESIZABLE |
    SDL_WINDOW_OPENGL; // SDL_WINDOW_ALWAYS_ON_TOP

  window_ =
      SDL_CreateWindow("YasEngine", kScreenWidth, kScreenHeight, windowFlags);

  SDL_SetWindowMinimumSize(window_, kScreenWidth, kScreenHeight);
  SDL_HideCursor();
}

void YasApplication::PrepareRenderingSettings() {
  pixels_table_ = new PixelsTable(kScreenWidth, kScreenHeight, kBlack);
  sdl_renderer_ = SDL_CreateRenderer(window_, NULL);
  SDL_SetRenderLogicalPresentation(sdl_renderer_, kScreenWidth, kScreenHeight,
                                   SDL_LOGICAL_PRESENTATION_DISABLED,
                                   SDL_SCALEMODE_NEAREST);

  screen_texture_ = SDL_CreateTexture(sdl_renderer_, SDL_PIXELFORMAT_RGBX32,
                                      SDL_TEXTUREACCESS_STREAMING, kScreenWidth,
                                      kScreenHeight);
}

void YasApplication::PrepareWorldSettings() {
  camera_position_ = new Vector4D<float>(0, 0, 0, 1);

  for (int i = 0; i < test_box_3d.vertices.size(); i++) {
    test_box_3d.rotatedvertices[i]->Set(test_box_3d.vertices[i]);
  }
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

  if (input_->up_) {
    test_box_3d.position.y_ += 0.05;
  }

  if (input_->down_) {
    test_box_3d.position.y_ -= 0.05;
  }

    if (input_->forward) {
    test_box_3d.position.z_ += 0.05;
  }

  if (input_->backward) {
    test_box_3d.position.z_ -= 0.05;
  }

  EulerRotationInLocalSpace();

  LocalToWorldTestBoxTransform();
  WorldToCameraTestBoxTransform();

  PerspectiveProjectionTestBoxProcess();
  Set2dVerticesForTestBox();
}

void YasApplication::HandleTestStuff() {
  if (mouse_position_change_information_->mouse_moved_) {
    rotateTestLineInToMouseDirection();
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
      quit_ = true;;
      break;
    case SDLK_SPACE:
      ;
      break;
    case SDLK_W:
      input_->up_ = true;
      break;
    case SDLK_S:
      input_->down_ = true;
      break;
    case SDLK_A:
      input_->left_ = true;
      break;
    case SDLK_D:
      input_->right_ = true;
      break;
    case SDLK_UP:
      input_->forward = true;
      break;
    case SDLK_DOWN:
      input_->backward = true;
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
      input_->up_ = false;
      break;
    case SDLK_S:
      input_->down_ = false;
      break;
    case SDLK_A:
      input_->left_ = false;
      break;
    case SDLK_D:
      input_->right_ = false;
      break;
    case SDLK_UP:
      input_->forward = false;
      break;
    case SDLK_DOWN:
      input_->backward = false;
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
  DrawBoxOnScreen();
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

void YasApplication::rotateTestLineInToMouseDirection() {
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

  if (input_->right_) {
    Matrix_4_4::RotationAroundX(rotation, 1.0f);
    for (int i = 0; i < test_box_3d.vertices.size(); i++) {
      Matrix_4_4::MultiplyByVector4D(rotation, test_box_3d.vertices[i],
                                     test_box_3d.rotatedvertices[i]);
    }
  }
  for (int i = 0; i < test_box_3d.vertices.size(); i++) {
    test_box_3d.vertices[i]->Set(test_box_3d.rotatedvertices[i]);
  }
}

void YasApplication::LocalToWorldTestBoxTransform() {
  Matrix_4_4::TranslationMatrix(local_to_world_matrix_, test_box_3d.position.x_, test_box_3d.position.y_,
                                test_box_3d.position.z_);

  // for (int i = 0; i < test_box_3d.vertices.size(); i++) {
  //   test_box_3d.rotatedvertices[i]->Set(test_box_3d.vertices[i]);
  // }

  // if (input_->right_) {
  //   Matrix_4_4::RotationAroundX(rotation, 1.0f);
  //   for (int i = 0; i < test_box_3d.vertices.size(); i++) {
  //     Matrix_4_4::MultiplyByVector4D(rotation, test_box_3d.rotatedvertices[i], test_box_3d.rotatedvertices[i]);
  //   }
  // }
  for (int i = 0; i < test_box_3d.vertices.size(); i++) {
    Matrix_4_4::MultiplyByVector4D(local_to_world_matrix_, test_box_3d.vertices[i],
                                   test_box_3d.worldVertices[i]);
  }
}

void YasApplication::WorldToCameraTestBoxTransform() {
  Matrix_4_4::TranslationMatrix(world_to_camera_matrix_, -camera_position_->x_, -camera_position_->y_,
                                -camera_position_->z_);
  // world_to_camera_matrix_.element_1_1_ = -1;
  // world_to_camera_matrix_.element_2_2_ = 1;
  for (int i = 0; i < test_box_3d.vertices.size(); i++) {
    Matrix_4_4::MultiplyByVector4D(world_to_camera_matrix_, test_box_3d.worldVertices[i],
                                   test_box_3d.cameraVertices[i]);
  }
}

void YasApplication::PerspectiveProjectionTestBoxProcess() {
  Matrix_4_4::ProjectionMatrix(world_to_projected_world_matrix_, kFov, kAspectRatio, z_near_, z_far_);
  for (int i = 0; i < test_box_3d.vertices.size(); i++) {
    // option with camera
    Matrix_4_4::MultiplyByVector4D(world_to_projected_world_matrix_, test_box_3d.cameraVertices[i],
                                   test_box_3d.resultVertices[i]);
    //Matrix_4_4::MultiplyByVector4D(world_to_projected_world_matrix_, test_box_3d.worldVertices[i], test_box_3d.resultVertices[i]);
  }
}


void YasApplication::Set2dVerticesForTestBox() {
  for (int i = 0; i < test_box_3d.vertices.size(); i++) {
    float xndc = test_box_3d.resultVertices[i]->x_ / test_box_3d.resultVertices[i]->w_;
    float yndc = test_box_3d.resultVertices[i]->y_ / test_box_3d.resultVertices[i]->w_;
    test_box_3d.vertices_in_2d_[i]->x_ = (kScreenWidth * 0.5f) * (xndc + 1);
    test_box_3d.vertices_in_2d_[i]->y_ = (kScreenHeight * 0.5f) * (1 - yndc);
  }
}

void YasApplication::DrawBoxOnScreen() {
  DrawLineV1(*test_box_3d.vertices_in_2d_[0], *test_box_3d.vertices_in_2d_[1], *pixels_table_, kYellow);
  DrawLineV1(*test_box_3d.vertices_in_2d_[1], *test_box_3d.vertices_in_2d_[2], *pixels_table_, kYellow);
  DrawLineV1(*test_box_3d.vertices_in_2d_[2], *test_box_3d.vertices_in_2d_[3], *pixels_table_, kYellow);
  DrawLineV1(*test_box_3d.vertices_in_2d_[3], *test_box_3d.vertices_in_2d_[0], *pixels_table_, kYellow);

  DrawLineV1(*test_box_3d.vertices_in_2d_[4], *test_box_3d.vertices_in_2d_[5], *pixels_table_, kYellow);
  DrawLineV1(*test_box_3d.vertices_in_2d_[5], *test_box_3d.vertices_in_2d_[6], *pixels_table_, kYellow);
  DrawLineV1(*test_box_3d.vertices_in_2d_[6], *test_box_3d.vertices_in_2d_[7], *pixels_table_, kYellow);
  DrawLineV1(*test_box_3d.vertices_in_2d_[7], *test_box_3d.vertices_in_2d_[4], *pixels_table_, kYellow);

  DrawLineV1(*test_box_3d.vertices_in_2d_[5], *test_box_3d.vertices_in_2d_[0], *pixels_table_, kYellow);
  DrawLineV1(*test_box_3d.vertices_in_2d_[6], *test_box_3d.vertices_in_2d_[1], *pixels_table_, kYellow);
  DrawLineV1(*test_box_3d.vertices_in_2d_[7], *test_box_3d.vertices_in_2d_[2], *pixels_table_, kYellow);
  DrawLineV1(*test_box_3d.vertices_in_2d_[4], *test_box_3d.vertices_in_2d_[3], *pixels_table_, kYellow);
}
