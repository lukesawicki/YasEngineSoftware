#ifndef YASGRAPHICSLIBRARY_HPP
#define YASGRAPHICSLIBRARY_HPP
#include<cmath>
#include<iostream>
#include<vector>
#include<SDL.h>
#include"YasVector2D.hpp"
#include"YasVector4D.hpp"
#include "PixelsTable.hpp"
#include "YasPolygon.hpp"

class PixelsTable;

const float PI = 3.141592F;
const Uint8 NUMBER_OF_COLORS = 4;
const Uint8 RGB_FORMAT = 3;
const Uint8 RGBA_FORMAT = 4;

const Uint8 RED_POSITION = 0;
const Uint8 GREEN_POSITION = 1;
const Uint8 BLUE_POSITION = 2;
const Uint8 ALPHA_POSITION = 3;

const YasVector4D<Uint8> RED(255, 0, 0, 0);
const YasVector4D<Uint8> GREEN(0, 255, 0, 0);
const YasVector4D<Uint8> BLUE(0, 0, 255, 0);
const YasVector4D<Uint8> BLACK(0, 0, 0, 255);
const YasVector4D<Uint8> WHITE(255, 255, 255, 255);
const YasVector4D<Uint8> YELLOW(255, 255, 0, 255);

void drawLine(const YasVector2D<float>& point0, const YasVector2D<float>& point1, PixelsTable& pixelsTable, const YasVector4D<Uint8>& drawingColor);

void drawPolygon(YasPolygon* polygon, PixelsTable& pixelsTable);

YasVector2D<float>* generateVerticesFromNumbers(const std::vector<int>& numbers);

//void drawPrimeNumbers(const std::vector<int>& primeNumbers, PixelsTable& pixelsTable);
void drawNumbersAsLineSegmentsNotContinuous(YasVector2D<float>* vertices, int maximumNumberOfVertices, PixelsTable& pixelsTable);

void drawNumbersAsPolyline(YasVector2D<float>* vertices, int maximumNumberOfVertices, PixelsTable& pixelsTable);

void drawPolygonDirection(YasPolygon* polygon, PixelsTable& pixelsTable);

void swapVectors(YasVector2D<int>& point0, YasVector2D<int>& point1);

void horizontalLineOnScreen(PixelsTable& pixelsTable, int y, YasVector4D<Uint8> color);

void verticalLineOnScreen(PixelsTable& pixelsTable, int x, YasVector4D<Uint8> color);

void drawCartesianAxies(PixelsTable& pixelsTable);

void drawCrossOnScreen(float x, float y, PixelsTable& pixelsTable);
    
int xyPixelToArrayPosition(YasVector2D<int>& point, int windowWidth);
int xyPixelToArrayPosition(int x, int y, int windowWidth);

//void windowPositionToCartesianPosition(double& x, double& y, YasVector2D<int>* windowDimensions);
void windowPositionToCartesianPosition(float& x, float& y, YasVector2D<int>* windowDimensions);

#endif
