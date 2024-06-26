#include<bitset>
#include"YasGraphicsLibrary.hpp"

    void drawLine(const Vector2D<float>& point0, const Vector2D<float>& point1, PixelsTable& pixelsTable, const Vector4D<Uint8>& drawingColor)
    {
        int x0 = static_cast<int>(point0.x);
        int y0 = static_cast<int>(point0.y);
    
        int originalPoint0X = static_cast<int>(point0.x);
        int originalPoint0Y = static_cast<int>(point0.y);
    
        int originalPoint1X = static_cast<int>(point1.x);
        int originalPoint1Y = static_cast<int>(point1.y);
    
        Vector2D<int> copyPoint0(static_cast<int>(point0.x), static_cast<int>(point0.y));
        Vector2D<int> copyPoint1(static_cast<int>(point1.x), static_cast<int>(point1.y));
    
        int deltaX = static_cast<int>(point1.x - point0.x);
        int deltaY = static_cast<int>(point1.y - point0.y);
        int cumulativeError = 0;
    
        if (abs(deltaX) != abs(deltaY))
        {
            // START GENTLE LINE IF
            if (abs(deltaX) > abs(deltaY)) // DELTAS CONDITION DX > DY
            {
                if (deltaX < 0) // DELTA X < 0 CONDITION (IT MEANS WRONG ORDER)
                {
                    originalPoint0X = static_cast<int>(point1.x);
                    originalPoint1X = static_cast<int>(point0.x);
    
                    // switch x for drawing
                    x0 = static_cast<int>(point1.x);
                    y0 = static_cast<int>(point1.y);
    
                    // NEGATIVE SLOPE)
                    if (deltaY > 0) // && (DELTAS CONDITION DX > DY) && (DELTA X < 0 CONDITION) -> IT MEANS OCTAN 3(NEGATIVE SLOPE, POINTS IN "WRONG ORDER")
                    {
                        deltaX = static_cast<int>(point0.x - point1.x);
                        deltaY = static_cast<int>(point0.y - point1.y);
                        for (int i = originalPoint0X; i <= originalPoint1X; i++)
                        {
                            pixelsTable.drawPoint(x0, y0, drawingColor);
                            x0++;
                            if ((2 * (cumulativeError + deltaY)) > -deltaX)
                            {
                                //y stays the same
                                cumulativeError = cumulativeError + deltaY;
                            }
                            else
                            {
                                y0--;
                                cumulativeError = cumulativeError + deltaY + deltaX;
                            }
                        }
                    }
                    else // POSITIVE SLOPE // (deltaY < 0) && (DELTAS CONDITION DX > DY) && (DELTA X < 0 CONDITION) -> IT MEANS OCTAN 4(POSITIVE SLOPE, POINTS IN "WRONG ORDER")
                    {
                        if (deltaX != 0)
                        {
                            deltaX = static_cast<int>(point0.x - point1.x);
                            deltaY = static_cast<int>(point0.y - point1.y);
                            for (int i = originalPoint0X; i <= originalPoint1X; i++)
                            {
                                pixelsTable.drawPoint(x0, y0, drawingColor);
                                x0++;
                                if ((2 * (cumulativeError + deltaY)) < deltaX)
                                {
                                    //y stays the same
                                    cumulativeError = cumulativeError + deltaY;
                                }
                                else
                                {
                                    y0++;
                                    cumulativeError = cumulativeError + deltaY - deltaX;
                                }
                            }
                        }
                    }
                }
                else  // DELTA X > 0 CONDITION  (IT MEANS CORRECT ORDER)
                {
    
                    if (deltaX != 0)
                    {
                        // POSITIVE SLOPE
                        if (deltaY > 0)  // && (DELTAS CONDITION DX > DY) && (DELTA X > 0 CONDITION) -> IT MEANS OCTAN 0(POSITIVE SLOPE, POINTS IN "CORRECT ORDER")
                        {
                            for (int i = originalPoint0X; i <= originalPoint1X; i++)
                            {
                                pixelsTable.drawPoint(x0, y0, drawingColor);
                                x0++;
                                if ((2 * (cumulativeError + deltaY)) < deltaX)
                                {
                                    //y stays the same
                                    cumulativeError = cumulativeError + deltaY;
                                }
                                else
                                {
                                    y0++;
                                    cumulativeError = cumulativeError + deltaY - deltaX;
                                }
                            }
                        }
                        else  // NEGATIVE SLOPE // (deltaY < 0) && (DELTAS CONDITION DX > DY) && (DELTA X > 0 CONDITION) -> IT MEANS OCTAN 7(NEGATIVE SLOPE, POINTS IN "CORRECT ORDER")
                        {
                            for (int i = originalPoint0X; i <= originalPoint1X; i++)
                            {
                                pixelsTable.drawPoint(x0, y0, drawingColor);
                                x0++;
                                if ((2 * (cumulativeError + deltaY)) > -deltaX)
                                {
                                    //y stays the same
                                    cumulativeError = cumulativeError + deltaY;
                                }
                                else
                                {
                                    y0--;
                                    cumulativeError = cumulativeError + deltaY + deltaX;
                                }
                            }
                        }
                    }
                    else //if (deltaX == 0) // It is straight line where x is constant. So draw simple line from y0 to y1
                    {						
						if (copyPoint0.y > copyPoint1.y)
						{
							swapVectors(copyPoint0, copyPoint1);
						}
						for (int i = copyPoint0.y; i <= copyPoint1.y; i++)
						{
							pixelsTable.drawPoint(copyPoint0.x, i, drawingColor);
						}
                    }
                }
            } // END GENTLE LINE IF
            else // abs(deltaX) < abs(deltaY) // DELTAS CONDITION DX < DY  // STEEP SLOPE
            {
                if (deltaY < 0) // DELTA Y < 0 CONDITION (IT MEANS WRONG ORDER (BECAUSE IN HERE Y IS LEADING AXIES)
                {
                    originalPoint0Y = static_cast<int>(point1.y);
                    originalPoint1Y = static_cast<int>(point0.y);
    
                    // switch x for drawing
                    x0 = static_cast<int>(point1.x);
                    y0 = static_cast<int>(point1.y);
    
                    // NEGATIVE SLOPE
                    if (deltaX > 0) // && (DELTAS CONDITION DX < DY) && (DELTA Y < 0 CONDITION) IT MEANS OCTAN 6(NEGATIVE SLOPE, POINTS IN "WRONG ORDER")
                    {
                        deltaX = static_cast<int>(point0.x - point1.x);
                        deltaY = static_cast<int>(point0.y - point1.y);
                        for (int i = originalPoint0Y; i <= originalPoint1Y; i++)
                        {
                            pixelsTable.drawPoint(x0, y0, drawingColor);
                            y0++;
                            if ((2 * (cumulativeError + deltaX)) > -deltaY)
                            {
                                //y stays the same
                                cumulativeError = cumulativeError + deltaX;
                            }
                            else
                            {
                                x0--;
                                cumulativeError = cumulativeError + deltaX + deltaY;
                            }
                        }
                    }
                    else // POSITIVE SLOPE  // deltaX < 0 && (DELTAS CONDITION DX < DY) && (DELTA Y < 0 CONDITION) IT MEANS OCTAN 5(POSITIVE SLOPE, POINTS IN "WRONG ORDER")
                    {
                        deltaX = static_cast<int>(point0.x - point1.x);
                        deltaY = static_cast<int>(point0.y - point1.y);
                        for (int i = originalPoint0Y; i <= originalPoint1Y; i++)
                        {
                            pixelsTable.drawPoint(x0, y0, drawingColor);
                            y0++;
                            if ((2 * (cumulativeError + deltaX)) < deltaY)
                            {
                                //y stays the same
                                cumulativeError = cumulativeError + deltaX;
                            }
                            else
                            {
                                x0++;
                                cumulativeError = cumulativeError + deltaX - deltaY;
                            }
                        }
                    }
                }
                else // DELTA Y > 0 CONDITION  (IT MEANS CORRECT ORDER)
                {
                    if (deltaY != 0)
                    {
                        // POSITIVE SLOPE
                        if (deltaX > 0) // && (DELTAS CONDITION DX < DY) && (DELTA Y > 0 CONDITION) -> IT MEANS OCTAN 1(POSITIVE SLOPE, POINT IN "CORRECT ORDER")
                        {
                            for (int i = originalPoint0Y; i <= originalPoint1Y; i++)
                            {
                                pixelsTable.drawPoint(x0, y0, drawingColor);
                                y0++;
                                if ((2 * (cumulativeError + deltaX)) < deltaY)
                                {
                                    //y stays the same
                                    cumulativeError = cumulativeError + deltaX;
                                }
                                else
                                {
                                    x0++;
                                    cumulativeError = cumulativeError + deltaX - deltaY;
                                }
                            }
                        }
                        else // NEGATIVE SLOPE // (deltaX < 0) && (DELTAS CONDITION DX < DY) && (DELTA Y > 0 CONDITION) -> IT MEANS OCTAN 2(NEGATIVE SLOPE POINTS IN "CORRECT ORDER")
                        {
                            for (int i = originalPoint0Y; i <= originalPoint1Y; i++)
                            {
                                pixelsTable.drawPoint(x0, y0, drawingColor);
                                y0++;
                                if ((2 * (cumulativeError + deltaX)) > -deltaY)
                                {
                                    //y stays the same
                                    cumulativeError = cumulativeError + deltaX;
                                }
                                else
                                {
                                    x0--;
                                    cumulativeError = cumulativeError + deltaX + deltaY;
                                }
                            }
                        }
                    }
					else // deltaY == 0 It is straight line where y is constant. So draw simple line from x0 to x1
                    {
                        if (copyPoint0.x > copyPoint1.x)
                        {
                            swapVectors(copyPoint0, copyPoint1);
                        }
                        for (int i = copyPoint0.x; i <= copyPoint1.x; i++)
                        {
                            pixelsTable.drawPoint(i, copyPoint0.y, drawingColor);
                        }
                    }
                }
            }
        }
        else // deltaX is equals deltaY
        {
            if (deltaX == 0 && deltaY == 0) // if both are equals 0 just draw point.
            {
                pixelsTable.drawPoint(x0, y0, drawingColor);
            }
            else
            {
                int absDeltaX = abs(copyPoint1.x - copyPoint0.x);
                // Positive line
                int i = 0;
                if (copyPoint0.x < copyPoint1.x && copyPoint0.y < copyPoint1.y)
                {
                    while (i < absDeltaX)
                    {
                        pixelsTable.drawPoint(copyPoint0.x + i, copyPoint0.y + i, drawingColor);
                        i++;
                    }
                }
                if (copyPoint1.x < copyPoint0.x && copyPoint0.y > copyPoint1.y)
                {
                    while (i < absDeltaX)
                    {
                        pixelsTable.drawPoint(copyPoint1.x + i, copyPoint1.y + i, drawingColor);
                        i++;
                    }
                }
    
                // Negative line
                if (copyPoint0.x < copyPoint1.x && copyPoint0.y > copyPoint1.y)
                {
                    while (i < absDeltaX)
                    {
                        pixelsTable.drawPoint(copyPoint0.x + i, copyPoint0.y - i, drawingColor);
                        i++;
                    }
                }
                if (copyPoint1.x < copyPoint0.x && copyPoint1.y > copyPoint0.y)
                {
                    while (i < absDeltaX)
                    {
                        pixelsTable.drawPoint(copyPoint1.x + i, copyPoint1.y - i, drawingColor);
                        i++;
                    }
                }
            }
        }
    }

    void drawPolygon(GameObject* polygon, PixelsTable& pixelsTable)
    {
        drawLine(polygon->worldVertices[0], polygon->worldVertices[1], pixelsTable, polygon->color);
        for (int i = 0; i < polygon->numberOfVertices; i++)
        {
            if ((i == polygon->numberOfVertices - 1))
            {
                drawLine(polygon->worldVertices[i], polygon->worldVertices[0], pixelsTable, polygon->color);
            }
            else
            {
                drawLine(polygon->worldVertices[i], polygon->worldVertices[i + 1], pixelsTable, polygon->color);
            }
        }
    }

    unsigned int calculateMaximumNumberOfElementsToProcess(const unsigned int& primaryMaximum)
    {
        int maximum = 0;
        if (primaryMaximum % 2 == 0)
        {
            maximum = primaryMaximum - 1;
        }
        else
        {
            maximum = primaryMaximum - 2;
        }
        return maximum;
    }

    Vector2D<float>* generateVerticesFromNumbers(const std::vector<int> &numbers)
    {
        if (numbers.size() < 4)
        {
            return nullptr;
        }
        const unsigned int numbersSize = static_cast<unsigned int>(numbers.size());
        const unsigned int maximumNumberOfVertices = numbersSize / 2;

        Vector2D<float>* vertices = new Vector2D<float>[maximumNumberOfVertices];

        const unsigned int maximum = calculateMaximumNumberOfElementsToProcess(static_cast<unsigned int>(numbersSize));

        int j = 0;
        for (unsigned int i = 0; i < maximum; i += 2)
        {
            vertices[j].x = static_cast<float>(numbers.at(i));
            vertices[j].y = static_cast<float>(numbers.at(i + 1));
            j++;
        }

        return vertices;
    }

    Vector2D<float>* generateVerticesFromNumbersAsVerticalLines(const std::vector<int>& numbers)
    {
        int maximumNumberOfVertices = calculateMaximumNumberOfElementsToProcess(static_cast<int>(numbers.size()));

        Vector2D<float>* vertices = new Vector2D<float>[maximumNumberOfVertices];

        for (int i = 0; i < static_cast<int>(numbers.size()); i++)
        {
            
        }

        return vertices;
    }

    void drawNumbersAsGroupOfLines(Vector2D<float>* vertices, int maximumNumberOfVertices, const Vector4D<Uint8>& color, bool areLinesContinuos, PixelsTable& pixelsTable)
    {
        int step = 1;
        if(!areLinesContinuos)
        {
            step = 2;
        }
        if (maximumNumberOfVertices > 1)
        {
            if (maximumNumberOfVertices <= 3)
            {
                drawLine(vertices[0], vertices[1], pixelsTable, color);
            }
            else
            {
                for (int i = 0; i < maximumNumberOfVertices -1; i += step)
                {
                    drawLine(vertices[i], vertices[i + 1], pixelsTable, color);
                }
            }
        }
    }

    void drawNumbersAsGroupOfLines(Vector2D<float>* vertices, int maximumNumberOfVertices, PixelsTable& pixelsTable)
    {
        if (maximumNumberOfVertices <= 3)
        {
            drawLine(vertices[0], vertices[1], pixelsTable, YELLOW);
        }
        else
        {
            int maximumVerticesToGenerateSegments = calculateMaximumNumberOfElementsToProcess(maximumNumberOfVertices);

            for (int i = 0; i < maximumVerticesToGenerateSegments; i+=2)
            {
                drawLine(vertices[i], vertices[i + 1], pixelsTable, YELLOW);
            }
        }
    }

    void drawNumbersAsPolyline(Vector2D<float>* vertices, int maximumNumberOfVertices, PixelsTable& pixelsTable)
    {
        if (maximumNumberOfVertices == 2)
        {
            drawLine(vertices[0], vertices[1], pixelsTable, YELLOW);
        }
        else
        {
            for (int i = 0; i < maximumNumberOfVertices - 1; i++)
            {
                drawLine(vertices[i], vertices[i + 1], pixelsTable, YELLOW);
            }
        }
    }

    void drawBinaryRepresentationOfFixedNumbers(std::vector<int> numbers, PixelsTable& pixelsTable)
    {
        for (unsigned int i = 0; i < static_cast<unsigned int>(numbers.size()); i++)
        {
            std::string str = std::bitset<10>(numbers.at(i)).to_string();
            for (int j = 0; j < 10; j++)
            {
                if (str.at(j) == '1')
                {
                    pixelsTable.drawPoint(j, i, YELLOW);
                }
                else
                {
                    pixelsTable.drawPoint(j, i, BLUE);
                }
            }
            str.clear();
        }
    }

    void drawPolygonDirection(GameObject* polygon, PixelsTable& pixelsTable)
    {
        Vector2D<float> direction(polygon->direction.x*100, polygon->direction.y*100);
        drawLine(polygon->vectorZero, direction, pixelsTable, polygon->color);
    }

    void swapVectors(Vector2D<int>& point0, Vector2D<int>& point1)
    {
        int temporaryX = point0.x;
        int temporaryY = point0.y;
        point0.x = point1.x;
        point0.y = point1.y;
        point1.x = temporaryX;
        point1.y = temporaryY;
    }

    void swapVectors(Vector2D<float>& point0, Vector2D<float>& point1)
    {
        float temporaryX = point0.x;
        float temporaryY = point0.y;
        point0.x = point1.x;
        point0.y = point1.y;
        point1.x = temporaryX;
        point1.y = temporaryY;
    }

    void drawCartesianAxies(PixelsTable& pixelsTable)
    {
        horizontalLineOnWholeScreen(pixelsTable, 0, RED);
        verticalLineOnWholeScreen(pixelsTable, 0, GREEN);
    }

    void drawCrossHair(float x, float y, PixelsTable& pixelsTable, bool isFullScreen)
    {
        drawCrossHair(x, y, pixelsTable, isFullScreen, GREEN);
    }

    void drawCrossHair(float x, float y, PixelsTable& pixelsTable, bool isFullScreen, Vector4D<Uint8> color)
    {
        if(isFullScreen)
        {
            horizontalLineOnWholeScreen(pixelsTable, static_cast<int>(y), BLUE);
            verticalLineOnWholeScreen(pixelsTable, static_cast<int>(x), BLUE);
        }
        else
        {
            // TODO refactor methods drawHor... and drawVer... should have float arguments and then round and cast
            drawHorizontalLine(pixelsTable, static_cast<int>(round(x) - 15), static_cast<int>(round(x) - 5), static_cast<int>(round(y)), color);
            drawHorizontalLine(pixelsTable, static_cast<int>(round(x) + 5), static_cast<int>(round(x) + 15), static_cast<int>(round(y)), color);

            drawVerticalLine(pixelsTable, static_cast<int>(round(y) - 15), static_cast<int>(round(y) - 5), static_cast<int>(round(x)), color);
            drawVerticalLine(pixelsTable, static_cast<int>(round(y) + 5), static_cast<int>(round(y) + 15), static_cast<int>(round(x)), color);
        }
    }

    void drawHorizontalLine(PixelsTable& pixelsTable, int x0, int x1,  int y, Vector4D<Uint8> color)
    {
        for (int i = x0; i < x1; i++)
        {
            pixelsTable.drawPoint(i, y, color);
        }
    }

    void drawVerticalLine(PixelsTable& pixelsTable, int y0, int y1, int x, Vector4D<Uint8> color)
    {
        if (y0 > y1)
        {
            int tmp = y0;
            y0 = y1;
            y1 = tmp;
        }
        for (int i = y0; i < y1; i++)
        {
            pixelsTable.drawPoint(x, i, color);
        }
    }

    void horizontalLineOnWholeScreen(PixelsTable& pixelsTable, int y, Vector4D<Uint8> color)
    {
        int maxX = static_cast<int>(0.5F * pixelsTable.windowDimensions.x);
		for (int i = -maxX; i < maxX; i++) //X
		{
			pixelsTable.drawPoint(i, y, color);
		}
    }

	void verticalLineOnWholeScreen(PixelsTable& pixelsTable, int x, Vector4D<Uint8> color)
	{
		int maxY = static_cast<int>(0.5F * pixelsTable.windowDimensions.y);
		for (int i = -maxY; i < maxY; i++) //X
		{
			pixelsTable.drawPoint(x, i, color);
		}
	}

    int xyPixelToArrayPosition(int x, int y, int windowWidth)
    {
        return y * windowWidth + x;
    }

    int xyPixelToArrayPosition(Vector2D<int>& point, int windowWidth)
    {
        return point.y* windowWidth + point.x;
    }

    void windowPositionToCartesianPosition(float& x, float& y, Vector2D<int>* windowDimensions)
    {
        x = x - static_cast<int>(0.5 * windowDimensions->x);
        y = (-(y - static_cast<int>(0.5 * windowDimensions->y)));
    }

    void drawMandelbrotSet()
    {

    }

void drawRectangle(PixelsTable& pixelsTable, int x, int y, int width, int height, Vector4D<Uint8> color)
{
    int posX = x;
    int posY = y ;

    pixelsTable.cartesianPositionToWindow(posX, posY);

    int startPoint = NUMBER_OF_COLORS * (posY * pixelsTable.windowDimensions.x + posX);
    int viewportIndex = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixelsTable.pixels[NUMBER_OF_COLORS * ((posY + i) * pixelsTable.windowDimensions.x + posX + j) + RED_POSITION] = color.x;// + RED_POSITION];
            pixelsTable.pixels[NUMBER_OF_COLORS * ((posY + i) * pixelsTable.windowDimensions.x + posX + j) + GREEN_POSITION] = color.y;// + GREEN_POSITION];
            pixelsTable.pixels[NUMBER_OF_COLORS * ((posY + i) * pixelsTable.windowDimensions.x + posX + j) + BLUE_POSITION] = color.z;// + BLUE_POSITION];
            pixelsTable.pixels[NUMBER_OF_COLORS * ((posY + i) * pixelsTable.windowDimensions.x + posX + j) + ALPHA_POSITION] = color.w;// + ALPHA_POSITION];
            viewportIndex = viewportIndex + 1;
        }
        startPoint = startPoint + width;
    }
}
