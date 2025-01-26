#include "yas_engine.hpp"

#include <SDL_mixer.h>

#include <bit>
#include <bitset>
#include <fstream>
#include <iostream>
#include <string>

#include "collider.hpp"
#include "cosine_points_generator.hpp"
#include "fibonacci_points_generator.hpp"
#include "prime_numbers_points_generator.hpp"
#include "randomizer.hpp"
#include "sine_points_generator.hpp"
#include "yas_graphics_library.hpp"

YasEngine* YasEngine::instance_ = nullptr;

void YasEngine::initialize() {
  engine_version_ = "YasEngine 1 | Beauty of Math 1 version: " +
    std::to_string(MAJOR_REVISION) + "." +
    std::to_string(MINOR_REVISION) + "." +
    std::to_string(BUG_FIX_RELEASE) + "." +
    std::to_string(BUILD_NUMBER);
  std::cout << engine_version_ << "\n";
  ReadSettingsFromFile();

  srand(clock());

  PrepareBasicSettings();
  PrepareRendering();
  PreparePlayer();
  PrepareGameWorld();
  PrepareSoundAndMusic();
  PrepareInterface();
  writer_ = new ScreenWriter(window_dimensions_);
  writer_->Initialize();

}

void YasEngine::clean() {
  for (auto drawable_object : objects_to_draw_) {
    delete drawable_object;
  }

  delete pixels_table_;
  delete window_dimensions_;

  delete sine_picture_;
  delete cosine_picture_;
  delete fibonaccie_picture_;
  delete prime_numbers_picture_;

  delete mouse_position_change_information_;

  // delete player_; // will be deleted in the loop at the beginning of that
  // method

  delete input_;

  Mix_FreeChunk(shoot_sound_);
  Mix_FreeChunk(hit_sound_);
  Mix_FreeMusic(music_);
  Mix_CloseAudio();
  Mix_Quit();
  SDL_DestroyTexture(screen_texture_);
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

void YasEngine::YasEngineStart() {
  TimePicker timePicker = TimePicker();
  time_ = timePicker.GetSeconds();
  fps_time_ = 0.0F;
  frames_ = 0;

  while (!quit_) {
    while (SDL_PollEvent(&event_)) {
      HandleInput(event_);
    }

    new_time_ = timePicker.GetSeconds();
    delta_time_ = new_time_ - time_;
    time_ = new_time_;

    ++frames_;
    fps_time_ = fps_time_ + delta_time_;
    if (fps_time_ >= 1.0F) {
      fps_ = frames_ / fps_time_;
      frames_ = 0;
      fps_time_ = 0.0F;
    }

    Update(delta_time_);
    Render(delta_time_);
    if (level_changed_) {
      level_changed_ = false;
    }
  }

  clean();
}

void YasEngine::ReadSettingsFromFile() {
  std::ifstream settings_file("settings.json");
  if (!settings_file.is_open()) {
    std::cerr << "Error opening JSON file" << std::endl;
    exit(1);
  }

  std::string settings_string((std::istreambuf_iterator<char>(settings_file)),
                              std::istreambuf_iterator<char>());
  settings_file.close();

  settings.Parse(settings_string.c_str());

  if (settings.HasParseError()) {
    std::cerr << "Error parsing JSON" << std::endl;
    exit(1);
  }

  const rapidjson::Value& sound_settings = settings["SOUND_MUSIC"];

  music_volume_ = sound_settings["MUSIC_VOLUME"].GetInt();
  shoot_volume_ = sound_settings["SHOOT_VOLUME"].GetInt();
  hit_volume_ = sound_settings["HIT_VOLUME"].GetInt();
  other_volume_ = sound_settings["OTHER_VOLUME"].GetInt();
}

void YasEngine::PrepareRendering() {
  pixels_table_ = new PixelsTable(window_width_, window_height_, kBlack);
  renderer_ = SDL_CreateRenderer(window_, NULL);
  SDL_SetRenderLogicalPresentation(renderer_, window_width_, window_height_,
                                   SDL_LOGICAL_PRESENTATION_DISABLED);
  screen_texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBX32,
                                      SDL_TEXTUREACCESS_STREAMING,
                                      window_width_, window_height_);
}

void YasEngine::PrepareBasicSettings() {
  CheckEndianness();

  SDL_Init(SDL_INIT_EVENTS | SDL_INIT_AUDIO | SDL_INIT_VIDEO);

      // Get the number of displays available
  SDL_DisplayID display_id = SDL_GetPrimaryDisplay();
  const SDL_DisplayMode* display_mode = SDL_GetCurrentDisplayMode(display_id);

  // window_width_ = display_mode->w;
  // window_height_ = display_mode->w / 2;

  window_dimensions_ = new Vector2D<int>(window_width_, window_height_);
  // Uint32 windowFlags = SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_BORDERLESS |
  //   SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_RESIZABLE |
  //   SDL_WINDOW_OPENGL; // SDL_WINDOW_ALWAYS_ON_TOP

    Uint32 windowFlags = SDL_WINDOW_OPENGL;  // SDL_WINDOW_ALWAYS_ON_TOP
  window_ =
    SDL_CreateWindow("YasEngine", window_width_, window_height_, windowFlags);

  SDL_SetWindowMinimumSize(window_, window_width_, window_height_);
  SDL_HideCursor();
}

void YasEngine::CheckEndianness() {
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

void YasEngine::DrawHudElements(double& delta_time) {
  DrawCrossHair(mouse_x_, mouse_y_, *pixels_table_, false);
}

void YasEngine::HandleInput(SDL_Event& event) {
  if (event.type == SDL_EVENT_QUIT) {
    quit_ = true;
  }
  else {
    HandleKeyboardInput(event);
    HandleMouseInput(event);
  }
}

void YasEngine::HandleKeyboardInput(SDL_Event& event) {
  if (event.type == SDL_EVENT_KEY_DOWN) {
    switch (event.key.key) {
    case SDLK_ESCAPE:
      HandleGameStateWhenESCbuttonPushed();
      break;
    case SDLK_SPACE:
      HandleGameStateWhenSPACEbuttonPushed();
      break;
    case SDLK_W:
      input_->up = true;
      break;
    case SDLK_S:
      input_->down = true;
      break;
    case SDLK_A:
      input_->left = true;
      break;
    case SDLK_D:
      input_->right = true;
      break;
    case SDLK_O:
      input_->test_o_button = true;
      break;
    case SDLK_RETURN:
      if (game_state_ == kLevelChangeScreen) {
        game_state_ = GameState::kGameplay;
      }
      if (game_state_ == GameState::kYouWon) {
        game_state_ = GameState::kMainMenuRestart;
        player_won_and_exited_ = true;
      }
      break;
    case SDLK_TAB:
      if (game_state_ == kLevelChangeScreen) {
        game_state_ = GameState::kGameplay;
      }
      if (game_state_ == GameState::kYouWon) {
        game_state_ = GameState::kMainMenuRestart;
        player_won_and_exited_ = true;
      }
      break;
    default: ;
    }
  }

  if (event.type == SDL_EVENT_KEY_UP) {
    switch (event.key.key) {
    case SDLK_W:
      input_->up = false;
      break;
    case SDLK_S:
      input_->down = false;
      break;
    case SDLK_A:
      input_->left = false;
      break;
    case SDLK_D:
      input_->right = false;
      break;
    case SDLK_O:
      if (input_->test_o_button == true) {
        Mix_PlayChannel(-1, other_sound_, 0);
        input_->test_o_button = false;
      }
      break;
    default: ;
    }
  }
}

void YasEngine::HandleMouseInput(SDL_Event& event) {
  if (event.type == SDL_EVENT_MOUSE_MOTION) {
    HandleMouseMovement();
  }
  if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
    event.button.button == SDL_BUTTON_LEFT) {
    switch (game_state_) {
    case kGameplay:
      player_->is_shooting_ = true;

      break;
    case kMainMenuRestart:
      HandleClickedButtons();
      break;
    case kIntro:
      game_state_ = GameState::kMainMenuRestart;
      break;
    case kOutro:
      quit_ = true;
      break;
    default: ;
    }
  }

  if (event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
    event.button.button == SDL_BUTTON_LEFT) {
    switch (game_state_) {
    case kGameplay:
      player_->is_shooting_ = false;
      break;
    }
  }
}

void YasEngine::HandleMouseMovement() {
  float x;
  float y;
  SDL_GetMouseState(&x, &y);
  mouse_position_change_information_->mouseMoved = true;
  mouse_position_change_information_->x = x;
  mouse_position_change_information_->y = y;
  mouse_x_ = static_cast<float>(mouse_position_change_information_->x);
  mouse_y_ = static_cast<float>(mouse_position_change_information_->y);

  WindowPositionToCartesianPosition(mouse_x_, mouse_y_, window_dimensions_);
}

void YasEngine::DeleteNotAliveObjects() {
  for (int i = 0; i < objects_to_draw_.size(); i++) {
    if (objects_to_draw_[i]->is_alive_ == false) {
      objects_to_draw_.erase(objects_to_draw_.begin() + i);
    }
  }
}

void YasEngine::HandleSpawningCollectibles() {
 
}

bool YasEngine::IsObjectInSameQuarterAsProtagonist(int random_spawner) {
  return false;
}

void YasEngine::HandleProjectiles() {
  Projectile* projectile = player_->shoot();

  if (projectile != nullptr) {
    Mix_PlayChannel(-1, shoot_sound_, 0);
    objects_to_draw_.push_back(projectile);
  }

  if (projectile != nullptr) {
    projectile = nullptr;
  }
}

void YasEngine::HandlePlayer() {
  if (mouse_position_change_information_->mouseMoved || input_->up ||
    input_->down || input_->left || input_->right) {
    player_->RotateToMousePositionInLocalCoordinateSystem(
      static_cast<float>(mouse_position_change_information_->x),
      static_cast<float>(mouse_position_change_information_->y),
      window_dimensions_);
  }
}

void YasEngine::PreparePlayer() {
  srand(clock());
  int size_Of_gameplay_space = static_cast<int>(window_dimensions_->x_ * 0.25F);
  int x = Randomizer::DrawNumberClosedInterval(0, size_Of_gameplay_space) - 64;
  int y = Randomizer::DrawNumberClosedInterval(0, size_Of_gameplay_space) - 64;

  player_ = new Player(0.0F, 0.0F);
  player_->set_color(kYellow);
  player_->set_input(input_);
  player_->set_input(mouse_position_change_information_);
  objects_to_draw_.push_back(player_);
}

void YasEngine::Update(double& delta_time) {
  if (level_changed_) {
    for (int i = 0; i < objects_to_draw_.size(); i++) {
      if (objects_to_draw_[i]->i_am_ != GameObject::kProtagonist) {
        objects_to_draw_[i]->is_alive_ = false;
      }
    }
    Spawner::number_of_spawned_objects_ = 0;
  }

  DeleteNotAliveObjects();

  switch (game_state_) {
  case kGameplay:
    HandleSpawningCollectibles();
    HandlePhysics();
    if (level_changed_) {
      game_state_ = kLevelChangeScreen;
    }
    if (level_changed_ && level_ == -1) {
      game_state_ = GameState::kYouWon;
    }
    MoveObjects();
    HandleProjectiles();
    HandlePlayer();
    break;
  }

  if (player_won_and_exited_) {
    ResetAll();
  }
}

void YasEngine::ResetAll() {
  DeleteNotAliveObjects();
  level_ = 1;
  previous_level_ = 0;
  level_changed_ = false;
  player_won_and_exited_ = false;
}

void YasEngine::Render(double& delta_time) {
  pixels_table_->ClearColor(kBlack);

switch (game_state_) {
    case kIntro:
      writer_->Write(
          -1 * ((static_cast<int>(line_22.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[1], line_22, kLightBlue,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_23.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[0], line_23, kRed, *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_24.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_bottom_positions_[0], line_24, kYellow,
          *pixels_table_);
      break;
    case kMainMenuRestart:
      DrawButtons();
      break;
    case kGameplay:
      RenderGameObjects();
      break;
    case kOutro:
      writer_->Write(
          -1 * ((static_cast<int>(line_25.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[7], line_25, kLightBlue,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_26.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[6], line_26, kPurple,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_27.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[5], line_27, kLightBlue,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_28.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[4], line_28, kPurple,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_29.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[3], line_29, kLightBlue,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_30.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[2], line_30, kPurple,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_31.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[1], line_31, kGreen, *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_32.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[0], line_32, kYellow,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_33.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_bottom_positions_[0], line_33, kYellow,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_34.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_bottom_positions_[1], line_34, kBlue,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_35.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_bottom_positions_[2], line_35, kYellow,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_36.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_bottom_positions_[3], line_36, kBlue,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_37.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_bottom_positions_[4], line_37, kYellow,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_38.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_bottom_positions_[5], line_38, kPolygon,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_39.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_bottom_positions_[6], line_39, kXportal,
          *pixels_table_);

      break;
    case kLevelChangeScreen:
      RenderLevelChange();
      break;
    case kYouWon:
      RenderWonScreen();
      break;
    default:;
  }

  

  DrawHudElements(delta_time);

  DrawTestStuff();

  SDL_UpdateTexture(screen_texture_, NULL, pixels_table_->pixels_,
                    window_width_ * 4);
  SDL_RenderTexture(renderer_, screen_texture_, NULL, NULL);
  SDL_RenderPresent(renderer_);
}

void YasEngine::DrawTestStuff() {
  DrawLine(test_line.point_0_, test_line.point_1_, *pixels_table_, test_color_);

  // int numberOfPixels = window_dimensions_->x_ * window_dimensions_->y_;
  // int index = numberOfPixels-1;
  // for (int i = 0; i < numberOfPixels;i++) {
  //   pixels_table_->pixels_[i * 4 + 0]/*R*/ = static_cast <Uint8>(tga.image_data_[index * 4 + 2]); //BGRA 
  //   pixels_table_->pixels_[i * 4 + 1]/*G*/ = static_cast <Uint8>(tga.image_data_[index * 4 + 1]); //BGRA 
  //   pixels_table_->pixels_[i * 4 + 2]/*B*/ = static_cast <Uint8>(tga.image_data_[index * 4 + 0]); //BGRA 
  //   pixels_table_->pixels_[i * 4 + 3]/*A*/ = static_cast <Uint8>(tga.image_data_[index * 4 + 3]); //BGRA
  //   index--;
  // }
  for (int i = 0; i < pixels_table_->window_dimensions_.x_*pixels_table_->window_dimensions_.y_*4; i++) {
      pixels_table_->pixels_[i] = tga.pixels_[i];
  }

}

void YasEngine::RenderGameObjects() {
  for (auto object : objects_to_draw_) {
    if (object->is_alive_) {
      // TODO if gamestate == kGameplay

      DrawPolygon(object, *pixels_table_);
    }
  }
}

void YasEngine::RenderLevelChange() {
  switch (previous_level_) {
    case 1:

      writer_->Write(
          -1 * ((static_cast<int>(line_1.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[1], line_1, kLightBlue,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_2.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[0], line_2, kBlue, *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_3.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_bottom_positions_[0], line_3, kGreen,
          *pixels_table_);
      break;
    case 2:

      writer_->Write(
          -1 * ((static_cast<int>(line_4.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[1], line_4, kLightBlue,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_5.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[0], line_5, kBlue, *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_6.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_bottom_positions_[0], line_6, kGreen,
          *pixels_table_);
      break;
    case 3:

      writer_->Write(
          -1 * ((static_cast<int>(line_7.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[1], line_7, kLightBlue,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_8.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[0], line_8, kBlue, *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_9.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_bottom_positions_[0], line_9, kGreen,
          *pixels_table_);
      break;
    case 4:

      writer_->Write(
          -1 * ((static_cast<int>(line_10.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[1], line_10, kLightBlue,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_11.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_top_positions_[0], line_11, kBlue, *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_12.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_bottom_positions_[0], line_12, kGreen,
          *pixels_table_);
      writer_->Write(
          -1 * ((static_cast<int>(line_13.size()) * writer_->kfont_width_) / 2),
          writer_->vertical_bottom_positions_[1], line_13, kYellow,
          *pixels_table_);
      break;
    default:;
  }
}

void YasEngine::RenderWonScreen() {
  //  8 wierszy

  writer_->Write(
      -1 * ((static_cast<int>(line_14.size()) * writer_->kfont_width_) / 2),
      writer_->vertical_top_positions_[3], line_14, kLightBlue, *pixels_table_);
  writer_->Write(
      -1 * ((static_cast<int>(line_15.size()) * writer_->kfont_width_) / 2),
      writer_->vertical_top_positions_[2], line_15, kYellow, *pixels_table_);
  writer_->Write(
      -1 * ((static_cast<int>(line_16.size()) * writer_->kfont_width_) / 2),
      writer_->vertical_top_positions_[1], line_16, kPurple, *pixels_table_);
  writer_->Write(
      -1 * ((static_cast<int>(line_17.size()) * writer_->kfont_width_) / 2),
      writer_->vertical_top_positions_[0], line_17, kPurple, *pixels_table_);
  writer_->Write(
      -1 * ((static_cast<int>(line_18.size()) * writer_->kfont_width_) / 2),
      writer_->vertical_bottom_positions_[0], line_18, kPurple, *pixels_table_);
  writer_->Write(
      -1 * ((static_cast<int>(line_19.size()) * writer_->kfont_width_) / 2),
      writer_->vertical_bottom_positions_[1], line_19, kGreen, *pixels_table_);
  writer_->Write(
      -1 * ((static_cast<int>(line_20.size()) * writer_->kfont_width_) / 2),
      writer_->vertical_bottom_positions_[2], line_20, kYellow, *pixels_table_);
  writer_->Write(
      -1 * ((static_cast<int>(line_21.size()) * writer_->kfont_width_) / 2),
      writer_->vertical_bottom_positions_[3], line_21, kLightBlue,
      *pixels_table_);
}

void YasEngine::HandlePhysics() {

} // END OF HandlePhysics()


bool YasEngine::IsObjectProtagonist(GameObject* game_object) {
  return game_object->i_am_ == GameObject::kProtagonist;
}

GameObject* YasEngine::GetProtagonist(GameObject* game_object_0,
                                      GameObject* game_object_1) {
  if (IsObjectProtagonist(game_object_0)) {
    return game_object_0;
  }
  if (IsObjectProtagonist(game_object_1)) {
    return game_object_1;
  }
  return nullptr;
}

GameObject* YasEngine::GetNotProtagonist(GameObject* game_object_0,
                                         GameObject* game_object_1) {
  if (!IsObjectProtagonist(game_object_0)) {
    return game_object_0;
  }
  if (!IsObjectProtagonist(game_object_1)) {
    return game_object_1;
  }
  return nullptr;
}

void YasEngine::MoveObjects() {
  for (auto object : objects_to_draw_) {
    if (object->is_alive_) {
      object->Move(static_cast<float>(delta_time_));
      object->RegeneratePolygon();
    }
  }
}

void YasEngine::PrepareSoundAndMusic() {
  audio_specs_.freq = 44100;
  audio_specs_.format = MIX_DEFAULT_FORMAT;
  audio_specs_.channels = 2;

  if (Mix_OpenAudio(0, &audio_specs_)) {
    std::cout << "Error cannot open audio device" << std::endl;
  }

  Mix_Init(MIX_DEFAULT_FORMAT);

  music_ = Mix_LoadMUS("music.wav");
  if (music_ == NULL) {
    std::cout << "Error while loading music_. Cannot load music." << std::endl;
    std::cout << "ERROR messages: " << SDL_GetError() << std::endl;
    quit_ = true;
  }

  Mix_VolumeMusic(music_volume_);

  shoot_sound_ = Mix_LoadWAV("shoot.wav");
  Mix_VolumeChunk(shoot_sound_, shoot_volume_);

  hit_sound_ = Mix_LoadWAV("hit.wav");
  Mix_VolumeChunk(hit_sound_, hit_volume_);

  other_sound_ = Mix_LoadWAV("other.wav");
  Mix_VolumeChunk(other_sound_, other_volume_);

  if (shoot_sound_ == NULL || hit_sound_ == NULL || other_sound_ == NULL) {
    std::cout << "Error while loading sounds. Cannot load sounds." << std::endl;
    std::cout << "Error messages: " << SDL_GetError() << std::endl;
    quit_ = true;
  }
  Mix_PlayMusic(music_, 999);
}

void YasEngine::PrepareGameWorld() {
  srand(clock());

  int main_node_x = -(window_dimensions_->x_ / 4);
  int main_node_y = 0;

  std::cout << "Main node X: " << main_node_x << "\n";
  std::cout << "Main node Y: " << main_node_y << "\n";

  number_of_given_colors_.insert({"kRed", 0});
  number_of_given_colors_.insert({"GREEN", 0});
  number_of_given_colors_.insert({"kBlue", 0});
  number_of_given_colors_.insert({"YELLOW", 0});
  PrepareDataForDrawingGraphs();

  LoadGraphicsFile("example.tga");

  test_line.point_0_.x_ = -128;
  test_line.point_0_.y_ = 128;

  test_line.point_1_.y_ = 255;
  test_line.point_1_.x_ = 400;

  const std::byte bit5Mask = std::byte{0x20};
  bool isTopToBottm = ((tga.image_descriptor_ & bit5Mask) != std::byte{0});

  if (isTopToBottm) {
    std::cout << "Top to bottom" << "\n";
  } else {
    std::cout << "Bottom to row" << "\n";
  }

  int pixelPosition = 16;
  test_color_.x_ = static_cast<Uint8>(tga.image_data_[pixelPosition * 4 + 2]);  // R
  test_color_.y_ = static_cast<Uint8>(tga.image_data_[pixelPosition * 4 + 1]);  // G
  test_color_.z_ = static_cast<Uint8>(tga.image_data_[pixelPosition * 4 + 0]);  // B
  test_color_.w_ = static_cast<Uint8>(tga.image_data_[pixelPosition * 4 + 3]);  // A

}

void YasEngine::LoadGraphicsFile(std::string fileName) { //"example.tga"

  std::cout << "\n\n\n\nReading *.tga file: " << fileName.c_str() << "\n";

  std::ifstream tgaFile(fileName.c_str(), std::ifstream::binary);
 
  std::streampos position = -1;

  if (tgaFile) {

    std::cout << "Reading TGA 32 bits BGRA without RLE" << "\n";
    std::cout << "Reading Header: " << "\n";

    position = tga.dataPositions[0];
    tgaFile.seekg(position);
    tgaFile.read(reinterpret_cast<char*>(&tga.id_length_), sizeof(std::byte));

    std::cout << "Field id_length_ \"ID length\" with value: " << to_integer<int>(tga.id_length_) << "\n";

    position = tga.dataPositions[1];
    tgaFile.seekg(position);
    tgaFile.read(reinterpret_cast<char*>(&tga.color_map_type_), sizeof(std::byte));

    std::cout << "Field color_map_type_ \"Color map type\" with value: " << to_integer<int>(tga.color_map_type_) << "\n";

    position = tga.dataPositions[2];
    tgaFile.seekg(position);
    tgaFile.read(reinterpret_cast<char*>(&tga.image_type_), sizeof(std::byte));

    std::cout << "Field image_type_ \"Image type\" with value: "  << to_integer<int>(tga.image_type_) << "\n";

    std::cout << "Reading Color map specification" << "\n";

    position = tga.dataPositions[3];
    tgaFile.seekg(position);
    tgaFile.read(reinterpret_cast<char*>(&tga.first_entry_index_),sizeof(unsigned short));

    std::cout << "Field first_entry_index_ \"First entry index\" with value: " << tga.first_entry_index_ << "\n";

    position = tga.dataPositions[4];
    tgaFile.seekg(position);
    tgaFile.read(reinterpret_cast<char*>(&tga.color_map_length_),sizeof(unsigned short));

    std::cout <<"Field color_map_length_ \"Color map length\" with value: " << tga.color_map_length_ << "\n";

    position = tga.dataPositions[5];
    tgaFile.seekg(position);
    tgaFile.read(reinterpret_cast<char*>(&tga.color_map_entry_size_), sizeof(std::byte));

    std::cout << "Field color_map_entry_size_ \"Color map entry size\" with value: " << to_integer<int>(tga.color_map_entry_size_) << "\n";

    std::cout << "Reading Image specification" << "\n";

    position = tga.dataPositions[6];
    tgaFile.seekg(position);
    tgaFile.read(reinterpret_cast<char*>(&tga.x_origin_),
                 sizeof(unsigned short));

    std::cout << "Field x_origin_ \"X-origin\" with value: " << tga.x_origin_ << "\n";

    position = tga.dataPositions[7];
    tgaFile.seekg(position);
    tgaFile.read(reinterpret_cast<char*>(&tga.y_origin_),
                 sizeof(unsigned short));

    std::cout << "Field y_origin_ \"Y-origin\" with value: " << tga.y_origin_ << "\n";

    position = tga.dataPositions[8];
    tgaFile.seekg(position);
    tgaFile.read(reinterpret_cast<char*>(&tga.image_width_),
                 sizeof(unsigned short));

    std::cout << "Field image_width_ \"Image width\" with value: " << tga.image_width_ << "\n";

    position = tga.dataPositions[9];
    tgaFile.seekg(position);
    tgaFile.read(reinterpret_cast<char*>(&tga.image_height_),sizeof(unsigned short));

    std::cout << "Field image_height_ \"Image height\" with value: " << tga.image_height_ << "\n";

    position = tga.dataPositions[10];
    tgaFile.seekg(position);
    tgaFile.read(reinterpret_cast<char*>(&tga.pixel_depth_), sizeof(std::byte));

    std::cout << "Field pixel_depth_ \"Pixel depth\" with value: " << to_integer<int>(tga.pixel_depth_) << "\n";

    position = tga.dataPositions[11];
    tgaFile.seekg(position);
    tgaFile.read(reinterpret_cast<char*>(&tga.image_descriptor_),
                 sizeof(std::byte));
    std::cout << "Field image_descriptor_ \"Image descriptor\"\n"
              << "Image descriptor has 1 byte of size which include in it:\n"
              << "bits 3–0 give the alpha channel depth, bits 5–4 give pixel ordering\n"
              << "Bit 4 of the image descriptor byte indicates right-to-left pixel ordering if set.\n"
              << "Bit 5 indicates an ordering of top-to-bottom. Otherwise, pixels are stored in bottom-to-top, left-to-right order."
              << "Field image descriptor whole value is: " << to_integer<int>(tga.image_descriptor_)
              << "\n";

    std::cout << "Reading Image and color map data:\n";

    if (to_integer<int>(tga.id_length_) != 0) {
      position = tga.dataPositions[12];
      tgaFile.seekg(position);
      tga.image_id_ = new std::byte[to_integer<int>(tga.id_length_)];
      tgaFile.read(reinterpret_cast<char*>(tga.image_id_),to_integer<int>(tga.id_length_));
      std::cout << "Field image_id_ \"Image ID\" with value: " << to_integer<int>(tga.id_length_);
    }

    std::cout << "Calculating Color map data position in file:\n";

    tga.dataPositions[13] = tga.dataPositions[12] + to_integer<int>(tga.id_length_);

    std::cout << "Calculated position: " << tga.dataPositions[13] << "\n";

    int sizeOfColorMapInBytes = 0;

    if (tga.first_entry_index_ != 0) {
      position = tga.dataPositions[13];
      tgaFile.seekg(position);
      std::cout << "Calculating size of color map in bytes" << "\n";

      sizeOfColorMapInBytes = (tga.color_map_length_ * to_integer<int>(tga.color_map_entry_size_)) / 8;

      std::cout << "Calculated value: " << sizeOfColorMapInBytes << "\n";
      tga.color_map_ = new std::byte[sizeOfColorMapInBytes];
      tgaFile.read(reinterpret_cast<char*>(tga.color_map_), sizeOfColorMapInBytes);
      std::cout << "Field color_map_ \"Color map data\" with value: " << tga.color_map_ << "\n";
    }
    std::cout << "Calculating Image data position in file\n";

    tga.dataPositions[14] = tga.dataPositions[13] + sizeOfColorMapInBytes;

    std::cout << "Calculated position: " << tga.dataPositions[14] << "\n";

    position = tga.dataPositions[14];
    tgaFile.seekg(position);

    std::cout << "Calculating Image data size in bytes:\n";

    int imageDataSize = ( (tga.image_width_ * tga.image_height_ * (to_integer<int>(tga.pixel_depth_)/8) ) );

    std::cout << "Calculated size: " << imageDataSize << "\n";

    tga.image_data_ = new std::byte[imageDataSize];

    std::cout << "Reading Image data:\n";

    tgaFile.read(reinterpret_cast<char*>(tga.image_data_), imageDataSize);

    tga.pixels_ = new Uint8[imageDataSize];

    int numberOfPixels = tga.image_width_ * tga.image_height_;
    int wiersz_tga=tga.image_height_;
    int pixel_tga=0;
    int index_tga = imageDataSize - tga.image_width_*4;

    int wiesz_pixels = 0;
    int pixel_pixels = 0;
    int index_pixels = wiesz_pixels + pixel_pixels;

    int k;

    std::cout << "Rewriting Image data to internal data format";

    for (int i = 0; i < tga.image_height_; i++) {
      for (int j = 0; j < tga.image_width_; j++) {
        k = numberOfPixels - tga.image_width_*(i + 1) + j;
        tga.pixels_[4*(i*tga.image_width_+j)+0] /*R*/ = static_cast<Uint8>(tga.image_data_[4*k + 2]);  // BGRA
        tga.pixels_[4*(i*tga.image_width_+j)+1] /*G*/ = static_cast<Uint8>(tga.image_data_[4*k + 1]);  // BGRA
        tga.pixels_[4*(i*tga.image_width_+j)+2] /*B*/ = static_cast<Uint8>(tga.image_data_[4*k + 0]);  // BGRA
        tga.pixels_[4*(i*tga.image_width_+j)+3] /*A*/ = static_cast<Uint8>(tga.image_data_[4*k + 3]);  // BGRA index--;
      }
    }
  } else {
    std::cout << "Error while reading tga file!" << "\n";
  }

  std::cout << "Image file: " << fileName.c_str() << " loaded successfully.\n\n\n\n";

  std::cout << "Creating and saving default value for output tga files\n";

  tgaFileWithDefaultValues.id_length_ = tga.id_length_;
  tgaFileWithDefaultValues.color_map_type_ = tga.color_map_type_;
  tgaFileWithDefaultValues.image_type_ = tga.image_type_;
  tgaFileWithDefaultValues.first_entry_index_ = tga.first_entry_index_;
  tgaFileWithDefaultValues.color_map_length_ = tga.color_map_length_;
  tgaFileWithDefaultValues.color_map_entry_size_ = tga.color_map_entry_size_;
  tgaFileWithDefaultValues.x_origin_ = tga.x_origin_;
  tgaFileWithDefaultValues.y_origin_ = tga.y_origin_;
  tgaFileWithDefaultValues.image_width_ = tga.image_width_;
  tgaFileWithDefaultValues.image_height_ = tga.image_height_;
  tgaFileWithDefaultValues.pixel_depth_ = tga.pixel_depth_;
  tgaFileWithDefaultValues.image_descriptor_ = tga.image_descriptor_;
  //std::byte* image_id_;
  //std::byte* color_map_;
  //std::byte* image_data_;

  tgaFile.close();

  SaveGraphicsFile("____tga_test.tga", tga.pixels_, 512, 512);
}

void YasEngine::SaveGraphicsFile(std::string fileName, Uint8* pixelsTable, short imageWidth, short imageHeight) {
  std::cout << "\n\n\n\Writing *.tga file: " << fileName.c_str() << "\n";

  Tga tgaToWrite;
  tgaToWrite.image_width_ = imageWidth;
  tgaToWrite.image_height_ = imageWidth;

  tgaToWrite.id_length_ = tgaFileWithDefaultValues.id_length_;
  tgaToWrite.color_map_type_ = tgaFileWithDefaultValues.color_map_type_;
  tgaToWrite.image_type_ = tgaFileWithDefaultValues.image_type_;
  tgaToWrite.first_entry_index_ = tgaFileWithDefaultValues.first_entry_index_;
  tgaToWrite.color_map_length_ = tgaFileWithDefaultValues.color_map_length_;
  tgaToWrite.color_map_entry_size_ = tgaFileWithDefaultValues.color_map_entry_size_;
  tgaToWrite.x_origin_ = tgaFileWithDefaultValues.x_origin_;
  tgaToWrite.y_origin_ = tgaFileWithDefaultValues.y_origin_;
  tgaToWrite.pixel_depth_ = tgaFileWithDefaultValues.pixel_depth_;
  tgaToWrite.image_descriptor_ = tgaFileWithDefaultValues.image_descriptor_;

  int imageDataSize = ( (tgaToWrite.image_width_ * tgaToWrite.image_height_ * (to_integer<int>(tgaToWrite.pixel_depth_)/8) ) );

  tgaToWrite.image_data_ = new std::byte[imageDataSize];

  int k;
  int numberOfPixels = tgaToWrite.image_width_ * tgaToWrite.image_height_;
  for (int i = 0; i < tgaToWrite.image_height_; i++) {
    for (int j = 0; j < tgaToWrite.image_width_; j++) {
      k = numberOfPixels - tgaToWrite.image_width_ * (i + 1) + j;
       tgaToWrite.image_data_[4 * k + 2] /* BGRA */ = static_cast<std::byte>(pixelsTable[4 * (i * tgaToWrite.image_width_ + j) + 0]);  /*R*/
       tgaToWrite.image_data_[4 * k + 1] /* BGRA */ = static_cast<std::byte>(pixelsTable[4 * (i * tgaToWrite.image_width_ + j) + 1]);  /*G*/
       tgaToWrite.image_data_[4 * k + 0] /* BGRA */ = static_cast<std::byte>(pixelsTable[4 * (i * tgaToWrite.image_width_ + j) + 2]);  /*B*/
       tgaToWrite.image_data_[4 * k + 3] /* BGRA */ = static_cast<std::byte>(pixelsTable[4 * (i * tgaToWrite.image_width_ + j) + 3]);  /*A*/
    }
  }

  std::ofstream tgaFile(fileName.c_str(), std::ofstream::binary);

  std::streampos position = -1;

  if (tgaFile) {
    std::cout << "Reading TGA 32 bits BGRA without RLE" << "\n";
    std::cout << "Reading Header: " << "\n";

    position = tgaToWrite.dataPositions[0];
    // tgaFile.seekg(position);
    tgaFile.write(reinterpret_cast<char*>(&tgaToWrite.id_length_),
                  sizeof(std::byte));

    std::cout << "Field id_length_ \"ID length\" with value: "
              << to_integer<int>(tgaToWrite.id_length_) << "\n";

    position = tgaToWrite.dataPositions[1];
    // tgaFile.seekg(position);
    tgaFile.write(reinterpret_cast<char*>(&tgaToWrite.color_map_type_),
                  sizeof(std::byte));

    std::cout << "Field color_map_type_ \"Color map type\" with value: "
              << to_integer<int>(tgaToWrite.color_map_type_) << "\n";

    position = tgaToWrite.dataPositions[2];
    // tgaFile.seekg(position);
    tgaFile.write(reinterpret_cast<char*>(&tgaToWrite.image_type_),
                  sizeof(std::byte));

    std::cout << "Field image_type_ \"Image type\" with value: "
              << to_integer<int>(tgaToWrite.image_type_) << "\n";

    std::cout << "Reading Color map specification" << "\n";

    position = tgaToWrite.dataPositions[3];
    // tgaFile.seekg(position);
    tgaFile.write(reinterpret_cast<char*>(&tgaToWrite.first_entry_index_),
                  sizeof(unsigned short));

    std::cout << "Field first_entry_index_ \"First entry index\" with value: "
              << tgaToWrite.first_entry_index_ << "\n";

    position = tgaToWrite.dataPositions[4];
    // tgaFile.seekg(position);
    tgaFile.write(reinterpret_cast<char*>(&tgaToWrite.color_map_length_),
                  sizeof(unsigned short));

    std::cout << "Field color_map_length_ \"Color map length\" with value: "
              << tgaToWrite.color_map_length_ << "\n";

    position = tgaToWrite.dataPositions[5];
    // tgaFile.seekg(position);
    tgaFile.write(reinterpret_cast<char*>(&tgaToWrite.color_map_entry_size_),
                  sizeof(std::byte));

    std::cout
        << "Field color_map_entry_size_ \"Color map entry size\" with value: "
        << to_integer<int>(tgaToWrite.color_map_entry_size_) << "\n";

    std::cout << "Reading Image specification" << "\n";

    position = tgaToWrite.dataPositions[6];
    // tgaFile.seekg(position);
    tgaFile.write(reinterpret_cast<char*>(&tgaToWrite.x_origin_),
                  sizeof(unsigned short));

    std::cout << "Field x_origin_ \"X-origin\" with value: "
              << tgaToWrite.x_origin_ << "\n";

    position = tgaToWrite.dataPositions[7];
    // tgaFile.seekg(position);
    tgaFile.write(reinterpret_cast<char*>(&tgaToWrite.y_origin_),
                  sizeof(unsigned short));

    std::cout << "Field y_origin_ \"Y-origin\" with value: "
              << tgaToWrite.y_origin_ << "\n";

    position = tgaToWrite.dataPositions[8];
    // tgaFile.seekg(position);
    tgaFile.write(reinterpret_cast<char*>(&tgaToWrite.image_width_),
                  sizeof(unsigned short));

    std::cout << "Field image_width_ \"Image width\" with value: "
              << tgaToWrite.image_width_ << "\n";

    position = tgaToWrite.dataPositions[9];
    // tgaFile.seekg(position);
    tgaFile.write(reinterpret_cast<char*>(&tgaToWrite.image_height_),
                  sizeof(unsigned short));

    std::cout << "Field image_height_ \"Image height\" with value: "
              << tgaToWrite.image_height_ << "\n";

    position = tgaToWrite.dataPositions[10];
    // tgaFile.seekg(position);
    tgaFile.write(reinterpret_cast<char*>(&tgaToWrite.pixel_depth_),
                  sizeof(std::byte));

    std::cout << "Field pixel_depth_ \"Pixel depth\" with value: "
              << to_integer<int>(tgaToWrite.pixel_depth_) << "\n";

    position = tgaToWrite.dataPositions[11];
    // tgaFile.seekg(position);
    tgaFile.write(reinterpret_cast<char*>(&tgaToWrite.image_descriptor_),
                  sizeof(std::byte));
    std::cout << "Field image_descriptor_ \"Image descriptor\"\n"
              << "Image descriptor has 1 byte of size which include in it:\n"
              << "bits 3–0 give the alpha channel depth, bits 5–4 give pixel "
                 "ordering\n"
              << "Bit 4 of the image descriptor byte indicates right-to-left "
                 "pixel ordering if set.\n"
              << "Bit 5 indicates an ordering of top-to-bottom. Otherwise, "
                 "pixels are stored in bottom-to-top, left-to-right order."
              << "Field image descriptor whole value is: "
              << to_integer<int>(tgaToWrite.image_descriptor_) << "\n";

    std::cout << "Reading Image and color map data:\n";

    if (to_integer<int>(tgaToWrite.id_length_) != 0) {
      position = tgaToWrite.dataPositions[12];
      // tgaFile.seekg(position);
      tgaToWrite.image_id_ =
          new std::byte[to_integer<int>(tgaToWrite.id_length_)];
      tgaFile.write(reinterpret_cast<char*>(tgaToWrite.image_id_),
                    to_integer<int>(tgaToWrite.id_length_));
      std::cout << "Field image_id_ \"Image ID\" with value: "
                << to_integer<int>(tgaToWrite.id_length_);
    }

    std::cout << "Calculating Color map data position in file:\n";

    tgaToWrite.dataPositions[13] =
        tgaToWrite.dataPositions[12] + to_integer<int>(tgaToWrite.id_length_);

    std::cout << "Calculated position: " << tgaToWrite.dataPositions[13]
              << "\n";

    int sizeOfColorMapInBytes = 0;

    if (tgaToWrite.first_entry_index_ != 0) {
      position = tgaToWrite.dataPositions[13];
      // tgaFile.seekg(position);
      std::cout << "Calculating size of color map in bytes" << "\n";

      sizeOfColorMapInBytes =
          (tgaToWrite.color_map_length_ *
           to_integer<int>(tgaToWrite.color_map_entry_size_)) /
          8;

      std::cout << "Calculated value: " << sizeOfColorMapInBytes << "\n";
      tgaToWrite.color_map_ = new std::byte[sizeOfColorMapInBytes];
      tgaFile.write(reinterpret_cast<char*>(tgaToWrite.color_map_),
                    sizeOfColorMapInBytes);
      std::cout << "Field color_map_ \"Color map data\" with value: "
                << tgaToWrite.color_map_ << "\n";
    }
    std::cout << "Calculating Image data position in file\n";

    tgaToWrite.dataPositions[14] =
        tgaToWrite.dataPositions[13] + sizeOfColorMapInBytes;

    std::cout << "Calculated position: " << tgaToWrite.dataPositions[14]
              << "\n";

    position = tgaToWrite.dataPositions[14];
    // tgaFile.seekg(position);

    std::cout << "Calculating Image data size in bytes:\n";

    int imageDataSize = ((tgaToWrite.image_width_ * tgaToWrite.image_height_ *
                          (to_integer<int>(tgaToWrite.pixel_depth_) / 8)));

    std::cout << "Calculated size: " << imageDataSize << "\n";

    // tgaToWrite.image_data_ = new std::byte[imageDataSize];

    std::cout << "Reading Image data:\n";

    tgaFile.write(reinterpret_cast<char*>(tgaToWrite.image_data_),
                  imageDataSize);

  } else {
    std::cout << "Error while writing tga file!" << "\n";
  }

  std::cout << "Image file: " << fileName.c_str()
            << " saved successfully.\n\n\n\n";

  tgaFile.close();
}

void YasEngine::PrepareDataForDrawingGraphs() {

}

void YasEngine::PrepareInterface() {
  // Button 1
  buttons_.push_back(new Button(Button::kRestartStart, "START", kRed, writer_));
  buttons_.at(0)->set_position(0, 50);
  dynamic_cast<Button*>(buttons_.at(0))->horizontal_margin_ = 10;
  dynamic_cast<Button*>(buttons_.at(0))->vertical_margin_ = 5;
  dynamic_cast<Button*>(buttons_.at(0))->button_width_ =
    writer_->kfont_width_ *
    static_cast<int>(
      dynamic_cast<Button*>(buttons_.at(0))->text_.size()) +
    2 * dynamic_cast<Button*>(buttons_.at(0))->horizontal_margin_;
  dynamic_cast<Button*>(buttons_.at(0))->button_height_ =
    writer_->kfont_height_ +
    2 * dynamic_cast<Button*>(buttons_.at(0))->vertical_margin_;
  buttons_.at(0)->local_vertices_ = new Vector2D<float>[4];
  buttons_.at(0)->world_vertices_ = new Vector2D<float>[4];
  buttons_.at(0)->local_vertices_[0].x_ =
    0 - dynamic_cast<Button*>(buttons_.at(0))->button_width_ * 0.5F;
  buttons_.at(0)->local_vertices_[0].y_ =
    0 + dynamic_cast<Button*>(buttons_.at(0))->button_height_ * 0.5F;
  buttons_.at(0)->local_vertices_[1].x_ =
    0 + dynamic_cast<Button*>(buttons_.at(0))->button_width_ * 0.5F;
  buttons_.at(0)->local_vertices_[1].y_ =
    0 + dynamic_cast<Button*>(buttons_.at(0))->button_height_ * 0.5F;
  buttons_.at(0)->local_vertices_[2].x_ =
    0 + dynamic_cast<Button*>(buttons_.at(0))->button_width_ * 0.5F;
  buttons_.at(0)->local_vertices_[2].y_ =
    0 - dynamic_cast<Button*>(buttons_.at(0))->button_height_ * 0.5F;
  buttons_.at(0)->local_vertices_[3].x_ =
    0 - dynamic_cast<Button*>(buttons_.at(0))->button_width_ * 0.5F;
  buttons_.at(0)->local_vertices_[3].y_ =
    0 - dynamic_cast<Button*>(buttons_.at(0))->button_height_ * 0.5F;
  buttons_.at(0)->Generate();

  // Button 2
  buttons_.push_back(new Button(Button::kQuit, "QUIT", kYellow, writer_));
  buttons_.at(1)->set_position(0, -50);
  dynamic_cast<Button*>(buttons_.at(1))->horizontal_margin_ = 10;
  dynamic_cast<Button*>(buttons_.at(1))->vertical_margin_ = 5;
  dynamic_cast<Button*>(buttons_.at(1))->button_width_ =
    writer_->kfont_width_ *
    static_cast<int>(
      dynamic_cast<Button*>(buttons_.at(1))->text_.size()) +
    2 * dynamic_cast<Button*>(buttons_.at(1))->horizontal_margin_;
  dynamic_cast<Button*>(buttons_.at(1))->button_height_ =
    writer_->kfont_height_ +
    2 * dynamic_cast<Button*>(buttons_.at(1))->vertical_margin_;
  buttons_.at(1)->local_vertices_ = new Vector2D<float>[4];
  buttons_.at(1)->world_vertices_ = new Vector2D<float>[4];
  buttons_.at(1)->local_vertices_[0].x_ =
    0 - dynamic_cast<Button*>(buttons_.at(1))->button_width_ * 0.5F;
  buttons_.at(1)->local_vertices_[0].y_ =
    0 + dynamic_cast<Button*>(buttons_.at(1))->button_height_ * 0.5F;
  buttons_.at(1)->local_vertices_[1].x_ =
    0 + dynamic_cast<Button*>(buttons_.at(1))->button_width_ * 0.5F;
  buttons_.at(1)->local_vertices_[1].y_ =
    0 + dynamic_cast<Button*>(buttons_.at(1))->button_height_ * 0.5F;
  buttons_.at(1)->local_vertices_[2].x_ =
    0 + dynamic_cast<Button*>(buttons_.at(1))->button_width_ * 0.5F;
  buttons_.at(1)->local_vertices_[2].y_ =
    0 - dynamic_cast<Button*>(buttons_.at(1))->button_height_ * 0.5F;
  buttons_.at(1)->local_vertices_[3].x_ =
    0 - dynamic_cast<Button*>(buttons_.at(1))->button_width_ * 0.5F;
  buttons_.at(1)->local_vertices_[3].y_ =
    0 - dynamic_cast<Button*>(buttons_.at(1))->button_height_ * 0.5F;
  buttons_.at(1)->Generate();
}

void YasEngine::DrawButtons() {
  for (unsigned int i = 0; i < buttons_.size(); i++) {
    DrawPolygon(buttons_.at(i), *pixels_table_);
    writer_->Write(
      static_cast<int>(
        buttons_.at(i)->get_position().x_ -
        dynamic_cast<Button*>(buttons_.at(i))->button_text_width_ * 0.5F +
        writer_->kfont_width_ * 0.5F),
      static_cast<int>(buttons_.at(i)->get_position().y_),
      dynamic_cast<Button*>(buttons_.at(i))->text_,
      dynamic_cast<Button*>(buttons_.at(i))->color_, *pixels_table_);
  }
}

Button::ButtonId YasEngine::CheckWhichButtonClicked() {
  float x = static_cast<float>(mouse_position_change_information_->x);
  float y = static_cast<float>(mouse_position_change_information_->y);
  WindowPositionToCartesianPosition(x, y, window_dimensions_);
  for (unsigned int i = 0; i < buttons_.size(); i++) {
    if (
      // mouse cursor under top Y
      y <= (buttons_.at(i)->get_position().y_ +
        dynamic_cast<Button*>(buttons_.at(i))->button_height_ * 0.5F) &&
      // mouser cursor above bottom Y
      y >= (buttons_.at(i)->get_position().y_ -
        dynamic_cast<Button*>(buttons_.at(i))->button_height_ * 0.5F) &&
      // cursor to the right of left X
      x >= (buttons_.at(i)->get_position().x_ -
        dynamic_cast<Button*>(buttons_.at(i))->button_width_ * 0.5F) &&
      // cursor to the left of X
      x <= (buttons_.at(i)->get_position().x_ +
        dynamic_cast<Button*>(buttons_.at(i))->button_width_ * 0.5F)) {
      return dynamic_cast<Button*>(buttons_.at(i))->button_id_;
    }
  }
  return Button::kNone;
}

void YasEngine::HandleClickedButtons() {
  switch (CheckWhichButtonClicked()) {
  case Button::kRestartStart:
    game_state_ = GameState::kGameplay;
    break;
  case Button::kQuit:
    game_state_ = GameState::kOutro;
    break;
  default: ;
  }
}

void YasEngine::HandleGameStateWhenESCbuttonPushed() {
  switch (game_state_) {
  case kIntro:
    game_state_ = GameState::kMainMenuRestart;
    break;
  case kMainMenuRestart:
    game_state_ = GameState::kOutro;
    break;
  case kGameplay:
    game_state_ = GameState::kMainMenuRestart;
    break;
  case kOutro:
    quit_ = true;
    break;
  case kLevelChangeScreen:
    game_state_ = GameState::kGameplay;
    break;
  case kYouWon:
    game_state_ = GameState::kMainMenuRestart;
    player_won_and_exited_ = true;
    break;
  default: ;
  }
}

void YasEngine::HandleGameStateWhenSPACEbuttonPushed() {
  switch (game_state_) {
  case kIntro:
    game_state_ = GameState::kMainMenuRestart;
    break;
  case kOutro:
    quit_ = true;
    break;
  case kLevelChangeScreen:
    game_state_ = GameState::kGameplay;
    break;
  case kGameplay:
    level_changed_ = false;
  // break;
    break;
  case kYouWon:
    game_state_ = GameState::kMainMenuRestart;
    player_won_and_exited_ = true;
    break;
  default: ;
  }
}
