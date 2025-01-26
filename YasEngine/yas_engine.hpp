#ifndef YASENGINE_HPP
#define YASENGINE_HPP
#include <SDL_mixer.h>
#include <document.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "button.hpp"
#include "game_object.hpp"
#include "input_output_handler.hpp"
#include "line_2d.hpp"
#include "map_frame.hpp"
#include "math_picture.hpp"
#include "mathematics_graphs_surface.hpp"
#include "node.hpp"
#include "pixels_table.hpp"
#include "player.hpp"
#include "screen_writer.hpp"
#include "tga.hpp"
#include "vector_2d.hpp"

class YasEngine {
 public:
  Tga tga;
  Tga tgaFileWithDefaultValues;

  bool tests_ = false;
  std::string engine_version_;
  int MAJOR_REVISION = 1;
  int MINOR_REVISION = 3;
  int BUG_FIX_RELEASE = 0;
  int BUILD_NUMBER = 0;

  rapidjson::Document settings;
  int music_volume_ = MIX_MAX_VOLUME;
  int shoot_volume_ = MIX_MAX_VOLUME;
  int hit_volume_ = MIX_MAX_VOLUME;
  int other_volume_ = MIX_MAX_VOLUME;

  bool collided_ = false;
  GameObject* game_object_;

  enum GameState {
    kIntro,
    kMainMenuRestart,
    kGameplay,
    kOutro,
    kPause,
    kLevelChangeScreen,
    kYouWon
  };

  bool player_won_and_exited_ = false;

  enum Wall { kLeft, kRight, kTop, kBottom };

  struct NodeNumbersOnTwoProceedingLevels {
    int first_level_node = 0;
    int second_level_node = 0;

    NodeNumbersOnTwoProceedingLevels() {
      first_level_node = 0;
      second_level_node = 0;
    }

    NodeNumbersOnTwoProceedingLevels(int first, int second) {
      this->first_level_node = first;
      this->second_level_node = second;
    }
  };

  std::vector<Vector2D<int>*> test_positions_;

  GameState game_state_ = GameState::kIntro;

  std::vector<GameObject*> buttons_;

  int base_points_number_ = 64;

  std::vector<std::string> main_menu_texts_;
  std::vector<std::string> intro_texts_;
  std::vector<std::string> outro_texts_;
  std::vector<std::string> in_game_texts_;

  static YasEngine* GetInstance() {
    if (instance_ != nullptr) {
      return instance_;
    }
    instance_ = new YasEngine();
    return instance_;
  }

  void initialize();
  void YasEngineStart();
  void clean();

 private:
  TimePicker time_picker_;

  Vector2D<float> A = Vector2D<float>(-300, 200);
  Vector2D<float> B = Vector2D<float>(100, -350);

  static YasEngine* instance_;

  int endianness_;

  SDL_Window* window_;
  SDL_Renderer* renderer_;
  SDL_Texture* screen_texture_;

  PixelsTable* pixels_table_;
  Vector2D<int>* window_dimensions_;
  SDL_Event event_;
  bool quit_ = false;

  double time_;
  double new_time_;
  double delta_time_;
  double fps_;
  double fps_time_;
  unsigned int frames_;

  float mouse_x_;
  float mouse_y_;

  int window_width_ = 512;
  int window_height_ = 512;

  std::vector<GameObject*> objects_to_draw_;
  bool first_16_spawned_ = false;
  int how_many_ = 0;
  Player* player_;

  const int kmax_collectibles_to_spawn_ = 64;

  std::map<std::string, std::map<int, float>*> numbers_map_;
  std::map<std::string, std::map<int, std::map<float, float>*>*>
      pair_numbers_map_;

  SDL_AudioSpec audio_specs_;

  Mix_Music* music_;
  Mix_Chunk* shoot_sound_;
  Mix_Chunk* hit_sound_;
  Mix_Chunk* other_sound_;

  MathPicture* sine_picture_;
  MathPicture* cosine_picture_;
  MathPicture* fibonaccie_picture_;
  MathPicture* prime_numbers_picture_;

  int level_ = 1;
  int previous_level_ = 0;
  bool level_changed_ = false;
  const int kmax_level_ = 4;

  std::map<std::string, int> number_of_given_colors_;
  Node* spawners_;

  Vector2D<float> test_point_0_;
  Vector2D<float> test_point_1_;
  Line2D<float> test_line;
  Vector4D<Uint8> test_color_;// RGBA

  ScreenWriter* writer_;
  int step_ = 0;

  MapFrame map_frame_;

  YasEngine() {};
  bool should_application_stop_running_ = false;
  YasInOut::Input* input_ = new YasInOut::Input();
  YasInOut::MousePositionChangeInformation* mouse_position_change_information_ =
      new YasInOut::MousePositionChangeInformation();
  int red_shot_downs_ = 0;
  int green_shotdowns_ = 0;
  int blue_shotdowns_ = 0;
  bool is_engine_instantiated_ = false;

  std::string line_1 = "YOU.JUST.FINISHED.LEVEL.1";
  std::string line_2 = "YOU.ARE.HAPPY.TO.HAVE.DISCOVERED";
  std::string line_3 = "PRIME.NUMBERS";

  std::string line_4 = "YOU.JUST.FINISHED.LEVEL.2";
  std::string line_5 = "YOU.ARE.HAPPY.TO.HAVE.DISCOVERED";
  std::string line_6 = "FIBONACCI.NUMBERS";

  std::string line_7 = "YOU.JUST.FINISHED.LEVEL.2";
  std::string line_8 = "YOU.ARE.HAPPY.TO.HAVE.DISCOVERED";
  std::string line_9 = "SINE.FUNCTION";

  std::string line_10 = "YOU.JUST.FINISHED.LEVEL.2";
  std::string line_11 = "YOU.ARE.HAPPY.TO.HAVE.DISCOVERED";
  std::string line_12 = "COSINE.FUNCTION";
  std::string line_13 = "AND.YOU.WON";

  std::string line_14 = "YOU.WON";
  std::string line_15 = "MATHEMATICS.IS.BEAUTIFUL";
  std::string line_16 = "YOU.ARE.NOT.SUPPOSED.TO.BELIEVE.ME";
  std::string line_17 = "CHECK.IT.FOR.YOURSELF";
  std::string line_18 =
      "FIND.AND.LEARN.MORE.ABOUT.WHAT.YOU.HAVE.JUST.DISCOVERED";
  std::string line_19 = "PRIME.NUMBERS";
  std::string line_20 = "FIBONACCI.NUMBERS";
  std::string line_21 = "SINE.AND.COSINE.FUNCTIONS";

  std::string line_22 = "BEAUTY.OF.MATH";
  std::string line_23 = "POWERED.BY";
  std::string line_24 = "YAS.ENGINE";

  std::string line_25 = "GAME.DESIGN.PROGRAMMING.AND.MARKETING";
  std::string line_26 = "LUKASZ.SAWICKI";
  std::string line_27 = "SOUND.DESIGN.AND.MUSIC";
  std::string line_28 = "JAKUB.TWAROGOWSKI";
  std::string line_29 = "QUALITY.ASSURANCE";
  std::string line_30 = "BARTLOMIEJ.KAWA";
  std::string line_31 = "SPECIAL.THANKS:";
  std::string line_32 = "MY.DEAR.SISTER.IZABELA";
  std::string line_33 = "MY.LOVE.MARIOLA";
  std::string line_34 = "MY.FRIENDS.FROM.WARSAW.SCHOOL.OF.COMPUTER.SCIENCE:";
  std::string line_35 = "LUKASZ.KRZYSZTOF.MICHAL.MAREK.TOMASZ";
  std::string line_36 = "MY.FRENDS.FROM.GDS.4:";
  std::string line_37 = "KASIA.AND.BARTOSZ";
  std::string line_38 = "WHOLE.COMMUNITY.OF.KNTG.POLYGON";
  std::string line_39 = "AND.ALL.MEMBERS.OF.TEAM.XPORTAL";
  std::string line_40 = "GAME.DESIGN.PROGRAMMING.AND.MARKETING";
  std::string line_41 = "LUKASZ.SAWICKI";
  std::string line_42 = "SOUND.DESIGN.AND.MUSIC";
  std::string line_43 = "JAKUB.TWAROGOWSKI";
  std::string line_44 = "QUALITY.ASSURANCE";
  std::string line_45 = "BARTLOMIEJ.KAWA";
  std::string line_46 = "SPECIAL.THANKS:";
  std::string line_47 = "MY.DEAR.SISTER.IZABELA";
  std::string line_48 = "MY.LOVE.MARIOLA";
  std::string line_49 = "MY.FRIENDS.FROM.WARSAW.SCHOOL.OF.COMPUTER.SCIENCE:";
  std::string line_50 = "LUKASZ.KRZYSZTOF.MICHAL.MAREK.TOMASZ";
  std::string line_51 = "MY.FRENDS.FROM.GDS.4:";
  std::string line_52 = "KASIA.AND.BARTOSZ";
  std::string line_53 = "WHOLE.COMMUNITY.OF.KNTG.POLYGON";
  std::string line_54 = "AND.ALL.MEMBERS.OF.TEAM.XPORTAL";

  void ReadSettingsFromFile();
  void PrepareRendering();
  void PrepareBasicSettings();
  void CheckEndianness();
  void HandlePhysics();
  bool IsObjectProtagonist(GameObject* game_object);
  GameObject* GetProtagonist(GameObject* game_object_0, GameObject* game_object_1);
  GameObject* GetNotProtagonist(GameObject* game_object_0, GameObject* game_object_1);
  void MoveObjects();
  void PrepareGameWorld();
  void LoadGraphicsFile(std::string fileName);  //"example.tga"
  void SaveGraphicsFile(std::string fileName, Uint8* pixelsTable, short imageWidth, short imageHeight);
  void PrepareDataForDrawingGraphs();
  void PreparePlayer();
  void PrepareInterface();
  void HandleInput(SDL_Event& event);
  void HandleKeyboardInput(SDL_Event& event);
  void HandleMouseInput(SDL_Event& event);
  void HandleMouseMovement();
  void DeleteNotAliveObjects();
  void HandleSpawningCollectibles();
  bool IsObjectInSameQuarterAsProtagonist(int random_spawner);
  void HandleProjectiles();
  void HandlePlayer();
  void Update(double& delta_time);
  void ResetAll();
  void DrawHudElements(double& delta_time);
  void Render(double& delta_time);
  void DrawTestStuff();
  void RenderGameObjects();
  void RenderLevelChange();
  void RenderWonScreen();
  Button::ButtonId CheckWhichButtonClicked();
  void HandleClickedButtons();
  void HandleGameStateWhenESCbuttonPushed();
  void HandleGameStateWhenSPACEbuttonPushed();
  void PrepareSoundAndMusic();
  void DrawButtons();
};

#endif
