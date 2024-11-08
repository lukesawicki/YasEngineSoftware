//
// Created by lukesawicki on 9/21/24.
//

#include "pixels_table.hpp"
#include <SDL.h>
#include "renderer.hpp"
#include "vector_4d.hpp"

PixelsTable::PixelsTable(int width, int height,
                         const Vector4D<Uint8> &default_color) {
  window_dimensions_.x_ = width;
  window_dimensions_.y_ = height;
  pixels_ = new Uint8[window_dimensions_.x_ * window_dimensions_.y_ *
                      kNumberOfColors];
  ClearColor(default_color);
}

PixelsTable::PixelsTable(const PixelsTable &original_pixels_table,
                         const Vector4D<Uint8> &default_color) {
  window_dimensions_.x_ = original_pixels_table.window_dimensions_.x_;
  window_dimensions_.y_ = original_pixels_table.window_dimensions_.y_;
  pixels_ =
      new Uint8[original_pixels_table.window_dimensions_.x_ *
                original_pixels_table.window_dimensions_.y_ * kNumberOfColors];
  for (int i = 0;
       i < window_dimensions_.x_ * window_dimensions_.y_ * kNumberOfColors;
       i++) {
    pixels_[i] = original_pixels_table.pixels_[i];
  }
}

PixelsTable::~PixelsTable() { delete[] pixels_; }

void PixelsTable::ClearColor(const Vector4D<Uint8> &drawing_color) {
  for (int y = 0; y < window_dimensions_.y_; y++) {
    for (int x = 0; x < window_dimensions_.x_; x++) {
      pixels_[kNumberOfColors * (y * window_dimensions_.x_ + x) +
              kRedPosition] = drawing_color.x_;
      pixels_[kNumberOfColors * (y * window_dimensions_.x_ + x) +
              kGreenPosition] = drawing_color.y_;
      pixels_[kNumberOfColors * (y * window_dimensions_.x_ + x) +
              kBluePosition] = drawing_color.z_;
      pixels_[kNumberOfColors * (y * window_dimensions_.x_ + x) +
              kAlphaPosition] = drawing_color.w_;
    }
  }
}

void PixelsTable::DrawPoint(int x, int y,
                            const Vector4D<Uint8> &drawing_color) {
  CartesianPositionToWindow(x, y);
  if (x >= 0 && x < window_dimensions_.x_ && y >= 0 &&
      y < window_dimensions_.y_) {
    pixels_[kNumberOfColors * (y * window_dimensions_.x_ + x) + kRedPosition] =
        drawing_color.x_;
    pixels_[kNumberOfColors * (y * window_dimensions_.x_ + x) +
            kGreenPosition] = drawing_color.y_;
    pixels_[kNumberOfColors * (y * window_dimensions_.x_ + x) + kBluePosition] =
        drawing_color.z_;
    pixels_[kNumberOfColors * (y * window_dimensions_.x_ + x) +
            kAlphaPosition] = drawing_color.w_;
  }
}

void PixelsTable::DrawPointV1(int x, int y,
                            const Vector4D<Uint8> &drawing_color) {
  if (x >= 0 && x < window_dimensions_.x_ && y >= 0 &&
      y < window_dimensions_.y_) {
    pixels_[kNumberOfColors * (y * window_dimensions_.x_ + x) + kRedPosition] =
        drawing_color.x_;
    pixels_[kNumberOfColors * (y * window_dimensions_.x_ + x) +
            kGreenPosition] = drawing_color.y_;
    pixels_[kNumberOfColors * (y * window_dimensions_.x_ + x) + kBluePosition] =
        drawing_color.z_;
    pixels_[kNumberOfColors * (y * window_dimensions_.x_ + x) +
            kAlphaPosition] = drawing_color.w_;
      }
}

void PixelsTable::CartesianPositionToWindow(int &x, int &y) {
  x = x + static_cast<int>(0.5F * window_dimensions_.x_);
  y = -y + static_cast<int>(0.5F * window_dimensions_.y_);
}
