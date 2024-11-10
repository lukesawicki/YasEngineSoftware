//
// Created by lukesawicki on 9/21/24.
//

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <SDL.h>

#include "pixels_table.hpp"
#include "vector_2d.hpp"
#include "vector_4d.hpp"

class PixelsTable;

const Uint8 kNumberOfColors = 4;
const Uint8 kRgbFormat = 3;
const Uint8 kRgbaFormat = 4;

const Uint8 kRedPosition = 0;
const Uint8 kGreenPosition = 1;
const Uint8 kBluePosition = 2;
const Uint8 kAlphaPosition = 3;

const Vector4D<Uint8> kRed(255, 0, 0, 0);
const Vector4D<Uint8> kGreen(0, 255, 0, 0);
const Vector4D<Uint8> kBlue(0, 0, 255, 0);
const Vector4D<Uint8> kBlack(0, 0, 0, 255);
const Vector4D<Uint8> kWhite(255, 255, 255, 0);
const Vector4D<Uint8> kYellow(255, 255, 0, 0);
const Vector4D<Uint8> kLightBlue(0, 255, 255, 0);
const Vector4D<Uint8> kPurple(255, 0, 255, 0);
const Vector4D<Uint8> kXportal(255, 77, 0, 0);
const Vector4D<Uint8> kPolygon(250, 180, 10, 0);

void DrawLine(const Vector2D<float> &point_0, const Vector2D<float> &point_1,
              PixelsTable &pixels_table, const Vector4D<Uint8> &drawing_color);
void DrawLineV1(const Vector2D<float> &point_0, const Vector2D<float> &point_1,
              PixelsTable &pixels_table, const Vector4D<Uint8> &drawing_color);

Vector2D<float> *GenerateVerticesFromNumbers(const std::vector<int> &numbers);

void DrawLineStrip(Vector2D<float> *vertices,
                   int maximum_number_of_vertices,
                   const Vector4D<Uint8> &color,
                   bool are_lines_continuous,
                   PixelsTable &pixels_table);

void SwapVectors(Vector2D<int> &point_0, Vector2D<int> &point_1);

void HorizontalLineOnWholeScreen(PixelsTable &pixels_table, int y,
                                 Vector4D<Uint8> color);

void VerticalLineOnWholeScreen(PixelsTable &pixels_table, int x,
                               Vector4D<Uint8> color);

void DrawCartesianAxes(PixelsTable &pixels_table);

void DrawDefaultCrossHair(float x, float y, PixelsTable &pixels_table,
                          bool is_full_screen);

void DrawCrossHair(float x, float y, PixelsTable &pixels_table,
                   bool is_full_screen, Vector4D<Uint8> color);

void DrawHorizontalLine(PixelsTable &pixels_table, int x_0, int x_1, int y,
                        Vector4D<Uint8> color);

void DrawVerticalLine(PixelsTable &pixels_table, int y_0, int y_1, int x,
                      Vector4D<Uint8> color);

int ScreenPixelPositionToArrayPosition(Vector2D<int> &point, int window_width);

int ScreenPixelPositionToArrayPosition(int x, int y, int window_width);

void WindowPositionToCartesianPosition(float &x, float &y, int window_width,
                                       int window_height);



#endif //RENDERER_HPP
