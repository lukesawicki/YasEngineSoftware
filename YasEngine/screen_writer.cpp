#include "screen_writer.hpp"

#include <iostream>
#include <sstream>

#include "yas_graphics_library.hpp"
using namespace std;

ScreenWriter::ScreenWriter() {
  int step;
  step = 65;
  for (int i = 0; i < 26; i++) {
    characters_table_[i] = static_cast<char>(step++);
  }
  step = 48;  // Digits
  for (int i = 26; i < 36; i++) {
    characters_table_[i] = static_cast<char>(step++);
  }
  characters_table_[36] = ' ';

  for (int i = 0; i < knumber_of_characters; i++) {
    fonts_.push_back(new Font());
  }
}

void ScreenWriter::initialize() {
  initializeFontSurfaces();
  initializeFontObjects();
  prepareFontVertices();
}

void ScreenWriter::initialize(int szerokosc_znaku, int wysokosc_znaku,
                              const char* plik_znakow) {}

void ScreenWriter::writeNew(int x, int y, string text, int width, int height) {
  SDL_Rect docelowe;
  int pom_w = 0, pom_h = 0;
  docelowe.x = x;
  docelowe.y = y;
  docelowe.w = width;
  docelowe.h = height;
  for (int i = 0; i < static_cast<int>(text.size()); i++) {
    for (int j = 0; j < 63; j++) {
      if (text.at(i) == characters_table_[j]) {
        docelowe.x = x + i * docelowe.w;
      }
    }
  }
}

void ScreenWriter::write(int x, int y, string text,
                         const Vector4D<Uint8>& color,
                         PixelsTable& pixelsTable) {
  for (int i = 0; i < static_cast<int>(text.size()); i++) {
    for (int j = 0; j < knumber_of_characters; j++) {
      if (text.at(i) == characters_table_[j]) {
        fonts_.at(j)->verticesBaseData->setPosition(
            static_cast<float>(x + i * kfont_width), static_cast<float>(y));
        fonts_.at(j)->verticesBaseData->generate();
        drawNumbersAsGroupOfLines(
            fonts_.at(j)->verticesBaseData->world_vertices_,
            fonts_.at(j)->verticesBaseData->number_of_vertices_, color, false,
            pixelsTable);
      }
    }
  }
}

void ScreenWriter::initializeFontObjects() {
  Vector2D<float> direction(0, 1);
  for (int i = 0; i < knumber_of_characters; i++) {
    fonts_.at(i)->verticesBaseData->initialize(17, 0, 0, direction, -1);
  }
}

void ScreenWriter::initializeFontSurfaces() {
  for (int i = 0; i < knumber_of_characters; i++) {
    fonts_.at(i)->surface->initialize(i * 17, 0, 17, 17, kGreen);
  }
}

void ScreenWriter::prepareFontVertices() {
  // A 6
  fonts_[0]->verticesBaseData->local_vertices_ = new Vector2D<float>[6];
  fonts_[0]->verticesBaseData->world_vertices_ = new Vector2D<float>[6];
  fonts_[0]->verticesBaseData->number_of_vertices_ = 6;

  fonts_[0]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[0]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[0]->verticesBaseData->local_vertices_[1].x = 0;
  fonts_[0]->verticesBaseData->local_vertices_[1].y = 7;

  fonts_[0]->verticesBaseData->local_vertices_[2].x = 0;
  fonts_[0]->verticesBaseData->local_vertices_[2].y = 7;

  fonts_[0]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[0]->verticesBaseData->local_vertices_[3].y = -7;

  fonts_[0]->verticesBaseData->local_vertices_[4].x = -2;
  fonts_[0]->verticesBaseData->local_vertices_[4].y = 0;

  fonts_[0]->verticesBaseData->local_vertices_[5].x = 2;
  fonts_[0]->verticesBaseData->local_vertices_[5].y = 0;

  // B 16
  fonts_[1]->verticesBaseData->local_vertices_ = new Vector2D<float>[16];
  fonts_[1]->verticesBaseData->world_vertices_ = new Vector2D<float>[16];

  fonts_[1]->verticesBaseData->number_of_vertices_ = 16;

  fonts_[1]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[1]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[1]->verticesBaseData->local_vertices_[1].x = -5;
  fonts_[1]->verticesBaseData->local_vertices_[1].y = 0;

  fonts_[1]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[1]->verticesBaseData->local_vertices_[2].y = 0;

  fonts_[1]->verticesBaseData->local_vertices_[3].x = -5;
  fonts_[1]->verticesBaseData->local_vertices_[3].y = 7;

  fonts_[1]->verticesBaseData->local_vertices_[4].x = -5;
  fonts_[1]->verticesBaseData->local_vertices_[4].y = 7;

  fonts_[1]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[1]->verticesBaseData->local_vertices_[5].y = 6;

  fonts_[1]->verticesBaseData->local_vertices_[6].x = 5;
  fonts_[1]->verticesBaseData->local_vertices_[6].y = 6;

  fonts_[1]->verticesBaseData->local_vertices_[7].x = 5;
  fonts_[1]->verticesBaseData->local_vertices_[7].y = 3;

  fonts_[1]->verticesBaseData->local_vertices_[8].x = 5;
  fonts_[1]->verticesBaseData->local_vertices_[8].y = 3;

  fonts_[1]->verticesBaseData->local_vertices_[9].x = -5;
  fonts_[1]->verticesBaseData->local_vertices_[9].y = 0;

  fonts_[1]->verticesBaseData->local_vertices_[10].x = -5;
  fonts_[1]->verticesBaseData->local_vertices_[10].y = 0;

  fonts_[1]->verticesBaseData->local_vertices_[11].x = 5;
  fonts_[1]->verticesBaseData->local_vertices_[11].y = -2;

  fonts_[1]->verticesBaseData->local_vertices_[12].x = 5;
  fonts_[1]->verticesBaseData->local_vertices_[12].y = -2;

  fonts_[1]->verticesBaseData->local_vertices_[13].x = 5;
  fonts_[1]->verticesBaseData->local_vertices_[13].y = -6;

  fonts_[1]->verticesBaseData->local_vertices_[14].x = 5;
  fonts_[1]->verticesBaseData->local_vertices_[14].y = -6;

  fonts_[1]->verticesBaseData->local_vertices_[15].x = -5;
  fonts_[1]->verticesBaseData->local_vertices_[15].y = -7;

  // C 6
  fonts_[2]->verticesBaseData->local_vertices_ = new Vector2D<float>[6];
  fonts_[2]->verticesBaseData->world_vertices_ = new Vector2D<float>[6];

  fonts_[2]->verticesBaseData->number_of_vertices_ = 6;

  fonts_[2]->verticesBaseData->local_vertices_[0].x = 5;
  fonts_[2]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[2]->verticesBaseData->local_vertices_[1].x = -5;
  fonts_[2]->verticesBaseData->local_vertices_[1].y = -5;

  fonts_[2]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[2]->verticesBaseData->local_vertices_[2].y = -5;

  fonts_[2]->verticesBaseData->local_vertices_[3].x = -5;
  fonts_[2]->verticesBaseData->local_vertices_[3].y = 5;

  fonts_[2]->verticesBaseData->local_vertices_[4].x = -5;
  fonts_[2]->verticesBaseData->local_vertices_[4].y = 5;

  fonts_[2]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[2]->verticesBaseData->local_vertices_[5].y = 7;

  // D 8
  fonts_[3]->verticesBaseData->local_vertices_ = new Vector2D<float>[8];
  fonts_[3]->verticesBaseData->world_vertices_ = new Vector2D<float>[8];

  fonts_[3]->verticesBaseData->number_of_vertices_ = 8;

  fonts_[3]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[3]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[3]->verticesBaseData->local_vertices_[1].x = -5;
  fonts_[3]->verticesBaseData->local_vertices_[1].y = 7;

  fonts_[3]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[3]->verticesBaseData->local_vertices_[2].y = 7;

  fonts_[3]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[3]->verticesBaseData->local_vertices_[3].y = 5;

  fonts_[3]->verticesBaseData->local_vertices_[4].x = 5;
  fonts_[3]->verticesBaseData->local_vertices_[4].y = 5;

  fonts_[3]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[3]->verticesBaseData->local_vertices_[5].y = -5;

  fonts_[3]->verticesBaseData->local_vertices_[6].x = 5;
  fonts_[3]->verticesBaseData->local_vertices_[6].y = -5;

  fonts_[3]->verticesBaseData->local_vertices_[7].x = -5;
  fonts_[3]->verticesBaseData->local_vertices_[7].y = -7;

  // E 10
  fonts_[4]->verticesBaseData->local_vertices_ = new Vector2D<float>[10];
  fonts_[4]->verticesBaseData->world_vertices_ = new Vector2D<float>[10];

  fonts_[4]->verticesBaseData->number_of_vertices_ = 10;

  fonts_[4]->verticesBaseData->local_vertices_[0].x = 5;
  fonts_[4]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[4]->verticesBaseData->local_vertices_[1].x = -5;
  fonts_[4]->verticesBaseData->local_vertices_[1].y = -7;

  fonts_[4]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[4]->verticesBaseData->local_vertices_[2].y = -7;

  fonts_[4]->verticesBaseData->local_vertices_[3].x = -5;
  fonts_[4]->verticesBaseData->local_vertices_[3].y = 0;

  fonts_[4]->verticesBaseData->local_vertices_[4].x = -5;
  fonts_[4]->verticesBaseData->local_vertices_[4].y = 0;

  fonts_[4]->verticesBaseData->local_vertices_[5].x = -5;
  fonts_[4]->verticesBaseData->local_vertices_[5].y = 7;

  fonts_[4]->verticesBaseData->local_vertices_[6].x = -5;
  fonts_[4]->verticesBaseData->local_vertices_[6].y = 7;

  fonts_[4]->verticesBaseData->local_vertices_[7].x = 5;
  fonts_[4]->verticesBaseData->local_vertices_[7].y = 7;

  fonts_[4]->verticesBaseData->local_vertices_[8].x = -5;
  fonts_[4]->verticesBaseData->local_vertices_[8].y = 0;

  fonts_[4]->verticesBaseData->local_vertices_[9].x = 5;
  fonts_[4]->verticesBaseData->local_vertices_[9].y = 0;

  // F 8
  fonts_[5]->verticesBaseData->local_vertices_ = new Vector2D<float>[8];
  fonts_[5]->verticesBaseData->world_vertices_ = new Vector2D<float>[8];

  fonts_[5]->verticesBaseData->number_of_vertices_ = 8;

  fonts_[5]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[5]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[5]->verticesBaseData->local_vertices_[1].x = -5;
  fonts_[5]->verticesBaseData->local_vertices_[1].y = 0;

  fonts_[5]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[5]->verticesBaseData->local_vertices_[2].y = 0;

  fonts_[5]->verticesBaseData->local_vertices_[3].x = -5;
  fonts_[5]->verticesBaseData->local_vertices_[3].y = 7;

  fonts_[5]->verticesBaseData->local_vertices_[4].x = -5;
  fonts_[5]->verticesBaseData->local_vertices_[4].y = 7;

  fonts_[5]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[5]->verticesBaseData->local_vertices_[5].y = 7;

  fonts_[5]->verticesBaseData->local_vertices_[6].x = -5;
  fonts_[5]->verticesBaseData->local_vertices_[6].y = 0;

  fonts_[5]->verticesBaseData->local_vertices_[7].x = 5;
  fonts_[5]->verticesBaseData->local_vertices_[7].y = 0;

  // G 10
  fonts_[6]->verticesBaseData->local_vertices_ = new Vector2D<float>[10];
  fonts_[6]->verticesBaseData->world_vertices_ = new Vector2D<float>[10];

  fonts_[6]->verticesBaseData->number_of_vertices_ = 10;

  fonts_[6]->verticesBaseData->local_vertices_[0].x = 1;
  fonts_[6]->verticesBaseData->local_vertices_[0].y = -1;

  fonts_[6]->verticesBaseData->local_vertices_[1].x = 4;
  fonts_[6]->verticesBaseData->local_vertices_[1].y = -1;

  fonts_[6]->verticesBaseData->local_vertices_[2].x = 4;
  fonts_[6]->verticesBaseData->local_vertices_[2].y = -1;

  fonts_[6]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[6]->verticesBaseData->local_vertices_[3].y = -7;

  fonts_[6]->verticesBaseData->local_vertices_[4].x = 5;
  fonts_[6]->verticesBaseData->local_vertices_[4].y = -7;

  fonts_[6]->verticesBaseData->local_vertices_[5].x = -5;
  fonts_[6]->verticesBaseData->local_vertices_[5].y = -5;

  fonts_[6]->verticesBaseData->local_vertices_[6].x = -5;
  fonts_[6]->verticesBaseData->local_vertices_[6].y = -5;

  fonts_[6]->verticesBaseData->local_vertices_[7].x = -5;
  fonts_[6]->verticesBaseData->local_vertices_[7].y = 5;

  fonts_[6]->verticesBaseData->local_vertices_[8].x = -5;
  fonts_[6]->verticesBaseData->local_vertices_[8].y = 5;

  fonts_[6]->verticesBaseData->local_vertices_[9].x = 5;
  fonts_[6]->verticesBaseData->local_vertices_[9].y = 7;

  // H 6
  fonts_[7]->verticesBaseData->local_vertices_ = new Vector2D<float>[6];
  fonts_[7]->verticesBaseData->world_vertices_ = new Vector2D<float>[6];

  fonts_[7]->verticesBaseData->number_of_vertices_ = 6;

  fonts_[7]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[7]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[7]->verticesBaseData->local_vertices_[1].x = -5;
  fonts_[7]->verticesBaseData->local_vertices_[1].y = 7;

  fonts_[7]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[7]->verticesBaseData->local_vertices_[2].y = 0;

  fonts_[7]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[7]->verticesBaseData->local_vertices_[3].y = 0;

  fonts_[7]->verticesBaseData->local_vertices_[4].x = 5;
  fonts_[7]->verticesBaseData->local_vertices_[4].y = 7;

  fonts_[7]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[7]->verticesBaseData->local_vertices_[5].y = -7;

  // I 6
  fonts_[8]->verticesBaseData->local_vertices_ = new Vector2D<float>[6];
  fonts_[8]->verticesBaseData->world_vertices_ = new Vector2D<float>[6];

  fonts_[8]->verticesBaseData->number_of_vertices_ = 6;

  fonts_[8]->verticesBaseData->local_vertices_[0].x = -1;
  fonts_[8]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[8]->verticesBaseData->local_vertices_[1].x = 1;
  fonts_[8]->verticesBaseData->local_vertices_[1].y = -7;

  fonts_[8]->verticesBaseData->local_vertices_[2].x = 0;
  fonts_[8]->verticesBaseData->local_vertices_[2].y = -7;

  fonts_[8]->verticesBaseData->local_vertices_[3].x = 0;
  fonts_[8]->verticesBaseData->local_vertices_[3].y = 7;

  fonts_[8]->verticesBaseData->local_vertices_[4].x = -1;
  fonts_[8]->verticesBaseData->local_vertices_[4].y = 7;

  fonts_[8]->verticesBaseData->local_vertices_[5].x = 1;
  fonts_[8]->verticesBaseData->local_vertices_[5].y = 7;

  // J 10
  fonts_[9]->verticesBaseData->local_vertices_ = new Vector2D<float>[10];
  fonts_[9]->verticesBaseData->world_vertices_ = new Vector2D<float>[10];

  fonts_[9]->verticesBaseData->number_of_vertices_ = 10;

  fonts_[9]->verticesBaseData->local_vertices_[0].x = -2;
  fonts_[9]->verticesBaseData->local_vertices_[0].y = -6;

  fonts_[9]->verticesBaseData->local_vertices_[1].x = -1;
  fonts_[9]->verticesBaseData->local_vertices_[1].y = -7;

  fonts_[9]->verticesBaseData->local_vertices_[2].x = -1;
  fonts_[9]->verticesBaseData->local_vertices_[2].y = -7;

  fonts_[9]->verticesBaseData->local_vertices_[3].x = 0;
  fonts_[9]->verticesBaseData->local_vertices_[3].y = -7;

  fonts_[9]->verticesBaseData->local_vertices_[4].x = 0;
  fonts_[9]->verticesBaseData->local_vertices_[4].y = -7;

  fonts_[9]->verticesBaseData->local_vertices_[5].x = 1;
  fonts_[9]->verticesBaseData->local_vertices_[5].y = -6;

  fonts_[9]->verticesBaseData->local_vertices_[6].x = 1;
  fonts_[9]->verticesBaseData->local_vertices_[6].y = -6;

  fonts_[9]->verticesBaseData->local_vertices_[7].x = 1;
  fonts_[9]->verticesBaseData->local_vertices_[7].y = 7;

  fonts_[9]->verticesBaseData->local_vertices_[8].x = 1;
  fonts_[9]->verticesBaseData->local_vertices_[8].y = 7;

  fonts_[9]->verticesBaseData->local_vertices_[9].x = -1;
  fonts_[9]->verticesBaseData->local_vertices_[9].y = 7;

  // K 8
  fonts_[10]->verticesBaseData->local_vertices_ = new Vector2D<float>[8];
  fonts_[10]->verticesBaseData->world_vertices_ = new Vector2D<float>[8];

  fonts_[10]->verticesBaseData->number_of_vertices_ = 8;

  fonts_[10]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[10]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[10]->verticesBaseData->local_vertices_[1].x = -5;
  fonts_[10]->verticesBaseData->local_vertices_[1].y = 0;

  fonts_[10]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[10]->verticesBaseData->local_vertices_[2].y = 0;

  fonts_[10]->verticesBaseData->local_vertices_[3].x = -5;
  fonts_[10]->verticesBaseData->local_vertices_[3].y = 7;

  fonts_[10]->verticesBaseData->local_vertices_[4].x = -5;
  fonts_[10]->verticesBaseData->local_vertices_[4].y = 0;

  fonts_[10]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[10]->verticesBaseData->local_vertices_[5].y = 7;

  fonts_[10]->verticesBaseData->local_vertices_[6].x = -5;
  fonts_[10]->verticesBaseData->local_vertices_[6].y = 0;

  fonts_[10]->verticesBaseData->local_vertices_[7].x = 5;
  fonts_[10]->verticesBaseData->local_vertices_[7].y = -7;

  // L 4
  fonts_[11]->verticesBaseData->local_vertices_ = new Vector2D<float>[4];
  fonts_[11]->verticesBaseData->world_vertices_ = new Vector2D<float>[4];

  fonts_[11]->verticesBaseData->number_of_vertices_ = 4;

  fonts_[11]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[11]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[11]->verticesBaseData->local_vertices_[1].x = 5;
  fonts_[11]->verticesBaseData->local_vertices_[1].y = -7;

  fonts_[11]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[11]->verticesBaseData->local_vertices_[2].y = -7;

  fonts_[11]->verticesBaseData->local_vertices_[3].x = -5;
  fonts_[11]->verticesBaseData->local_vertices_[3].y = 7;

  // M 8
  fonts_[12]->verticesBaseData->local_vertices_ = new Vector2D<float>[8];
  fonts_[12]->verticesBaseData->world_vertices_ = new Vector2D<float>[8];

  fonts_[12]->verticesBaseData->number_of_vertices_ = 8;

  fonts_[12]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[12]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[12]->verticesBaseData->local_vertices_[1].x = -5;
  fonts_[12]->verticesBaseData->local_vertices_[1].y = 7;

  fonts_[12]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[12]->verticesBaseData->local_vertices_[2].y = 7;

  fonts_[12]->verticesBaseData->local_vertices_[3].x = 0;
  fonts_[12]->verticesBaseData->local_vertices_[3].y = -7;

  fonts_[12]->verticesBaseData->local_vertices_[4].x = 0;
  fonts_[12]->verticesBaseData->local_vertices_[4].y = -7;

  fonts_[12]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[12]->verticesBaseData->local_vertices_[5].y = 7;

  fonts_[12]->verticesBaseData->local_vertices_[6].x = 5;
  fonts_[12]->verticesBaseData->local_vertices_[6].y = 7;

  fonts_[12]->verticesBaseData->local_vertices_[7].x = 5;
  fonts_[12]->verticesBaseData->local_vertices_[7].y = -7;

  // N 6
  fonts_[13]->verticesBaseData->local_vertices_ = new Vector2D<float>[6];
  fonts_[13]->verticesBaseData->world_vertices_ = new Vector2D<float>[6];

  fonts_[13]->verticesBaseData->number_of_vertices_ = 6;

  fonts_[13]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[13]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[13]->verticesBaseData->local_vertices_[1].x = -5;
  fonts_[13]->verticesBaseData->local_vertices_[1].y = 7;

  fonts_[13]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[13]->verticesBaseData->local_vertices_[2].y = 7;

  fonts_[13]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[13]->verticesBaseData->local_vertices_[3].y = -7;

  fonts_[13]->verticesBaseData->local_vertices_[4].x = 5;
  fonts_[13]->verticesBaseData->local_vertices_[4].y = -7;

  fonts_[13]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[13]->verticesBaseData->local_vertices_[5].y = 7;

  // O 8
  fonts_[14]->verticesBaseData->local_vertices_ = new Vector2D<float>[8];
  fonts_[14]->verticesBaseData->world_vertices_ = new Vector2D<float>[8];

  fonts_[14]->verticesBaseData->number_of_vertices_ = 8;

  fonts_[14]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[14]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[14]->verticesBaseData->local_vertices_[1].x = -5;
  fonts_[14]->verticesBaseData->local_vertices_[1].y = 7;

  fonts_[14]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[14]->verticesBaseData->local_vertices_[2].y = 7;

  fonts_[14]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[14]->verticesBaseData->local_vertices_[3].y = 7;

  fonts_[14]->verticesBaseData->local_vertices_[4].x = 5;
  fonts_[14]->verticesBaseData->local_vertices_[4].y = 7;

  fonts_[14]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[14]->verticesBaseData->local_vertices_[5].y = -7;

  fonts_[14]->verticesBaseData->local_vertices_[6].x = -5;
  fonts_[14]->verticesBaseData->local_vertices_[6].y = -7;

  fonts_[14]->verticesBaseData->local_vertices_[7].x = 5;
  fonts_[14]->verticesBaseData->local_vertices_[7].y = -7;

  // P 10
  fonts_[15]->verticesBaseData->local_vertices_ = new Vector2D<float>[10];
  fonts_[15]->verticesBaseData->world_vertices_ = new Vector2D<float>[10];

  fonts_[15]->verticesBaseData->number_of_vertices_ = 10;

  fonts_[15]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[15]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[15]->verticesBaseData->local_vertices_[1].x = -5;
  fonts_[15]->verticesBaseData->local_vertices_[1].y = 0;

  fonts_[15]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[15]->verticesBaseData->local_vertices_[2].y = 0;

  fonts_[15]->verticesBaseData->local_vertices_[3].x = -5;
  fonts_[15]->verticesBaseData->local_vertices_[3].y = 7;

  fonts_[15]->verticesBaseData->local_vertices_[4].x = -5;
  fonts_[15]->verticesBaseData->local_vertices_[4].y = 7;

  fonts_[15]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[15]->verticesBaseData->local_vertices_[5].y = 7;

  fonts_[15]->verticesBaseData->local_vertices_[6].x = 5;
  fonts_[15]->verticesBaseData->local_vertices_[6].y = 7;

  fonts_[15]->verticesBaseData->local_vertices_[7].x = 5;
  fonts_[15]->verticesBaseData->local_vertices_[7].y = 0;

  fonts_[15]->verticesBaseData->local_vertices_[8].x = 5;
  fonts_[15]->verticesBaseData->local_vertices_[8].y = 0;

  fonts_[15]->verticesBaseData->local_vertices_[9].x = -5;
  fonts_[15]->verticesBaseData->local_vertices_[9].y = 0;

  // Q 10
  fonts_[16]->verticesBaseData->local_vertices_ = new Vector2D<float>[10];
  fonts_[16]->verticesBaseData->world_vertices_ = new Vector2D<float>[10];

  fonts_[16]->verticesBaseData->number_of_vertices_ = 10;

  fonts_[16]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[16]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[16]->verticesBaseData->local_vertices_[1].x = -5;
  fonts_[16]->verticesBaseData->local_vertices_[1].y = 7;

  fonts_[16]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[16]->verticesBaseData->local_vertices_[2].y = 7;

  fonts_[16]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[16]->verticesBaseData->local_vertices_[3].y = 7;

  fonts_[16]->verticesBaseData->local_vertices_[4].x = 5;
  fonts_[16]->verticesBaseData->local_vertices_[4].y = 7;

  fonts_[16]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[16]->verticesBaseData->local_vertices_[5].y = -7;

  fonts_[16]->verticesBaseData->local_vertices_[6].x = 5;
  fonts_[16]->verticesBaseData->local_vertices_[6].y = -7;

  fonts_[16]->verticesBaseData->local_vertices_[7].x = -5;
  fonts_[16]->verticesBaseData->local_vertices_[7].y = -7;

  fonts_[16]->verticesBaseData->local_vertices_[8].x = 3;
  fonts_[16]->verticesBaseData->local_vertices_[8].y = -5;

  fonts_[16]->verticesBaseData->local_vertices_[9].x = 7;
  fonts_[16]->verticesBaseData->local_vertices_[9].y = -8;

  // R 12
  fonts_[17]->verticesBaseData->local_vertices_ = new Vector2D<float>[12];
  fonts_[17]->verticesBaseData->world_vertices_ = new Vector2D<float>[12];

  fonts_[17]->verticesBaseData->number_of_vertices_ = 12;

  fonts_[17]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[17]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[17]->verticesBaseData->local_vertices_[1].x = -5;
  fonts_[17]->verticesBaseData->local_vertices_[1].y = 0;

  fonts_[17]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[17]->verticesBaseData->local_vertices_[2].y = 0;

  fonts_[17]->verticesBaseData->local_vertices_[3].x = -5;
  fonts_[17]->verticesBaseData->local_vertices_[3].y = 7;

  fonts_[17]->verticesBaseData->local_vertices_[4].x = -5;
  fonts_[17]->verticesBaseData->local_vertices_[4].y = 7;

  fonts_[17]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[17]->verticesBaseData->local_vertices_[5].y = 7;

  fonts_[17]->verticesBaseData->local_vertices_[6].x = 5;
  fonts_[17]->verticesBaseData->local_vertices_[6].y = 7;

  fonts_[17]->verticesBaseData->local_vertices_[7].x = 5;
  fonts_[17]->verticesBaseData->local_vertices_[7].y = 0;

  fonts_[17]->verticesBaseData->local_vertices_[8].x = 5;
  fonts_[17]->verticesBaseData->local_vertices_[8].y = 0;

  fonts_[17]->verticesBaseData->local_vertices_[9].x = -5;
  fonts_[17]->verticesBaseData->local_vertices_[9].y = 0;

  fonts_[17]->verticesBaseData->local_vertices_[10].x = -5;
  fonts_[17]->verticesBaseData->local_vertices_[10].y = 0;

  fonts_[17]->verticesBaseData->local_vertices_[11].x = 5;
  fonts_[17]->verticesBaseData->local_vertices_[11].y = -7;

  // S 10
  fonts_[18]->verticesBaseData->local_vertices_ = new Vector2D<float>[10];
  fonts_[18]->verticesBaseData->world_vertices_ = new Vector2D<float>[10];

  fonts_[18]->verticesBaseData->number_of_vertices_ = 10;

  fonts_[18]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[18]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[18]->verticesBaseData->local_vertices_[1].x = 5;
  fonts_[18]->verticesBaseData->local_vertices_[1].y = -7;

  fonts_[18]->verticesBaseData->local_vertices_[2].x = 5;
  fonts_[18]->verticesBaseData->local_vertices_[2].y = -7;

  fonts_[18]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[18]->verticesBaseData->local_vertices_[3].y = 0;

  fonts_[18]->verticesBaseData->local_vertices_[4].x = 5;
  fonts_[18]->verticesBaseData->local_vertices_[4].y = 0;

  fonts_[18]->verticesBaseData->local_vertices_[5].x = -5;
  fonts_[18]->verticesBaseData->local_vertices_[5].y = 0;

  fonts_[18]->verticesBaseData->local_vertices_[6].x = -5;
  fonts_[18]->verticesBaseData->local_vertices_[6].y = 0;

  fonts_[18]->verticesBaseData->local_vertices_[7].x = -5;
  fonts_[18]->verticesBaseData->local_vertices_[7].y = 7;

  fonts_[18]->verticesBaseData->local_vertices_[8].x = -5;
  fonts_[18]->verticesBaseData->local_vertices_[8].y = 7;

  fonts_[18]->verticesBaseData->local_vertices_[9].x = 5;
  fonts_[18]->verticesBaseData->local_vertices_[9].y = 7;

  // T 4
  fonts_[19]->verticesBaseData->local_vertices_ = new Vector2D<float>[4];
  fonts_[19]->verticesBaseData->world_vertices_ = new Vector2D<float>[4];

  fonts_[19]->verticesBaseData->number_of_vertices_ = 4;

  fonts_[19]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[19]->verticesBaseData->local_vertices_[0].y = 7;

  fonts_[19]->verticesBaseData->local_vertices_[1].x = 5;
  fonts_[19]->verticesBaseData->local_vertices_[1].y = 7;

  fonts_[19]->verticesBaseData->local_vertices_[2].x = 0;
  fonts_[19]->verticesBaseData->local_vertices_[2].y = 7;

  fonts_[19]->verticesBaseData->local_vertices_[3].x = 0;
  fonts_[19]->verticesBaseData->local_vertices_[3].y = -7;

  // U 6
  fonts_[20]->verticesBaseData->local_vertices_ = new Vector2D<float>[6];
  fonts_[20]->verticesBaseData->world_vertices_ = new Vector2D<float>[6];

  fonts_[20]->verticesBaseData->number_of_vertices_ = 6;

  fonts_[20]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[20]->verticesBaseData->local_vertices_[0].y = 7;

  fonts_[20]->verticesBaseData->local_vertices_[1].x = -5;
  fonts_[20]->verticesBaseData->local_vertices_[1].y = -7;

  fonts_[20]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[20]->verticesBaseData->local_vertices_[2].y = -7;

  fonts_[20]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[20]->verticesBaseData->local_vertices_[3].y = -7;

  fonts_[20]->verticesBaseData->local_vertices_[4].x = 5;
  fonts_[20]->verticesBaseData->local_vertices_[4].y = -7;

  fonts_[20]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[20]->verticesBaseData->local_vertices_[5].y = 7;

  // V 4
  fonts_[21]->verticesBaseData->local_vertices_ = new Vector2D<float>[4];
  fonts_[21]->verticesBaseData->world_vertices_ = new Vector2D<float>[4];

  fonts_[21]->verticesBaseData->number_of_vertices_ = 4;

  fonts_[21]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[21]->verticesBaseData->local_vertices_[0].y = 7;

  fonts_[21]->verticesBaseData->local_vertices_[1].x = 0;
  fonts_[21]->verticesBaseData->local_vertices_[1].y = -7;

  fonts_[21]->verticesBaseData->local_vertices_[2].x = 0;
  fonts_[21]->verticesBaseData->local_vertices_[2].y = -7;

  fonts_[21]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[21]->verticesBaseData->local_vertices_[3].y = 7;

  // W 8
  fonts_[22]->verticesBaseData->local_vertices_ = new Vector2D<float>[8];
  fonts_[22]->verticesBaseData->world_vertices_ = new Vector2D<float>[8];

  fonts_[22]->verticesBaseData->number_of_vertices_ = 8;

  fonts_[22]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[22]->verticesBaseData->local_vertices_[0].y = 7;

  fonts_[22]->verticesBaseData->local_vertices_[1].x = -3;
  fonts_[22]->verticesBaseData->local_vertices_[1].y = -7;

  fonts_[22]->verticesBaseData->local_vertices_[2].x = -3;
  fonts_[22]->verticesBaseData->local_vertices_[2].y = -7;

  fonts_[22]->verticesBaseData->local_vertices_[3].x = 0;
  fonts_[22]->verticesBaseData->local_vertices_[3].y = 7;

  fonts_[22]->verticesBaseData->local_vertices_[4].x = 0;
  fonts_[22]->verticesBaseData->local_vertices_[4].y = 7;

  fonts_[22]->verticesBaseData->local_vertices_[5].x = 3;
  fonts_[22]->verticesBaseData->local_vertices_[5].y = -7;

  fonts_[22]->verticesBaseData->local_vertices_[6].x = 3;
  fonts_[22]->verticesBaseData->local_vertices_[6].y = -7;

  fonts_[22]->verticesBaseData->local_vertices_[7].x = 5;
  fonts_[22]->verticesBaseData->local_vertices_[7].y = 7;

  // X 4
  fonts_[23]->verticesBaseData->local_vertices_ = new Vector2D<float>[4];
  fonts_[23]->verticesBaseData->world_vertices_ = new Vector2D<float>[4];

  fonts_[23]->verticesBaseData->number_of_vertices_ = 4;

  fonts_[23]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[23]->verticesBaseData->local_vertices_[0].y = 7;

  fonts_[23]->verticesBaseData->local_vertices_[1].x = 5;
  fonts_[23]->verticesBaseData->local_vertices_[1].y = -7;

  fonts_[23]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[23]->verticesBaseData->local_vertices_[2].y = -7;

  fonts_[23]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[23]->verticesBaseData->local_vertices_[3].y = 7;

  // Y 6
  fonts_[24]->verticesBaseData->local_vertices_ = new Vector2D<float>[6];
  fonts_[24]->verticesBaseData->world_vertices_ = new Vector2D<float>[6];

  fonts_[24]->verticesBaseData->number_of_vertices_ = 6;

  fonts_[24]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[24]->verticesBaseData->local_vertices_[0].y = 7;

  fonts_[24]->verticesBaseData->local_vertices_[1].x = 0;
  fonts_[24]->verticesBaseData->local_vertices_[1].y = 0;

  fonts_[24]->verticesBaseData->local_vertices_[2].x = 0;
  fonts_[24]->verticesBaseData->local_vertices_[2].y = 0;

  fonts_[24]->verticesBaseData->local_vertices_[3].x = 0;
  fonts_[24]->verticesBaseData->local_vertices_[3].y = -7;

  fonts_[24]->verticesBaseData->local_vertices_[4].x = 0;
  fonts_[24]->verticesBaseData->local_vertices_[4].y = 0;

  fonts_[24]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[24]->verticesBaseData->local_vertices_[5].y = 7;

  // Z 6
  fonts_[25]->verticesBaseData->local_vertices_ = new Vector2D<float>[6];
  fonts_[25]->verticesBaseData->world_vertices_ = new Vector2D<float>[6];

  fonts_[25]->verticesBaseData->number_of_vertices_ = 6;

  fonts_[25]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[25]->verticesBaseData->local_vertices_[0].y = 7;

  fonts_[25]->verticesBaseData->local_vertices_[1].x = 5;
  fonts_[25]->verticesBaseData->local_vertices_[1].y = 7;

  fonts_[25]->verticesBaseData->local_vertices_[2].x = 5;
  fonts_[25]->verticesBaseData->local_vertices_[2].y = 7;

  fonts_[25]->verticesBaseData->local_vertices_[3].x = -5;
  fonts_[25]->verticesBaseData->local_vertices_[3].y = -7;

  fonts_[25]->verticesBaseData->local_vertices_[4].x = -5;
  fonts_[25]->verticesBaseData->local_vertices_[4].y = -7;

  fonts_[25]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[25]->verticesBaseData->local_vertices_[5].y = -7;

  // 0 10
  fonts_[26]->verticesBaseData->local_vertices_ = new Vector2D<float>[10];
  fonts_[26]->verticesBaseData->world_vertices_ = new Vector2D<float>[10];

  fonts_[26]->verticesBaseData->number_of_vertices_ = 10;

  fonts_[26]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[26]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[26]->verticesBaseData->local_vertices_[1].x = -5;
  fonts_[26]->verticesBaseData->local_vertices_[1].y = 7;

  fonts_[26]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[26]->verticesBaseData->local_vertices_[2].y = 7;

  fonts_[26]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[26]->verticesBaseData->local_vertices_[3].y = 7;

  fonts_[26]->verticesBaseData->local_vertices_[4].x = 5;
  fonts_[26]->verticesBaseData->local_vertices_[4].y = 7;

  fonts_[26]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[26]->verticesBaseData->local_vertices_[5].y = -7;

  fonts_[26]->verticesBaseData->local_vertices_[6].x = -5;
  fonts_[26]->verticesBaseData->local_vertices_[6].y = -7;

  fonts_[26]->verticesBaseData->local_vertices_[7].x = 5;
  fonts_[26]->verticesBaseData->local_vertices_[7].y = -7;

  fonts_[26]->verticesBaseData->local_vertices_[8].x = -3;
  fonts_[26]->verticesBaseData->local_vertices_[8].y = 5;

  fonts_[26]->verticesBaseData->local_vertices_[9].x = 3;
  fonts_[26]->verticesBaseData->local_vertices_[9].y = -5;

  // 1 6
  fonts_[27]->verticesBaseData->local_vertices_ = new Vector2D<float>[6];
  fonts_[27]->verticesBaseData->world_vertices_ = new Vector2D<float>[6];

  fonts_[27]->verticesBaseData->number_of_vertices_ = 6;

  fonts_[27]->verticesBaseData->local_vertices_[0].x = -1;
  fonts_[27]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[27]->verticesBaseData->local_vertices_[1].x = 1;
  fonts_[27]->verticesBaseData->local_vertices_[1].y = -7;

  fonts_[27]->verticesBaseData->local_vertices_[2].x = 0;
  fonts_[27]->verticesBaseData->local_vertices_[2].y = -7;

  fonts_[27]->verticesBaseData->local_vertices_[3].x = 0;
  fonts_[27]->verticesBaseData->local_vertices_[3].y = 7;

  fonts_[27]->verticesBaseData->local_vertices_[4].x = 0;
  fonts_[27]->verticesBaseData->local_vertices_[4].y = 7;

  fonts_[27]->verticesBaseData->local_vertices_[5].x = -2;
  fonts_[27]->verticesBaseData->local_vertices_[5].y = 5;

  // 2 10
  fonts_[28]->verticesBaseData->local_vertices_ = new Vector2D<float>[10];
  fonts_[28]->verticesBaseData->world_vertices_ = new Vector2D<float>[10];

  fonts_[28]->verticesBaseData->number_of_vertices_ = 10;

  fonts_[28]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[28]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[28]->verticesBaseData->local_vertices_[1].x = 5;
  fonts_[28]->verticesBaseData->local_vertices_[1].y = -7;

  fonts_[28]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[28]->verticesBaseData->local_vertices_[2].y = -7;

  fonts_[28]->verticesBaseData->local_vertices_[3].x = -5;
  fonts_[28]->verticesBaseData->local_vertices_[3].y = 2;

  fonts_[28]->verticesBaseData->local_vertices_[4].x = -5;
  fonts_[28]->verticesBaseData->local_vertices_[4].y = 2;

  fonts_[28]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[28]->verticesBaseData->local_vertices_[5].y = 2;

  fonts_[28]->verticesBaseData->local_vertices_[6].x = 5;
  fonts_[28]->verticesBaseData->local_vertices_[6].y = 2;

  fonts_[28]->verticesBaseData->local_vertices_[7].x = 5;
  fonts_[28]->verticesBaseData->local_vertices_[7].y = 7;

  fonts_[28]->verticesBaseData->local_vertices_[8].x = 5;
  fonts_[28]->verticesBaseData->local_vertices_[8].y = 7;

  fonts_[28]->verticesBaseData->local_vertices_[9].x = -5;
  fonts_[28]->verticesBaseData->local_vertices_[9].y = 7;

  // 3 10
  fonts_[29]->verticesBaseData->local_vertices_ = new Vector2D<float>[10];
  fonts_[29]->verticesBaseData->world_vertices_ = new Vector2D<float>[10];

  fonts_[29]->verticesBaseData->number_of_vertices_ = 10;

  fonts_[29]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[29]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[29]->verticesBaseData->local_vertices_[1].x = 5;
  fonts_[29]->verticesBaseData->local_vertices_[1].y = -7;

  fonts_[29]->verticesBaseData->local_vertices_[2].x = 5;
  fonts_[29]->verticesBaseData->local_vertices_[2].y = -7;

  fonts_[29]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[29]->verticesBaseData->local_vertices_[3].y = 2;

  fonts_[29]->verticesBaseData->local_vertices_[4].x = -5;
  fonts_[29]->verticesBaseData->local_vertices_[4].y = 2;

  fonts_[29]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[29]->verticesBaseData->local_vertices_[5].y = 2;

  fonts_[29]->verticesBaseData->local_vertices_[6].x = 5;
  fonts_[29]->verticesBaseData->local_vertices_[6].y = 2;

  fonts_[29]->verticesBaseData->local_vertices_[7].x = 5;
  fonts_[29]->verticesBaseData->local_vertices_[7].y = 7;

  fonts_[29]->verticesBaseData->local_vertices_[8].x = -5;
  fonts_[29]->verticesBaseData->local_vertices_[8].y = 7;

  fonts_[29]->verticesBaseData->local_vertices_[9].x = 5;
  fonts_[29]->verticesBaseData->local_vertices_[9].y = 7;

  // 4 6
  fonts_[30]->verticesBaseData->local_vertices_ = new Vector2D<float>[6];
  fonts_[30]->verticesBaseData->world_vertices_ = new Vector2D<float>[6];

  fonts_[30]->verticesBaseData->number_of_vertices_ = 6;

  fonts_[30]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[30]->verticesBaseData->local_vertices_[0].y = -4;

  fonts_[30]->verticesBaseData->local_vertices_[1].x = 5;
  fonts_[30]->verticesBaseData->local_vertices_[1].y = -4;

  fonts_[30]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[30]->verticesBaseData->local_vertices_[2].y = -4;

  fonts_[30]->verticesBaseData->local_vertices_[3].x = 4;
  fonts_[30]->verticesBaseData->local_vertices_[3].y = 7;

  fonts_[30]->verticesBaseData->local_vertices_[4].x = 4;
  fonts_[30]->verticesBaseData->local_vertices_[4].y = 7;

  fonts_[30]->verticesBaseData->local_vertices_[5].x = 4;
  fonts_[30]->verticesBaseData->local_vertices_[5].y = -7;

  // 5 10
  fonts_[31]->verticesBaseData->local_vertices_ = new Vector2D<float>[10];
  fonts_[31]->verticesBaseData->world_vertices_ = new Vector2D<float>[10];

  fonts_[31]->verticesBaseData->number_of_vertices_ = 10;

  fonts_[31]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[31]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[31]->verticesBaseData->local_vertices_[1].x = 5;
  fonts_[31]->verticesBaseData->local_vertices_[1].y = -7;

  fonts_[31]->verticesBaseData->local_vertices_[2].x = 5;
  fonts_[31]->verticesBaseData->local_vertices_[2].y = -7;

  fonts_[31]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[31]->verticesBaseData->local_vertices_[3].y = 2;

  fonts_[31]->verticesBaseData->local_vertices_[4].x = -5;
  fonts_[31]->verticesBaseData->local_vertices_[4].y = 2;

  fonts_[31]->verticesBaseData->local_vertices_[5].x = 5;
  fonts_[31]->verticesBaseData->local_vertices_[5].y = 2;

  fonts_[31]->verticesBaseData->local_vertices_[6].x = -5;
  fonts_[31]->verticesBaseData->local_vertices_[6].y = 2;

  fonts_[31]->verticesBaseData->local_vertices_[7].x = -5;
  fonts_[31]->verticesBaseData->local_vertices_[7].y = 7;

  fonts_[31]->verticesBaseData->local_vertices_[8].x = -5;
  fonts_[31]->verticesBaseData->local_vertices_[8].y = 7;

  fonts_[31]->verticesBaseData->local_vertices_[9].x = 5;
  fonts_[31]->verticesBaseData->local_vertices_[9].y = 7;

  // 6 12
  fonts_[32]->verticesBaseData->local_vertices_ = new Vector2D<float>[12];
  fonts_[32]->verticesBaseData->world_vertices_ = new Vector2D<float>[12];

  fonts_[32]->verticesBaseData->number_of_vertices_ = 12;

  fonts_[32]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[32]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[32]->verticesBaseData->local_vertices_[1].x = 5;
  fonts_[32]->verticesBaseData->local_vertices_[1].y = -7;

  fonts_[32]->verticesBaseData->local_vertices_[2].x = 5;
  fonts_[32]->verticesBaseData->local_vertices_[2].y = -7;

  fonts_[32]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[32]->verticesBaseData->local_vertices_[3].y = 2;

  fonts_[32]->verticesBaseData->local_vertices_[4].x = 5;
  fonts_[32]->verticesBaseData->local_vertices_[4].y = 2;

  fonts_[32]->verticesBaseData->local_vertices_[5].x = -5;
  fonts_[32]->verticesBaseData->local_vertices_[5].y = 2;

  fonts_[32]->verticesBaseData->local_vertices_[6].x = -5;
  fonts_[32]->verticesBaseData->local_vertices_[6].y = -7;

  fonts_[32]->verticesBaseData->local_vertices_[7].x = -5;
  fonts_[32]->verticesBaseData->local_vertices_[7].y = 2;

  fonts_[32]->verticesBaseData->local_vertices_[8].x = -5;
  fonts_[32]->verticesBaseData->local_vertices_[8].y = 2;

  fonts_[32]->verticesBaseData->local_vertices_[9].x = -5;
  fonts_[32]->verticesBaseData->local_vertices_[9].y = 7;

  fonts_[32]->verticesBaseData->local_vertices_[10].x = -5;
  fonts_[32]->verticesBaseData->local_vertices_[10].y = 7;

  fonts_[32]->verticesBaseData->local_vertices_[11].x = 5;
  fonts_[32]->verticesBaseData->local_vertices_[11].y = 7;

  // 7 4
  fonts_[33]->verticesBaseData->local_vertices_ = new Vector2D<float>[4];
  fonts_[33]->verticesBaseData->world_vertices_ = new Vector2D<float>[4];

  fonts_[33]->verticesBaseData->number_of_vertices_ = 4;

  fonts_[33]->verticesBaseData->local_vertices_[0].x = -2;
  fonts_[33]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[33]->verticesBaseData->local_vertices_[1].x = 5;
  fonts_[33]->verticesBaseData->local_vertices_[1].y = 7;

  fonts_[33]->verticesBaseData->local_vertices_[2].x = -5;
  fonts_[33]->verticesBaseData->local_vertices_[2].y = 7;

  fonts_[33]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[33]->verticesBaseData->local_vertices_[3].y = 7;

  // 8 14
  fonts_[34]->verticesBaseData->local_vertices_ = new Vector2D<float>[14];
  fonts_[34]->verticesBaseData->world_vertices_ = new Vector2D<float>[14];

  fonts_[34]->verticesBaseData->number_of_vertices_ = 14;

  fonts_[34]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[34]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[34]->verticesBaseData->local_vertices_[1].x = 5;
  fonts_[34]->verticesBaseData->local_vertices_[1].y = -7;

  fonts_[34]->verticesBaseData->local_vertices_[2].x = 5;
  fonts_[34]->verticesBaseData->local_vertices_[2].y = -5;

  fonts_[34]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[34]->verticesBaseData->local_vertices_[3].y = 0;

  fonts_[34]->verticesBaseData->local_vertices_[4].x = 5;
  fonts_[34]->verticesBaseData->local_vertices_[4].y = 0;

  fonts_[34]->verticesBaseData->local_vertices_[5].x = -5;
  fonts_[34]->verticesBaseData->local_vertices_[5].y = 0;

  fonts_[34]->verticesBaseData->local_vertices_[6].x = -5;
  fonts_[34]->verticesBaseData->local_vertices_[6].y = -7;

  fonts_[34]->verticesBaseData->local_vertices_[7].x = -5;
  fonts_[34]->verticesBaseData->local_vertices_[7].y = 0;

  fonts_[34]->verticesBaseData->local_vertices_[8].x = -5;
  fonts_[34]->verticesBaseData->local_vertices_[8].y = 0;

  fonts_[34]->verticesBaseData->local_vertices_[9].x = -5;
  fonts_[34]->verticesBaseData->local_vertices_[9].y = 7;

  fonts_[34]->verticesBaseData->local_vertices_[10].x = -5;
  fonts_[34]->verticesBaseData->local_vertices_[10].y = 7;

  fonts_[34]->verticesBaseData->local_vertices_[11].x = 5;
  fonts_[34]->verticesBaseData->local_vertices_[11].y = 7;

  fonts_[34]->verticesBaseData->local_vertices_[12].x = 5;
  fonts_[34]->verticesBaseData->local_vertices_[12].y = 7;

  fonts_[34]->verticesBaseData->local_vertices_[13].x = 5;
  fonts_[34]->verticesBaseData->local_vertices_[13].y = 0;

  // 9 12
  fonts_[35]->verticesBaseData->local_vertices_ = new Vector2D<float>[12];
  fonts_[35]->verticesBaseData->world_vertices_ = new Vector2D<float>[12];

  fonts_[35]->verticesBaseData->number_of_vertices_ = 12;

  fonts_[35]->verticesBaseData->local_vertices_[0].x = -5;
  fonts_[35]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[35]->verticesBaseData->local_vertices_[1].x = 5;
  fonts_[35]->verticesBaseData->local_vertices_[1].y = -7;

  fonts_[35]->verticesBaseData->local_vertices_[2].x = 5;
  fonts_[35]->verticesBaseData->local_vertices_[2].y = -7;

  fonts_[35]->verticesBaseData->local_vertices_[3].x = 5;
  fonts_[35]->verticesBaseData->local_vertices_[3].y = 0;

  fonts_[35]->verticesBaseData->local_vertices_[4].x = 5;
  fonts_[35]->verticesBaseData->local_vertices_[4].y = 0;

  fonts_[35]->verticesBaseData->local_vertices_[5].x = -5;
  fonts_[35]->verticesBaseData->local_vertices_[5].y = 0;

  fonts_[35]->verticesBaseData->local_vertices_[6].x = -5;
  fonts_[35]->verticesBaseData->local_vertices_[6].y = 0;

  fonts_[35]->verticesBaseData->local_vertices_[7].x = -5;
  fonts_[35]->verticesBaseData->local_vertices_[7].y = 7;

  fonts_[35]->verticesBaseData->local_vertices_[8].x = -5;
  fonts_[35]->verticesBaseData->local_vertices_[8].y = 7;

  fonts_[35]->verticesBaseData->local_vertices_[9].x = 5;
  fonts_[35]->verticesBaseData->local_vertices_[9].y = 7;

  fonts_[35]->verticesBaseData->local_vertices_[10].x = 5;
  fonts_[35]->verticesBaseData->local_vertices_[10].y = 7;

  fonts_[35]->verticesBaseData->local_vertices_[11].x = 5;
  fonts_[35]->verticesBaseData->local_vertices_[11].y = 0;

  // SPACE
  fonts_[36]->verticesBaseData->local_vertices_ = new Vector2D<float>[2];
  fonts_[36]->verticesBaseData->world_vertices_ = new Vector2D<float>[2];

  fonts_[36]->verticesBaseData->number_of_vertices_ = 2;

  fonts_[36]->verticesBaseData->local_vertices_[0].x = -1;
  fonts_[36]->verticesBaseData->local_vertices_[0].y = -7;

  fonts_[36]->verticesBaseData->local_vertices_[1].x = 1;
  fonts_[36]->verticesBaseData->local_vertices_[1].y = -7;
}