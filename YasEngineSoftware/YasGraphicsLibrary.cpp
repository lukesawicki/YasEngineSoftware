#include"YasGraphicsLibrary.hpp"

namespace YasGL
{
    void drawGentleSlopeLine(Vector2D<int>* point0, Vector2D<int>* point1, SDL_Renderer* renderer)
    {
        int deltaX = point1->x - point0->x;
        int deltaY = point1->y - point0->y;

        int yIteration = 1;

        if (deltaY < 0)
        {
            yIteration = -1;
            deltaY = -deltaY;
        }

        int difference = (2 * deltaY) - deltaX;

        int y = point0->y;

        for (int i = point0->x; i <= point1->x; i++)
        {
            SDL_RenderDrawPoint(renderer, i, y);
            if (difference > 0)
            {
                y = y + yIteration;
                difference = difference + (2 * (deltaY - deltaX));
            }
            else
            {
                difference = difference + 2 * deltaY;
            }
        }
    }

    void drawSteepSlopeLine(Vector2D<int>* point0, Vector2D<int>* point1, SDL_Renderer* renderer)
    {
        int deltaX = point1->x - point0->x;
        int deltaY = point1->y - point0->y;

        int xIteration = 1;

        if (deltaX < 0)
        {
            xIteration = -1;
            deltaX = -deltaX;
        }

        int difference = (2 * deltaX) - deltaY;

        int x = point0->x;

        for (int i = point0->y; i <= point1->y; i++)
        {
            SDL_RenderDrawPoint(renderer, x, i);
            if (difference > 0)
            {
                x = x + xIteration;
                difference = difference + (2 * (deltaX - deltaY));
            }
            else
            {
                difference = difference + 2 * deltaX;
            }
        }
    }

    void drawLine(Vector2D<int>* point0, Vector2D<int>* point1, SDL_Renderer* renderer)
    {
        if (abs(point1->y - point0->y) < abs(point1->x - point0->x))
        {
            if (point0->x > point1->x)
            {
                drawGentleSlopeLine(point1, point0, renderer);
            }
            else
            {
                drawGentleSlopeLine(point0, point1, renderer);
            }
        }
        else
        {
            if (point0->y > point1->y)
            {
                drawSteepSlopeLine(point1, point0, renderer);
            }
            else
            {
                drawSteepSlopeLine(point0, point1, renderer);
            }
        }
    }

}
