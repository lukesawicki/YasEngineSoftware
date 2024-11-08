//
// Created by lukesawicki on 9/21/24.
//

#include <bitset>
#include "renderer.hpp"

void DrawLine(const Vector2D<float> &point_0, const Vector2D<float> &point_1,
              PixelsTable &pixels_table, const Vector4D<Uint8> &drawing_color) {
  int x_0 = static_cast<int>(point_0.x_);
  int y_0 = static_cast<int>(point_0.y_);

  int original_point_0_x = static_cast<int>(point_0.x_);
  int original_point_0_y = static_cast<int>(point_0.y_);

  int original_point_1_x = static_cast<int>(point_1.x_);
  int original_point_1_y = static_cast<int>(point_1.y_);

  Vector2D<int> copy_point_0(static_cast<int>(point_0.x_),
                             static_cast<int>(point_0.y_));
  Vector2D<int> copy_point_1(static_cast<int>(point_1.x_),
                             static_cast<int>(point_1.y_));

  int delta_x = static_cast<int>(point_1.x_ - point_0.x_);
  int delta_y = static_cast<int>(point_1.y_ - point_0.y_);
  int cumulative_error = 0;

  if (abs(delta_x) != abs(delta_y)) {
    // START GENTLE LINE IF
    if (abs(delta_x) > abs(delta_y)) {
      // DELTAS CONDITION DX > DY
      if (delta_x < 0) {
        // DELTA X < 0 CONDITION (IT MEANS WRONG ORDER)
        original_point_0_x = static_cast<int>(point_1.x_);
        original_point_1_x = static_cast<int>(point_0.x_);

        // switch x_ for drawing
        x_0 = static_cast<int>(point_1.x_);
        y_0 = static_cast<int>(point_1.y_);

        // NEGATIVE SLOPE)
        if (delta_y > 0) {
          // && (DELTAS CONDITION DX > DY) && (DELTA X < 0 CONDITION) -> IT
          // MEANS OCTAN 3(NEGATIVE SLOPE, POINTS IN "WRONG ORDER")

          delta_x = static_cast<int>(point_0.x_ - point_1.x_);
          delta_y = static_cast<int>(point_0.y_ - point_1.y_);
          for (int i = original_point_0_x; i <= original_point_1_x; i++) {
            pixels_table.DrawPoint(x_0, y_0, drawing_color);
            x_0++;
            if ((2 * (cumulative_error + delta_y)) > -delta_x) {
              // y_ stays the same
              cumulative_error = cumulative_error + delta_y;
            } else {
              y_0--;
              cumulative_error = cumulative_error + delta_y + delta_x;
            }
          }
        } else {
          // POSITIVE SLOPE // (deltaY < 0) && (DELTAS CONDITION DX > DY) &&
          // (DELTA X < 0 CONDITION) -> IT MEANS OCTAN 4(POSITIVE SLOPE, POINTS
          // IN "WRONG ORDER")

          if (delta_x != 0) {
            delta_x = static_cast<int>(point_0.x_ - point_1.x_);
            delta_y = static_cast<int>(point_0.y_ - point_1.y_);
            for (int i = original_point_0_x; i <= original_point_1_x; i++) {
              pixels_table.DrawPoint(x_0, y_0, drawing_color);
              x_0++;
              if ((2 * (cumulative_error + delta_y)) < delta_x) {
                // y_ stays the same
                cumulative_error = cumulative_error + delta_y;
              } else {
                y_0++;
                cumulative_error = cumulative_error + delta_y - delta_x;
              }
            }
          }
        }
      } else {
        // DELTA X > 0 CONDITION  (IT MEANS CORRECT ORDER)
        if (delta_x != 0) {
          // POSITIVE SLOPE
          if (delta_y > 0) {
            // && (DELTAS CONDITION DX > DY) && (DELTA X > 0 CONDITION) -> IT
            // MEANS OCTAN 0(POSITIVE SLOPE, POINTS IN "CORRECT ORDER")
            for (int i = original_point_0_x; i <= original_point_1_x; i++) {
              pixels_table.DrawPoint(x_0, y_0, drawing_color);
              x_0++;
              if ((2 * (cumulative_error + delta_y)) < delta_x) {
                // y_ stays the same
                cumulative_error = cumulative_error + delta_y;
              } else {
                y_0++;
                cumulative_error = cumulative_error + delta_y - delta_x;
              }
            }
          } else {
            // NEGATIVE SLOPE // (deltaY < 0) && (DELTAS CONDITION DX > DY) &&
            // (DELTA X > 0 CONDITION) -> IT MEANS OCTAN 7(NEGATIVE SLOPE,
            // POINTS IN "CORRECT ORDER")
            for (int i = original_point_0_x; i <= original_point_1_x; i++) {
              pixels_table.DrawPoint(x_0, y_0, drawing_color);
              x_0++;
              if ((2 * (cumulative_error + delta_y)) > -delta_x) {
                // y_ stays the same
                cumulative_error = cumulative_error + delta_y;
              } else {
                y_0--;
                cumulative_error = cumulative_error + delta_y + delta_x;
              }
            }
          }
        } else {
          // if (deltaX == 0) // It is straight line where x_ is constant. So
          // draw simple line from y0 to y1
          if (copy_point_0.y_ > copy_point_1.y_) {
            SwapVectors(copy_point_0, copy_point_1);
          }
          for (int i = copy_point_0.y_; i <= copy_point_1.y_; i++) {
            pixels_table.DrawPoint(copy_point_0.x_, i, drawing_color);
          }
        }
      }
    } // END GENTLE LINE IF
    else {
      // abs(deltaX) < abs(deltaY) // DELTAS CONDITION DX < DY  // STEEP SLOPE
      if (delta_y < 0) {
        // DELTA Y < 0 CONDITION (IT MEANS WRONG ORDER (BECAUSE IN HERE Y IS
        // LEADING AXIES)
        original_point_0_y = static_cast<int>(point_1.y_);
        original_point_1_y = static_cast<int>(point_0.y_);

        // switch x_ for drawing
        x_0 = static_cast<int>(point_1.x_);
        y_0 = static_cast<int>(point_1.y_);

        // NEGATIVE SLOPE
        if (delta_x > 0) {
          // && (DELTAS CONDITION DX < DY) && (DELTA Y < 0 CONDITION) IT MEANS
          // OCTAN 6(NEGATIVE SLOPE, POINTS IN "WRONG ORDER")
          delta_x = static_cast<int>(point_0.x_ - point_1.x_);
          delta_y = static_cast<int>(point_0.y_ - point_1.y_);
          for (int i = original_point_0_y; i <= original_point_1_y; i++) {
            pixels_table.DrawPoint(x_0, y_0, drawing_color);
            y_0++;
            if ((2 * (cumulative_error + delta_x)) > -delta_y) {
              // y_ stays the same
              cumulative_error = cumulative_error + delta_x;
            } else {
              x_0--;
              cumulative_error = cumulative_error + delta_x + delta_y;
            }
          }
        } else {
          // POSITIVE SLOPE  // deltaX < 0 && (DELTAS CONDITION DX < DY) &&
          // (DELTA Y < 0 CONDITION) IT MEANS OCTAN 5(POSITIVE SLOPE, POINTS IN
          // "WRONG ORDER")

          delta_x = static_cast<int>(point_0.x_ - point_1.x_);
          delta_y = static_cast<int>(point_0.y_ - point_1.y_);
          for (int i = original_point_0_y; i <= original_point_1_y; i++) {
            pixels_table.DrawPoint(x_0, y_0, drawing_color);
            y_0++;
            if ((2 * (cumulative_error + delta_x)) < delta_y) {
              // y_ stays the same
              cumulative_error = cumulative_error + delta_x;
            } else {
              x_0++;
              cumulative_error = cumulative_error + delta_x - delta_y;
            }
          }
        }
      } else {
        // DELTA Y > 0 CONDITION  (IT MEANS CORRECT ORDER)
        if (delta_y != 0) {
          // POSITIVE SLOPE
          if (delta_x > 0) {
            // && (DELTAS CONDITION DX < DY) && (DELTA Y > 0 CONDITION) -> IT
            // MEANS OCTAN 1(POSITIVE SLOPE, POINT IN "CORRECT ORDER")

            for (int i = original_point_0_y; i <= original_point_1_y; i++) {
              pixels_table.DrawPoint(x_0, y_0, drawing_color);
              y_0++;
              if ((2 * (cumulative_error + delta_x)) < delta_y) {
                // y_ stays the same
                cumulative_error = cumulative_error + delta_x;
              } else {
                x_0++;
                cumulative_error = cumulative_error + delta_x - delta_y;
              }
            }
          } else {
            // NEGATIVE SLOPE // (deltaX < 0) && (DELTAS CONDITION DX < DY) &&
            // (DELTA Y > 0 CONDITION) -> IT MEANS OCTAN 2(NEGATIVE SLOPE POINTS
            // IN "CORRECT ORDER")
            for (int i = original_point_0_y; i <= original_point_1_y; i++) {
              pixels_table.DrawPoint(x_0, y_0, drawing_color);
              y_0++;
              if ((2 * (cumulative_error + delta_x)) > -delta_y) {
                // y_ stays the same
                cumulative_error = cumulative_error + delta_x;
              } else {
                x_0--;
                cumulative_error = cumulative_error + delta_x + delta_y;
              }
            }
          }
        } else {
          // deltaY == 0 It is straight line where y_ is constant. So draw
          // simple line from x0 to x1
          if (copy_point_0.x_ > copy_point_1.x_) {
            SwapVectors(copy_point_0, copy_point_1);
          }
          for (int i = copy_point_0.x_; i <= copy_point_1.x_; i++) {
            pixels_table.DrawPoint(i, copy_point_0.y_, drawing_color);
          }
        }
      }
    }
  } else {
    // deltaX is equals deltaY
    if (delta_x == 0 && delta_y == 0) {
      // if both are equals 0 just draw point.

      pixels_table.DrawPoint(x_0, y_0, drawing_color);
    } else {
      int absolute_value_of_delta_x = abs(copy_point_1.x_ - copy_point_0.x_);
      // Positive line
      int i = 0;
      if (copy_point_0.x_ < copy_point_1.x_ && copy_point_0.y_ < copy_point_1.
          y_) {
        while (i < absolute_value_of_delta_x) {
          pixels_table.DrawPoint(copy_point_0.x_ + i, copy_point_0.y_ + i,
                                 drawing_color);
          i++;
        }
      }
      if (copy_point_1.x_ < copy_point_0.x_ && copy_point_0.y_ > copy_point_1.
          y_) {
        while (i < absolute_value_of_delta_x) {
          pixels_table.DrawPoint(copy_point_1.x_ + i, copy_point_1.y_ + i,
                                 drawing_color);
          i++;
        }
      }

      // Negative line
      if (copy_point_0.x_ < copy_point_1.x_ && copy_point_0.y_ > copy_point_1.
          y_) {
        while (i < absolute_value_of_delta_x) {
          pixels_table.DrawPoint(copy_point_0.x_ + i, copy_point_0.y_ - i,
                                 drawing_color);
          i++;
        }
      }
      if (copy_point_1.x_ < copy_point_0.x_ && copy_point_1.y_ > copy_point_0.
          y_) {
        while (i < absolute_value_of_delta_x) {
          pixels_table.DrawPoint(copy_point_1.x_ + i, copy_point_1.y_ - i,
                                 drawing_color);
          i++;
        }
      }
    }
  }
}

void DrawLineV1(const Vector2D<float> &point_0, const Vector2D<float> &point_1,
              PixelsTable &pixels_table, const Vector4D<Uint8> &drawing_color) {
  int x_0 = static_cast<int>(point_0.x_);
  int y_0 = static_cast<int>(point_0.y_);

  int original_point_0_x = static_cast<int>(point_0.x_);
  int original_point_0_y = static_cast<int>(point_0.y_);

  int original_point_1_x = static_cast<int>(point_1.x_);
  int original_point_1_y = static_cast<int>(point_1.y_);

  Vector2D<int> copy_point_0(static_cast<int>(point_0.x_),
                             static_cast<int>(point_0.y_));
  Vector2D<int> copy_point_1(static_cast<int>(point_1.x_),
                             static_cast<int>(point_1.y_));

  int delta_x = static_cast<int>(point_1.x_ - point_0.x_);
  int delta_y = static_cast<int>(point_1.y_ - point_0.y_);
  int cumulative_error = 0;

  if (abs(delta_x) != abs(delta_y)) {
    // START GENTLE LINE IF
    if (abs(delta_x) > abs(delta_y)) {
      // DELTAS CONDITION DX > DY
      if (delta_x < 0) {
        // DELTA X < 0 CONDITION (IT MEANS WRONG ORDER)
        original_point_0_x = static_cast<int>(point_1.x_);
        original_point_1_x = static_cast<int>(point_0.x_);

        // switch x_ for drawing
        x_0 = static_cast<int>(point_1.x_);
        y_0 = static_cast<int>(point_1.y_);

        // NEGATIVE SLOPE)
        if (delta_y > 0) {
          // && (DELTAS CONDITION DX > DY) && (DELTA X < 0 CONDITION) -> IT
          // MEANS OCTAN 3(NEGATIVE SLOPE, POINTS IN "WRONG ORDER")

          delta_x = static_cast<int>(point_0.x_ - point_1.x_);
          delta_y = static_cast<int>(point_0.y_ - point_1.y_);
          for (int i = original_point_0_x; i <= original_point_1_x; i++) {
            pixels_table.DrawPointV1(x_0, y_0, drawing_color);
            x_0++;
            if ((2 * (cumulative_error + delta_y)) > -delta_x) {
              // y_ stays the same
              cumulative_error = cumulative_error + delta_y;
            } else {
              y_0--;
              cumulative_error = cumulative_error + delta_y + delta_x;
            }
          }
        } else {
          // POSITIVE SLOPE // (deltaY < 0) && (DELTAS CONDITION DX > DY) &&
          // (DELTA X < 0 CONDITION) -> IT MEANS OCTAN 4(POSITIVE SLOPE, POINTS
          // IN "WRONG ORDER")

          if (delta_x != 0) {
            delta_x = static_cast<int>(point_0.x_ - point_1.x_);
            delta_y = static_cast<int>(point_0.y_ - point_1.y_);
            for (int i = original_point_0_x; i <= original_point_1_x; i++) {
              pixels_table.DrawPointV1(x_0, y_0, drawing_color);
              x_0++;
              if ((2 * (cumulative_error + delta_y)) < delta_x) {
                // y_ stays the same
                cumulative_error = cumulative_error + delta_y;
              } else {
                y_0++;
                cumulative_error = cumulative_error + delta_y - delta_x;
              }
            }
          }
        }
      } else {
        // DELTA X > 0 CONDITION  (IT MEANS CORRECT ORDER)
        if (delta_x != 0) {
          // POSITIVE SLOPE
          if (delta_y > 0) {
            // && (DELTAS CONDITION DX > DY) && (DELTA X > 0 CONDITION) -> IT
            // MEANS OCTAN 0(POSITIVE SLOPE, POINTS IN "CORRECT ORDER")
            for (int i = original_point_0_x; i <= original_point_1_x; i++) {
              pixels_table.DrawPointV1(x_0, y_0, drawing_color);
              x_0++;
              if ((2 * (cumulative_error + delta_y)) < delta_x) {
                // y_ stays the same
                cumulative_error = cumulative_error + delta_y;
              } else {
                y_0++;
                cumulative_error = cumulative_error + delta_y - delta_x;
              }
            }
          } else {
            // NEGATIVE SLOPE // (deltaY < 0) && (DELTAS CONDITION DX > DY) &&
            // (DELTA X > 0 CONDITION) -> IT MEANS OCTAN 7(NEGATIVE SLOPE,
            // POINTS IN "CORRECT ORDER")
            for (int i = original_point_0_x; i <= original_point_1_x; i++) {
              pixels_table.DrawPointV1(x_0, y_0, drawing_color);
              x_0++;
              if ((2 * (cumulative_error + delta_y)) > -delta_x) {
                // y_ stays the same
                cumulative_error = cumulative_error + delta_y;
              } else {
                y_0--;
                cumulative_error = cumulative_error + delta_y + delta_x;
              }
            }
          }
        } else {
          // if (deltaX == 0) // It is straight line where x_ is constant. So
          // draw simple line from y0 to y1
          if (copy_point_0.y_ > copy_point_1.y_) {
            SwapVectors(copy_point_0, copy_point_1);
          }
          for (int i = copy_point_0.y_; i <= copy_point_1.y_; i++) {
            pixels_table.DrawPointV1(copy_point_0.x_, i, drawing_color);
          }
        }
      }
    } // END GENTLE LINE IF
    else {
      // abs(deltaX) < abs(deltaY) // DELTAS CONDITION DX < DY  // STEEP SLOPE
      if (delta_y < 0) {
        // DELTA Y < 0 CONDITION (IT MEANS WRONG ORDER (BECAUSE IN HERE Y IS
        // LEADING AXIES)
        original_point_0_y = static_cast<int>(point_1.y_);
        original_point_1_y = static_cast<int>(point_0.y_);

        // switch x_ for drawing
        x_0 = static_cast<int>(point_1.x_);
        y_0 = static_cast<int>(point_1.y_);

        // NEGATIVE SLOPE
        if (delta_x > 0) {
          // && (DELTAS CONDITION DX < DY) && (DELTA Y < 0 CONDITION) IT MEANS
          // OCTAN 6(NEGATIVE SLOPE, POINTS IN "WRONG ORDER")
          delta_x = static_cast<int>(point_0.x_ - point_1.x_);
          delta_y = static_cast<int>(point_0.y_ - point_1.y_);
          for (int i = original_point_0_y; i <= original_point_1_y; i++) {
            pixels_table.DrawPointV1(x_0, y_0, drawing_color);
            y_0++;
            if ((2 * (cumulative_error + delta_x)) > -delta_y) {
              // y_ stays the same
              cumulative_error = cumulative_error + delta_x;
            } else {
              x_0--;
              cumulative_error = cumulative_error + delta_x + delta_y;
            }
          }
        } else {
          // POSITIVE SLOPE  // deltaX < 0 && (DELTAS CONDITION DX < DY) &&
          // (DELTA Y < 0 CONDITION) IT MEANS OCTAN 5(POSITIVE SLOPE, POINTS IN
          // "WRONG ORDER")

          delta_x = static_cast<int>(point_0.x_ - point_1.x_);
          delta_y = static_cast<int>(point_0.y_ - point_1.y_);
          for (int i = original_point_0_y; i <= original_point_1_y; i++) {
            pixels_table.DrawPointV1(x_0, y_0, drawing_color);
            y_0++;
            if ((2 * (cumulative_error + delta_x)) < delta_y) {
              // y_ stays the same
              cumulative_error = cumulative_error + delta_x;
            } else {
              x_0++;
              cumulative_error = cumulative_error + delta_x - delta_y;
            }
          }
        }
      } else {
        // DELTA Y > 0 CONDITION  (IT MEANS CORRECT ORDER)
        if (delta_y != 0) {
          // POSITIVE SLOPE
          if (delta_x > 0) {
            // && (DELTAS CONDITION DX < DY) && (DELTA Y > 0 CONDITION) -> IT
            // MEANS OCTAN 1(POSITIVE SLOPE, POINT IN "CORRECT ORDER")

            for (int i = original_point_0_y; i <= original_point_1_y; i++) {
              pixels_table.DrawPointV1(x_0, y_0, drawing_color);
              y_0++;
              if ((2 * (cumulative_error + delta_x)) < delta_y) {
                // y_ stays the same
                cumulative_error = cumulative_error + delta_x;
              } else {
                x_0++;
                cumulative_error = cumulative_error + delta_x - delta_y;
              }
            }
          } else {
            // NEGATIVE SLOPE // (deltaX < 0) && (DELTAS CONDITION DX < DY) &&
            // (DELTA Y > 0 CONDITION) -> IT MEANS OCTAN 2(NEGATIVE SLOPE POINTS
            // IN "CORRECT ORDER")
            for (int i = original_point_0_y; i <= original_point_1_y; i++) {
              pixels_table.DrawPointV1(x_0, y_0, drawing_color);
              y_0++;
              if ((2 * (cumulative_error + delta_x)) > -delta_y) {
                // y_ stays the same
                cumulative_error = cumulative_error + delta_x;
              } else {
                x_0--;
                cumulative_error = cumulative_error + delta_x + delta_y;
              }
            }
          }
        } else {
          // deltaY == 0 It is straight line where y_ is constant. So draw
          // simple line from x0 to x1
          if (copy_point_0.x_ > copy_point_1.x_) {
            SwapVectors(copy_point_0, copy_point_1);
          }
          for (int i = copy_point_0.x_; i <= copy_point_1.x_; i++) {
            pixels_table.DrawPointV1(i, copy_point_0.y_, drawing_color);
          }
        }
      }
    }
  } else {
    // deltaX is equals deltaY
    if (delta_x == 0 && delta_y == 0) {
      // if both are equals 0 just draw point.

      pixels_table.DrawPointV1(x_0, y_0, drawing_color);
    } else {
      int absolute_value_of_delta_x = abs(copy_point_1.x_ - copy_point_0.x_);
      // Positive line
      int i = 0;
      if (copy_point_0.x_ < copy_point_1.x_ && copy_point_0.y_ < copy_point_1.
          y_) {
        while (i < absolute_value_of_delta_x) {
          pixels_table.DrawPointV1(copy_point_0.x_ + i, copy_point_0.y_ + i,
                                 drawing_color);
          i++;
        }
      }
      if (copy_point_1.x_ < copy_point_0.x_ && copy_point_0.y_ > copy_point_1.
          y_) {
        while (i < absolute_value_of_delta_x) {
          pixels_table.DrawPointV1(copy_point_1.x_ + i, copy_point_1.y_ + i,
                                 drawing_color);
          i++;
        }
      }

      // Negative line
      if (copy_point_0.x_ < copy_point_1.x_ && copy_point_0.y_ > copy_point_1.
          y_) {
        while (i < absolute_value_of_delta_x) {
          pixels_table.DrawPointV1(copy_point_0.x_ + i, copy_point_0.y_ - i,
                                 drawing_color);
          i++;
        }
      }
      if (copy_point_1.x_ < copy_point_0.x_ && copy_point_1.y_ > copy_point_0.
          y_) {
        while (i < absolute_value_of_delta_x) {
          pixels_table.DrawPointV1(copy_point_1.x_ + i, copy_point_1.y_ - i,
                                 drawing_color);
          i++;
        }
      }
    }
  }
}

void DrawLineStrip(Vector2D<float> *vertices,
                   int maximum_number_of_vertices,
                   const Vector4D<Uint8> &color,
                   bool are_lines_continuous,
                   PixelsTable &pixels_table) {
  int step = 1;
  if (!are_lines_continuous) {
    step = 2;
  }
  if (maximum_number_of_vertices > 1) {
    if (maximum_number_of_vertices <= 3) {
      DrawLine(vertices[0], vertices[1], pixels_table, color);
    } else {
      for (int i = 0; i < maximum_number_of_vertices - 1; i += step) {
        DrawLine(vertices[i], vertices[i + 1], pixels_table, color);
      }
    }
  }
}

void SwapVectors(Vector2D<int> &point_0, Vector2D<int> &point_1) {
  int temporary_x = point_0.x_;
  int temporary_y = point_0.y_;
  point_0.x_ = point_1.x_;
  point_0.y_ = point_1.y_;
  point_1.x_ = temporary_x;
  point_1.y_ = temporary_y;
}

void SwapVectors(Vector2D<float> &point0, Vector2D<float> &point1) {
  float temporary_x = point0.x_;
  float temporary_y = point0.y_;
  point0.x_ = point1.x_;
  point0.y_ = point1.y_;
  point1.x_ = temporary_x;
  point1.y_ = temporary_y;
}

void DrawCartesianAxes(PixelsTable &pixels_table) {
  HorizontalLineOnWholeScreen(pixels_table, 0, kRed);
  VerticalLineOnWholeScreen(pixels_table, 0, kGreen);
}

void DrawCrossHair(float x, float y, PixelsTable &pixels_table,
                   bool is_full_screen, Vector4D<Uint8> color) {
  if (is_full_screen) {
    HorizontalLineOnWholeScreen(pixels_table, static_cast<int>(y), kBlue);
    VerticalLineOnWholeScreen(pixels_table, static_cast<int>(x), kBlue);
  } else {
    // TODO refactor methods drawHor... and drawVer... should have float
    // arguments and then round and cast
    DrawHorizontalLine(pixels_table, static_cast<int>(round(x) - 15),
                       static_cast<int>(round(x) - 5),
                       static_cast<int>(round(y)), color);
    DrawHorizontalLine(pixels_table, static_cast<int>(round(x) + 5),
                       static_cast<int>(round(x) + 15),
                       static_cast<int>(round(y)), color);

    DrawVerticalLine(pixels_table, static_cast<int>(round(y) - 15),
                     static_cast<int>(round(y) - 5), static_cast<int>(round(x)),
                     color);
    DrawVerticalLine(pixels_table, static_cast<int>(round(y) + 5),
                     static_cast<int>(round(y) + 15),
                     static_cast<int>(round(x)), color);
  }
}

void DrawHorizontalLine(PixelsTable &pixels_table, int x_0, int x_1, int y,
                        Vector4D<Uint8> color) {
  for (int i = x_0; i < x_1; i++) {
    pixels_table.DrawPoint(i, y, color);
  }
}

void DrawVerticalLine(PixelsTable &pixels_table, int y_0, int y_1, int x,
                      Vector4D<Uint8> color) {
  if (y_0 > y_1) {
    int temporary = y_0;
    y_0 = y_1;
    y_1 = temporary;
  }
  for (int i = y_0; i < y_1; i++) {
    pixels_table.DrawPoint(x, i, color);
  }
}

void HorizontalLineOnWholeScreen(PixelsTable &pixels_table, int y,
                                 Vector4D<Uint8> color) {
  int max_x = static_cast<int>(0.5F * pixels_table.window_dimensions_.x_);
  for (int i = -max_x; i < max_x; i++) {
    pixels_table.DrawPoint(i, y, color);
  }
}

void VerticalLineOnWholeScreen(PixelsTable &pixels_table, int x,
                               Vector4D<Uint8> color) {
  int max_y = static_cast<int>(0.5F * pixels_table.window_dimensions_.y_);
  for (int i = -max_y; i < max_y; i++) {
    pixels_table.DrawPoint(x, i, color);
  }
}

int ScreenPixelPositionToArrayPosition(int x, int y, int window_width) {
  return y * window_width + x;
}

int ScreenPixelPositionToArrayPosition(Vector2D<int> &point, int window_width) {
  return point.y_ * window_width + point.x_;
}

void WindowPositionToCartesianPosition(float &x, float &y, int window_width,
                                       int window_height) {
  x = x - static_cast<int>(0.5 * window_width);
  y = (-(y - static_cast<int>(0.5 * window_height)));
}
