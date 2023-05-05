#include"YasEngine.hpp"
//#include<cstdlib>     /* srand, rand */
#include<ctime>
//#include<bit>
//#include<SDL2/SDL_endian.h>
#include"VariousTools.hpp"
#include"Circle.hpp"
#include"Collider.hpp"
#include"CosinusPointsGenerator.hpp"
#include"FibonacciPointsGenerator.hpp"
#include"Math.hpp"
#include"PrimeNumbersPointsGenerator.hpp"
#include"SinusPointsGenerator.hpp"

YasEngine* YasEngine::instance = nullptr;

void YasEngine::initialize()
{
    //spawner.spawnObject(go);
    prepareBasicSettings();
    prepareRendering();
    prepareGameWorld();
    preparePlayer();
    prepareSoundAndMusic();

    mathPlay = new MathematicsFunSurface(0, windowDimensions->y * 0.5F, static_cast<int>(windowDimensions->x * 0.5F), static_cast<int>(windowDimensions->y * 0.5F), BLACK);

    SinusPointsGenerator sinusPointsGenerator;
    CosinusPointsGenerator cosinusPointsGenerator;
}

void YasEngine::clean()
{
    for (auto drawableObject : objectsToDraw)
    {
        delete drawableObject;
    }

    delete sinusPoints;
    delete cosinusPoints;
    delete fibonacciePoints;
    delete primeNumbersPoints;
    delete mathPlay;
    delete pixelsTable;
    delete windowDimensions;

    // clean up our resources
    Mix_FreeChunk(shootSound);
    Mix_FreeChunk(hitSound);
    Mix_FreeMusic(music);
    Mix_CloseAudio();
	Mix_Quit();
    SDL_DestroyTexture(screenTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void YasEngine::YasEnginStart()
{
    TimePicker timePicker = TimePicker();
    time = timePicker.getSeconds();
    fpsTime = 0.0F;
    frames = 0;

    spawner.spawnObject(go);

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            handleInput(event);
        }

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

        update(deltaTime);
        render(deltaTime);
    }

    clean();

    return;
}

void YasEngine::drawMathArt()
{
}

void YasEngine::prepareRendering()
{
    pixelsTable     =   new PixelsTable(WINDOW_WIDTH, WINDOW_HEIGHT, BLACK);
    renderer        =   SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

    screenTexture   =   SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
///////////////////

//    std::string basePath = SDL_GetBasePath();
//    timizedSurface->pixels

////////////////
}

void YasEngine::prepareBasicSettings()
{
    if (endianness)
    {
        std::cout << "BIG ENDIAN" << std::endl;
    }
    else
    {
        std::cout << "LITTLE ENDIAN" << std::endl;
    }

    SDL_Init(SDL_INIT_EVERYTHING);

    windowDimensions    =   new Vector2D<int>(WINDOW_WIDTH, WINDOW_HEIGHT);
    window              =   SDL_CreateWindow("YasEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

    SDL_SetWindowMinimumSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_ShowCursor(SDL_DISABLE);
}

void YasEngine::drawHudElements(double& deltaTime)
{
    #ifdef DEBUG_DRAWINGS
        drawCartesianAxies(*pixelsTable);
    #endif // DEBUG_DRAWINGS

    drawCrossHair(mouseX, mouseY, *pixelsTable, false);
}

void YasEngine::handleInput(SDL_Event& event)
{
    if (event.type == SDL_QUIT)
    {
        quit = true;
    }
    else
    {
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                quit = true;
                break;
            case SDLK_w:
                input->up = true;
                break;
            case SDLK_s:
                input->down = true;
                break;
            case SDLK_a:
                input->left = true;
                break;
            case SDLK_d:
                input->right = true;
                break;
            default:
                ;
            }
        }
        if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
                input->up = false;
                break;
            case SDLK_s:
                input->down = false;
                break;
            case SDLK_a:
                input->left = false;
                break;
            case SDLK_d:
                input->right = false;
                break;
            default:
                ;
            }
        }
        if (event.type == SDL_MOUSEMOTION)
        {
            int x;
            int y;
            mousePositionChangeInformation->mouseMoved = true;
            SDL_GetMouseState(&x, &y);
            mousePositionChangeInformation->x = x;
            mousePositionChangeInformation->y = y;

        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            player->isShooting      = true;
        }
        if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
        {
            player->isShooting      = false;
        }
    }
}

void YasEngine::preparePlayer()
{
    player = new Player(0, 0);
    player->setColor(YELLOW);
    player->setInput(input);
    player->setInput(mousePositionChangeInformation);
    objectsToDraw.push_back(player);
}

void YasEngine::update(double& deltaTime)
{
    //int numberOfPhysicalObjects = objectsToDraw.size();

    //std::cout << numberOfPhysicalObjects << std::endl;

    handlePhysics();
    for (auto object : objectsToDraw)
    {
        if (object->isAlive)
        {
            object->move(static_cast<float>(deltaTime));
            object->regeneratePolygon();
        }
    }

    Projectile* projectile = player->shoot();
    if (projectile != nullptr)
    {
        Mix_PlayChannel(-1, shootSound, 0);
        objectsToDraw.push_back(projectile);
    }

    if(go != nullptr)
    {
        objectsToDraw.push_back(go);
    }

    if (mousePositionChangeInformation->mouseMoved || input->up || input->down || input->left || input->right)
    {
        player->rotateToMousePositionInLocalCoordinateSystem(static_cast<float>(mousePositionChangeInformation->x), static_cast<float>(mousePositionChangeInformation->y), windowDimensions);
    }

    mouseX = static_cast<float>(mousePositionChangeInformation->x);
    mouseY = static_cast<float>(mousePositionChangeInformation->y);

    windowPositionToCartesianPosition(mouseX, mouseY, windowDimensions);

    projectile = nullptr;
}

void YasEngine::render(double& deltaTime)
{
    pixelsTable->clearColor(BLACK);
    mathPlay->clearColor(BLACK);

    for (auto object : objectsToDraw)
    {
        if (object->isAlive)
        {
            drawPolygon(object, *pixelsTable);
        }
    }
    
    int vertical = static_cast<int>(-WINDOW_WIDTH * 0.25F);
    int horizontal = static_cast<int>(-WINDOW_HEIGHT * 0.25F);

    mathPlay->verticalLineOnSurface(0, GREEN);
    mathPlay->horizontalLineOnSurface(0, RED);//-WINDOW_HEIGHT * 0.25F

	// mathPlay->drawNumbersAsGroupOfNotConnectedLines(sinusPoints, 100, YELLOW);

    mathPlay->drawNumbersAsGroupOfLines(cosinusPoints->points, cosinusPoints->pointsNumber, YELLOW, true);
    mathPlay->drawNumbersAsGroupOfLines(sinusPoints->points, sinusPoints->pointsNumber, BLUE, false);
    mathPlay->drawNumbersAsGroupOfLines(fibonacciePoints->points, fibonacciePoints->pointsNumber, RED, false);

    mathPlay->drawNumbersAsGroupOfLines(primeNumbersPoints->points, primeNumbersPoints->pointsNumber, YELLOW, false);

    mathPlay->copyPixelsInToPIxelTable(*pixelsTable);

    drawRectangle(*pixelsTable, -110, -110, 32, 32,YELLOW);

    verticalLineOnWholeScreen(*pixelsTable, 0, GREEN);
    horizontalLineOnWholeScreen(*pixelsTable, 0, RED);

    drawHudElements(deltaTime);

    SDL_UpdateTexture(screenTexture , NULL, pixelsTable->pixels, WINDOW_WIDTH * 4);
    SDL_RenderCopyExF(renderer, screenTexture, NULL, NULL, 0, NULL, SDL_RendererFlip::SDL_FLIP_NONE); //SDL_FLIP_VERTICAL);
    SDL_RenderPresent(renderer);
}

void YasEngine::handlePhysics()
{
    if (objectsToDraw.size() >= 3)
    {
        for (int i = 0; i < static_cast<int>(objectsToDraw.size() - 2); i++)
        {
            if (!objectsToDraw[i]->isAlive || objectsToDraw[i]->iAm == GameObject::PROTAGONIST)
            {
                continue;
            }

            if (Collider::isCollidingWithWall(objectsToDraw[i]->collider, *windowDimensions))
            {
                objectsToDraw[i]->isAlive = false;
                //std::cout << "HIT" << std::endl;
                continue;
            }

            for (int j = i; j < static_cast<int>(objectsToDraw.size()); j++)
            {
                if (!objectsToDraw[j]->isAlive || objectsToDraw[j]->iAm == GameObject::PROTAGONIST ||
                    (objectsToDraw[i]->iAm == GameObject::PROJECTILE && objectsToDraw[j]->iAm == GameObject::PROJECTILE) ||
                    (objectsToDraw[i]->iAm == GameObject::COLLECTIBLE && objectsToDraw[j]->iAm == GameObject::COLLECTIBLE))
                {
                    continue;
                }

                if (!(objectsToDraw[i] == objectsToDraw[j]) && objectsToDraw[i]->isAlive && objectsToDraw[j]->isAlive)
                {
                    if (Collider::isInCollision(objectsToDraw[i]->collider, objectsToDraw[j]->collider))
                    {
                        objectsToDraw[i]->isAlive = false;
                        objectsToDraw[j]->isAlive = false;
                        Mix_PlayChannel(-1, hitSound, 0);
                    }
                }
            }
        }
    }
}

void YasEngine::prepareSoundAndMusic()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
    {
        std::cout << "Error cannot open audio device" << std::endl;
    }

    std::string basePath = SDL_GetBasePath();
    std::cout << "Base path is: " << basePath << std::endl;

    std::string musicFilePath;
    musicFilePath.append(basePath);
    musicFilePath.append("music.wav");

    std::string shootSoundFilePath;
    shootSoundFilePath.append(basePath);
    shootSoundFilePath.append("shoot.wav");

    std::string hitSoundFilePath;
    hitSoundFilePath.append(basePath);
    hitSoundFilePath.append("hit.wav");
    std::cout << "hit.wav path: -> " << hitSoundFilePath << std::endl;

    Mix_Init(MIX_DEFAULT_FORMAT);

    music = Mix_LoadMUS(musicFilePath.c_str());
    if (music == NULL)
    {
        std::cout << "Error while loading music. Cannot load music." << std::endl;
        std::cout << "SDL message: " << SDL_GetError() << std::endl << " | Mix library error: " << Mix_GetError() << std::endl;
        quit = true;
    }

    shootSound = Mix_LoadWAV(shootSoundFilePath.c_str());
    hitSound = Mix_LoadWAV(hitSoundFilePath.c_str());

    if (shootSound == NULL || hitSound == NULL)
    {
        std::cout << "Error while loading sounds. Cannot load sounds." << std::endl;
        std::cout << "SDL message: " << SDL_GetError() << std::endl << " | Mix library error: " << Mix_GetError() << std::endl;
        quit = true;
    }
    Mix_PlayMusic(music, 999);
}

void YasEngine::prepareGameWorld()
{
    #ifdef DEBUG_DRAWINGS
        Circle* circle = new Circle(100, 0, 0);
        circle->setColor(BLUE);
        objectsToDraw.push_back(circle);
    #endif
        SinusPointsGenerator sinusPointsGenerator;
        CosinusPointsGenerator cosinusPointsGenerator;
        FibonacciPointsGenerator fibonacciPointsGenerator;
        PrimeNumbersPointsGenerator primeNumberPointsGenerator;

        sinusPoints = sinusPointsGenerator.generatePoints();
        cosinusPoints = cosinusPointsGenerator.generatePoints();
        fibonacciePoints = fibonacciPointsGenerator.generatePoints();
        primeNumbersPoints = primeNumberPointsGenerator.generatePoints();

        spawner.position.x = -200;
        spawner.position.y = 0;
}
