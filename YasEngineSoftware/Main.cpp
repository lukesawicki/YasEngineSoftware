#include<Windows.h>
#include<GLFW/glfw3.h>
#include<gl/gl.h>
#include<glext.h>
#include<cstdlib>
#include<iostream>
#include<vector>
#include<cmath>
#include"YasVector2D.hpp"
#include"YasGraphicsLibrary.hpp"
#include"TimePicker.hpp"
#include"PixelsTable.hpp"
#include"Circle.hpp"
#include<vector>
#include"YasPolygon.hpp"
#include"Player.hpp"
#include"InputOutputHandler.hpp"

//-----------------------------------------------------------------------------|---------------------------------------|
//                                                                            80                                     120
bool shouldApplicationStopRunning = false;
YasInOut::Input* input = new YasInOut::Input();
YasInOut::MousePositionChangeInformation* mousePositionChangeInformation = new YasInOut::MousePositionChangeInformation();

void windowToCartesian(double& x, double& y, YasVector2D<int>& windowDimensions)
{
	x = x - static_cast<int>(0.5 * windowDimensions.x);
	y = (-(y - static_cast<int>(0.5 * windowDimensions.y)));
}

void keysHandleCallbackFunction(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
        shouldApplicationStopRunning = true;
        glfwSetWindowShouldClose(window, GL_TRUE);
	}

    // PRESS KEY

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		input->up = true;
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		input->down = true;
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		input->left = true;
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		input->right = true;
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		input->rotateCounterClockwise = true;
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		input->shoot = true;
	}

    // RELEASE KEY

	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		input->up = false;
	}

	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		input->down = false;
	}

	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
	{
		input->left = false;
	}

	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
	{
		input->right = false;
	}

	if (key == GLFW_KEY_R && action == GLFW_RELEASE)
	{
		input->rotateCounterClockwise = false;
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
	{
		input->shoot = false;
	}
}

void mouseButtonsCallbackFunction(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        mousePositionChangeInformation->leftMouseButton = true;
    }

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
        mousePositionChangeInformation->rightMouseButton = true;
	}

    //////////////////////////////////////////////////////////////////////////

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
        mousePositionChangeInformation->leftMouseButton = false;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
        mousePositionChangeInformation->rightMouseButton = false;
	}
}

void mouseMoveHandleCallbackFunction(GLFWwindow* window, double x, double y)
{
    if (abs(mousePositionChangeInformation->x - x) > 1 || abs(mousePositionChangeInformation->y - y) > 1)
    {
        mousePositionChangeInformation->mouseMoved = true;
    }
    else
    {
        mousePositionChangeInformation->mouseMoved = false;
        return;
    }

	mousePositionChangeInformation->x = x;
	mousePositionChangeInformation->y = y;

}

int main(int argc, char* argv[])
{
    const int WINDOW_WIDTH = 1024;
	const int WINDOW_HEIGHT = 768;

    YasVector2D<int>* windowDimensions = new YasVector2D<int>(WINDOW_WIDTH, WINDOW_HEIGHT);

    GLFWwindow* window;

    if (!glfwInit())
    {
        return 1;
    }

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keysHandleCallbackFunction);

    glfwSetCursorPosCallback(window, mouseMoveHandleCallbackFunction);

    if(glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    glfwSetMouseButtonCallback(window, mouseButtonsCallbackFunction);

    PixelsTable pixelsTable(WINDOW_WIDTH, WINDOW_HEIGHT, BLACK);

    YasVector2D<int> zeroVector;
    YasVector2D<int> direction;

	YasVector2D<int> testA;
	YasVector2D<int> testB;

	std::vector<YasPolygon*> objectsToDraw;

    Player* player = new Player(0, 0);
    player->setColor(YELLOW);
    player->setInput(input);
    player->setInput(mousePositionChangeInformation);

    Circle* circle = new Circle(100, 0, 0);
    circle->setColor(BLUE);

    objectsToDraw.push_back(circle);
    objectsToDraw.push_back(player);

    // End of test objects definitions

    double time;
    double newTime;
    double deltaTime;
    double fps;
    double fpsTime;
    unsigned int frames;

    TimePicker timePicker = TimePicker();
    time = timePicker.getSeconds();

    fpsTime = 0.0F;
    frames = 0;
    bool close = false;
    bool firstTime = true;

    while (!shouldApplicationStopRunning)
    {
        while (!glfwWindowShouldClose(window))
        {
            newTime = timePicker.getSeconds();
            deltaTime = newTime - time;
            time = newTime;

            ++frames;
            fpsTime = fpsTime + deltaTime;
            if (fpsTime >= 1.0F)
            {
                fps = frames / fpsTime;
                frames = 0;
                fpsTime = 0.0F;
            }
            
            pixelsTable.clearColor(BLACK);
            
			drawCartesianAxies(pixelsTable);

//          ########  BEGINT TEST CODE  ################

            if (mousePositionChangeInformation->mouseMoved)
            {
                player->rotateToMousePosition(mousePositionChangeInformation->x, mousePositionChangeInformation->y, windowDimensions);
                //verystrangeExperiment
                //player->rotate(static_cast<float>(deltaTime)); // rotation using key
            }


            //player->rotateToMousePosition(mousePositionChangeInformation->x, mousePositionChangeInformation->y, windowDimensions);
            //player->rotate(static_cast<float>(deltaTime)); // rotation using key

            for (auto object : objectsToDraw)
            {
                object->move(static_cast<float>(deltaTime));
                object->regeneratePolygon();
                drawPolygon(object, pixelsTable);
            }

            //drawPolygon(player, pixelsTable);

            // DRAW YELLOW LINE WHICH SHOWING THE DIRECTION OF MOUSE(PREVIOUSLY PLAYER)
            drawPolygonDirection(player, pixelsTable);

            Projectile* projectile = player->shoot();
            if (projectile != nullptr)
            {
                objectsToDraw.push_back(projectile);
            }

            projectile = nullptr;

//          ########  END TEST CODE  ################

            glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, pixelsTable.pixels);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }


        
        for (auto drawableObject : objectsToDraw)
        {
            delete drawableObject;
        }

        glfwTerminate();
        return 0;
    }
}

//                                                                            80                                     120
//-----------------------------------------------------------------------------|---------------------------------------|
