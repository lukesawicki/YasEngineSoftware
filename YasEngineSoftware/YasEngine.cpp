#include"YasEngine.hpp"
//#include<cstdlib>     /* srand, rand */
#include<ctime>
//#include<bit>
#include<SDL_endian.h>
#include<SDL_mixer.h>
#include"VariousTools.hpp"
#include"Circle.hpp"
#include "Collider.hpp"
#include "CosinusPointsGenerator.hpp"
#include "FibonacciPointsGenerator.hpp"
#include"Math.hpp"
#include"PrimeNumbersPointsGenerator.hpp"
#include"SinusPointsGenerator.hpp"

YasEngine* YasEngine::instance = nullptr;

void YasEngine::initialize()
{
    spawner.spawnObject(go);
    prepareBasicSettings();
    prepareRendering();
    prepareGameWorld();
    preparePlayer();
    prepareSoundAndMusic();
    prepareInterface();

    writer.initialize();

    mathPlay = new MathematicsFunSurface(0, static_cast<int>(windowDimensions->y * 0.5F), static_cast<int>(windowDimensions->x * 0.5F), static_cast<int>(windowDimensions->y), BLACK);

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

    //spawner.spawnObject(go);

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
    if constexpr (std::endian::native == std::endian::big)
    {
        std::cout << "BIG ENDIAN" << std::endl;
        endianness = 0;
    }
    else
    {
        if (std::endian::native == std::endian::little)
        {
            std::cout << "LITTLE ENDIAN" << std::endl;
            endianness = 1;
        }
        else
        {
            std::cout << "MIXED ENDIAN" << std::endl;
            endianness = 2;
        }
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
        // drawCartesianAxies(*pixelsTable);
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
                    handleGameStateWhenESCbuttonPushed();
                    break;
                case SDLK_SPACE:
                    handleGameStateWhenSPACEbuttonPushed();
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
            switch(gameState)
            {
                case GAMEPLAY:
            player->isShooting      = true;
                    break;
                case MAIN_MENU_RESTART:
                    handleClickedButtons();
                    break;
                case INTRO:
                    gameState = GameState::MAIN_MENU_RESTART;
                    break;
                case OUTRO:
                    quit = true;
                    break;
                default:
                    ;
            }
        }

        if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
        {
            switch(gameState)
            {
                case GAMEPLAY:
					player->isShooting = false;
                    break;
            }
        }
    }
}

void YasEngine::preparePlayer()
{
    player = new Player(-windowDimensions->x * 0.25F, 0);
    player->setColor(YELLOW);
    player->setInput(input);
    player->setInput(mousePositionChangeInformation);
    objectsToDraw.push_back(player);
}

void YasEngine::update(double& deltaTime)
{
    // TODO switch with handling different things
    if(gameState==GameState::GAMEPLAY) {

        spawner.spawnObject(go);
        if(go != nullptr)
        {
            objectsToDraw.push_back(go);
        }
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

    if (mousePositionChangeInformation->mouseMoved || input->up || input->down || input->left || input->right)
    {
        player->rotateToMousePositionInLocalCoordinateSystem(static_cast<float>(mousePositionChangeInformation->x), static_cast<float>(mousePositionChangeInformation->y), windowDimensions);
    }

        if(projectile != nullptr)
        {
            projectile = nullptr;
        }
    }

    mouseX = static_cast<float>(mousePositionChangeInformation->x);
    mouseY = static_cast<float>(mousePositionChangeInformation->y);

    // TODO sprawdzenie ktory Button zostal klikniety i obsluga tego

    windowPositionToCartesianPosition(mouseX, mouseY, windowDimensions);

}

void YasEngine::render(double& deltaTime) {
    pixelsTable->clearColor(BLACK);
    mathPlay->clearColor(BLACK);

    if (gameState == GameState::GAMEPLAY) // TODO if is gameplay
    {
        for (auto object: objectsToDraw) {
            if (object->isAlive) // TODO if gamestate == gameplay
        {
            drawPolygon(object, *pixelsTable);
        }
    }

	renderViewports(deltaTime);

    // verticalLineOnWholeScreen(*pixelsTable, 0, GREEN);
    // horizontalLineOnWholeScreen(*pixelsTable, 0, RED);
    } else {
        if (gameState == GameState::MAIN_MENU_RESTART) {
            drawButtons();// TODO drawPolygon(object, *pixelsTable);
        } else {
            if (gameState == GameState::INTRO) {
                writer.write(0, 100, "THE BEOUTY OF MATH       POWERED BY YASENGINE", YELLOW,
                             *pixelsTable); // TODO write title and version and tha game is powered by YasEngine
            } else {
                if (gameState == GameState::OUTRO) {
                    writer.write(0, 100,
                                 "CREDITS       CODE DESIGN LUKASZ LUKE SAWICKI       SOUND AND MUSIC FROM INTERNET WITH FRE LICENSE",
                                 BLUE,
                                 *pixelsTable); // TODO Write creators, thank you for playing and see you in other games
                }
            }
        }
    }

    writer.write(static_cast<int>(0 - (4*17)/2.0F), 0, "ABCD", BLUE, *pixelsTable);
    drawHudElements(deltaTime);

    SDL_UpdateTexture(screenTexture , NULL, pixelsTable->pixels, WINDOW_WIDTH * 4);
    SDL_RenderCopyExF(renderer, screenTexture, NULL, NULL, 0, NULL, SDL_RendererFlip::SDL_FLIP_NONE); //SDL_FLIP_VERTICAL);
    SDL_RenderPresent(renderer);
}

void YasEngine::renderViewports(double& deltaTime)
{
    mathPlay->verticalLineOnSurface(0, GREEN);
    mathPlay->horizontalLineOnSurface(0, RED);//-WINDOW_HEIGHT * 0.25F
    mathPlay->drawNumbersAsGroupOfLines(cosinusPoints->points, cosinusPoints->pointsNumber, YELLOW, true);
    mathPlay->drawNumbersAsGroupOfLines(sinusPoints->points, sinusPoints->pointsNumber, BLUE, false);
    mathPlay->drawNumbersAsGroupOfLines(fibonacciePoints->points, fibonacciePoints->pointsNumber, RED, false);
    mathPlay->drawNumbersAsGroupOfLines(primeNumbersPoints->points, primeNumbersPoints->pointsNumber, YELLOW, false);
    mathPlay->copyPixelsInToPIxelTable(*pixelsTable);
}

void YasEngine::handlePhysics()
{
    if (objectsToDraw.size() >= 3)
    {
        for (int i = 0; i < static_cast<int>(objectsToDraw.size() - 2); i++)
        {
            if((objectsToDraw[i]->iAm == GameObject::PROTAGONIST) && Collider::isCollidingWithCustomWalls(objectsToDraw[i]->collider, *windowDimensions))
            {
                float leftWall = -static_cast<float>(windowDimensions->x) * 0.5F;
                float rightWall = static_cast<float>(windowDimensions->x) * 0.0F;
                float topWall = static_cast<float>(windowDimensions->y) * 0.5F;
                float bottomWall = -static_cast<float>(windowDimensions->y) * 0.5F;
                if(objectsToDraw[i]->getPosition().x - objectsToDraw[i]->collider.radius <  leftWall)
                {
                    objectsToDraw[i]->setX(leftWall + objectsToDraw[i]->collider.radius + 1);
                }
                if(objectsToDraw[i]->getPosition().x + objectsToDraw[i]->collider.radius >  rightWall)
                {
                    objectsToDraw[i]->setX(rightWall - objectsToDraw[i]->collider.radius - 1);
                }
                if(objectsToDraw[i]->getPosition().y + objectsToDraw[i]->collider.radius > topWall)
                {
                    objectsToDraw[i]->setY(topWall - objectsToDraw[i]->collider.radius - 1);
                }
                if(objectsToDraw[i]->getPosition().y - objectsToDraw[i]->collider.radius < bottomWall)
                {
                    objectsToDraw[i]->setY(bottomWall + objectsToDraw[i]->collider.radius + 1);
                }
                //collided = true;
            }

//              DO NOT DELETE IT IS COLLISION WITH NORMAL WALLS WHICH MEANS WINDOWS BOUNDRIES
//            if((objectsToDraw[i]->iAm == GameObject::PROTAGONIST) && Collider::isCollidingWithWall(objectsToDraw[i]->collider, *windowDimensions))
//            {
//                float leftWall = -static_cast<float>(windowDimensions->x) * 0.5F;
//                float rightWall = static_cast<float>(windowDimensions->x) * 0.5F;
//                float topWall = static_cast<float>(windowDimensions->y) * 0.5F;
//                float bottomWall = -static_cast<float>(windowDimensions->y) * 0.5F;
//                if(objectsToDraw[i]->getPosition().x - objectsToDraw[i]->collider.radius <  leftWall)
//                {
//                    objectsToDraw[i]->setX(leftWall + objectsToDraw[i]->collider.radius + 1);
//                }
//                if(objectsToDraw[i]->getPosition().x + objectsToDraw[i]->collider.radius >  rightWall)
//                {
//                    objectsToDraw[i]->setX(rightWall - objectsToDraw[i]->collider.radius - 1);
//                }
//                if(objectsToDraw[i]->getPosition().y + objectsToDraw[i]->collider.radius > topWall)
//                {
//                    objectsToDraw[i]->setY(topWall - objectsToDraw[i]->collider.radius - 1);
//                }
//                if(objectsToDraw[i]->getPosition().y - objectsToDraw[i]->collider.radius < bottomWall)
//                {
//                    objectsToDraw[i]->setY(bottomWall + objectsToDraw[i]->collider.radius + 1);
//                }
//            }

            if (!objectsToDraw[i]->isAlive || (objectsToDraw[i]->iAm == GameObject::PROTAGONIST))
            {
                continue;
            }

           if (Collider::isCollidingWithCustomWalls(objectsToDraw[i]->collider, *windowDimensions))
            {
                objectsToDraw[i]->isAlive = false;
                if(objectsToDraw[i]->iAm == GameObject::PROTAGONIST)
                {
                    writer.write(0, 0, "PLAYER COLLIDING",BLUE, *pixelsTable);
                }
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

        numberOfGivenColors.insert({"RED", 0});
        numberOfGivenColors.insert({"GREEN", 0});
        numberOfGivenColors.insert({"BLUE", 0});

        std::map<std::string, int> numberOfGivenColors;
        std::map<float, int> sinusNumbers;
        std::map<float, int> cosinusNumbers;
        std::map<int, int> fibonacciNumbers;
        std::map<int, int> primeNumbers;

        for(unsigned int i=0; i<SinusPointsGenerator::numbers.size(); i++)
        {
            sinusNumbers.insert({SinusPointsGenerator::numbers.at(i),0});
        }
        for(unsigned int i=0; i<CosinusPointsGenerator::numbers.size(); i++)
        {
            cosinusNumbers.insert({ CosinusPointsGenerator::numbers.at(i),0});
        }
        for(unsigned int i=0; i<FibonacciPointsGenerator::numbers.size(); i++)
        {
            fibonacciNumbers.insert({ FibonacciPointsGenerator::numbers.at(i), 0 });
        }
        for(unsigned int i=0; i<PrimeNumbersPointsGenerator::numbers.size(); i++)
        {
            primeNumbers.insert({ PrimeNumbersPointsGenerator::numbers.at(i),0 });
        }
}

void YasEngine::prepareInterface()
{
    //Button 1
    buttons.push_back(new Button(Button::RESTART_START, "START RESTART", RED));
    buttons.at(0)->setPosition(0, 50);
    dynamic_cast<Button*>(buttons.at(0))->horizontalMargin = 10;
    dynamic_cast<Button*>(buttons.at(0))->verticalMargin = 5;
    dynamic_cast<Button*>(buttons.at(0))->buttonWidth = writer.FONT_WIDTH * dynamic_cast<Button*>(buttons.at(0))->text.size() + 2*dynamic_cast<Button*>(buttons.at(0))->horizontalMargin;
    dynamic_cast<Button*>(buttons.at(0))->buttonHeight = writer.FONT_HEIGHT + 2*dynamic_cast<Button*>(buttons.at(0))->verticalMargin;
    buttons.at(0)->localVertices = new Vector2D<float>[4];
    buttons.at(0)->worldVertices = new Vector2D<float>[4];
    buttons.at(0)->localVertices[0].x = 0 - dynamic_cast<Button*>(buttons.at(0))->buttonWidth * 0.5F;
    buttons.at(0)->localVertices[0].y = 0 + dynamic_cast<Button*>(buttons.at(0))->buttonHeight * 0.5F;
    buttons.at(0)->localVertices[1].x = 0 + dynamic_cast<Button*>(buttons.at(0))->buttonWidth * 0.5F;
    buttons.at(0)->localVertices[1].y = 0 + dynamic_cast<Button*>(buttons.at(0))->buttonHeight * 0.5F;
    buttons.at(0)->localVertices[2].x = 0 + dynamic_cast<Button*>(buttons.at(0))->buttonWidth * 0.5F;
    buttons.at(0)->localVertices[2].y = 0 - dynamic_cast<Button*>(buttons.at(0))->buttonHeight * 0.5F;
    buttons.at(0)->localVertices[3].x = 0 - dynamic_cast<Button*>(buttons.at(0))->buttonWidth * 0.5F;
    buttons.at(0)->localVertices[3].y = 0 - dynamic_cast<Button*>(buttons.at(0))->buttonHeight * 0.5F;
    buttons.at(0)->generate();

    //Button 2
    buttons.push_back(new Button(Button::QUIT, "QUIT", YELLOW));
    buttons.at(1)->setPosition(0, -50);
    dynamic_cast<Button*>(buttons.at(1))->horizontalMargin = 10;
    dynamic_cast<Button*>(buttons.at(1))->verticalMargin = 5;
    dynamic_cast<Button*>(buttons.at(1))->buttonWidth = writer.FONT_WIDTH * dynamic_cast<Button*>(buttons.at(1))->text.size() + 2*dynamic_cast<Button*>(buttons.at(1))->horizontalMargin;
    dynamic_cast<Button*>(buttons.at(1))->buttonHeight = writer.FONT_HEIGHT + 2*dynamic_cast<Button*>(buttons.at(1))->verticalMargin;
    buttons.at(1)->localVertices = new Vector2D<float>[4];
    buttons.at(1)->worldVertices = new Vector2D<float>[4];
    buttons.at(1)->localVertices[0].x = 0 - dynamic_cast<Button*>(buttons.at(1))->buttonWidth * 0.5F;
    buttons.at(1)->localVertices[0].y = 0 + dynamic_cast<Button*>(buttons.at(1))->buttonHeight * 0.5F;
    buttons.at(1)->localVertices[1].x = 0 + dynamic_cast<Button*>(buttons.at(1))->buttonWidth * 0.5F;
    buttons.at(1)->localVertices[1].y = 0 + dynamic_cast<Button*>(buttons.at(1))->buttonHeight * 0.5F;
    buttons.at(1)->localVertices[2].x = 0 + dynamic_cast<Button*>(buttons.at(1))->buttonWidth * 0.5F;
    buttons.at(1)->localVertices[2].y = 0 - dynamic_cast<Button*>(buttons.at(1))->buttonHeight * 0.5F;
    buttons.at(1)->localVertices[3].x = 0 - dynamic_cast<Button*>(buttons.at(1))->buttonWidth * 0.5F;
    buttons.at(1)->localVertices[3].y = 0 - dynamic_cast<Button*>(buttons.at(1))->buttonHeight * 0.5F;
    buttons.at(1)->generate();
}

void YasEngine::drawButtons()
{
    for(unsigned int i=0; i<buttons.size(); i++)
    {
        drawPolygon(buttons.at(i), *pixelsTable);
        writer.write(static_cast<int>(buttons.at(i)->getPosition().x - dynamic_cast<Button*>(buttons.at(i))->buttonTextWidth * 0.5F + ScreenWriter::FONT_WIDTH * 0.5F), static_cast<int>(buttons.at(i)->getPosition().y), dynamic_cast<Button*>(buttons.at(i))->text,dynamic_cast<Button*>(buttons.at(i))->color, *pixelsTable);
        
    }
}

Button::ButtonId YasEngine::checkWhichButtonClicked()
{
    //windowPositionToCartesianPosition(currentX, currentY, windowDimensions);
    float x = static_cast<float>(mousePositionChangeInformation->x);
    float y = static_cast<float>(mousePositionChangeInformation->y);
    windowPositionToCartesianPosition(x, y, windowDimensions);
    for(unsigned int i=0; i<buttons.size(); i++)
    {
        if(
        // kursor ponizej gornego Y
        y <= (buttons.at(i)->getPosition().y + dynamic_cast<Button*>(buttons.at(i))->buttonHeight * 0.5F) &&
        // kursor powyzej dolnego y
        y >= (buttons.at(i)->getPosition().y - dynamic_cast<Button*>(buttons.at(i))->buttonHeight * 0.5F) &&
        // kursor na prawo od lewego x
        x >= (buttons.at(i)->getPosition().x - dynamic_cast<Button*>(buttons.at(i))->buttonWidth * 0.5F) &&
        // kursor na lewo od prawego x
        x <= (buttons.at(i)->getPosition().x + dynamic_cast<Button*>(buttons.at(i))->buttonWidth * 0.5F)
        )
        {
            return dynamic_cast<Button*>(buttons.at(i))-> buttonId;
        }
    }
    return Button::NONE;
}

void YasEngine::handleClickedButtons()
{
    switch(checkWhichButtonClicked())
    {
        case Button::RESTART_START:
            gameState = GameState::GAMEPLAY;
            break;
        case Button::QUIT:
            gameState = GameState::OUTRO;
            break;
        default:
            ;
    }
}

void YasEngine::handleGameStateWhenESCbuttonPushed()
{
    switch(gameState)
    {            
        case INTRO:
            gameState = GameState::MAIN_MENU_RESTART;
            break;
        case MAIN_MENU_RESTART:
            gameState = GameState::OUTRO;
            break;
        case GAMEPLAY:
            gameState = GameState::MAIN_MENU_RESTART;
            break;
        case OUTRO:
            quit = true;
            break;
        default:
            ;
    }
}

void YasEngine::handleGameStateWhenSPACEbuttonPushed()
{
    switch(gameState)
    {
        case INTRO:
            gameState = GameState::MAIN_MENU_RESTART;
            break;
        case OUTRO:
            quit = true;
            break;
        default:
            ;
    }
}