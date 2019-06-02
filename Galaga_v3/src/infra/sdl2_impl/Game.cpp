#include <chrono>
#include <thread>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "infra/media_layer/AudioPlayer.hpp"
#include "infra/Util.hpp"
#include "infra/Keyboard.hpp"
#include "infra/media_layer/AudioPlayer.hpp"
#include "infra/media_layer/Game.hpp"
#include "infra/media_layer/Renderer.hpp"
#include "infra/StarsBackground.hpp"
#include "infra/SceneManager.hpp"

typedef std::chrono::duration<float> Duration;
typedef std::chrono::steady_clock Clock;
typedef Clock::time_point TimePoint;

static const std::chrono::microseconds TIME_STEP(16667); // = 1.0f / 60.0f

float ol::gameElapsedTime = 0; // declared in Util.hpp

SDL_Window* window; // also used in Renderer.cpp
SDL_Renderer* windowRenderer; // also used in Renderer.cpp

bool running;

extern std::map<ol::Key, bool> keysDown;
extern std::map<ol::Key, bool> keysDownConsumed;
std::map<SDL_Keycode, ol::Key> keyMap;

static void mapKeys()
{
    keyMap[SDLK_LEFT] = ol::Key::LEFT;
    keyMap[SDLK_RIGHT] = ol::Key::RIGHT;
    keyMap[SDLK_SPACE] = ol::Key::FIRE;
    keyMap[SDLK_p] = ol::Key::PAUSE;
    keyMap[SDLK_ESCAPE] = ol::Key::EXIT;
    keyMap[SDLK_q] = ol::Key::Q;
    keyMap[SDLK_w] = ol::Key::W;
    keyMap[SDLK_e] = ol::Key::E;
    keyMap[SDLK_r] = ol::Key::R;
    keyMap[SDLK_t] = ol::Key::T;
    keyMap[SDLK_y] = ol::Key::Y;
    keyMap[SDLK_u] = ol::Key::U;
    keyMap[SDLK_i] = ol::Key::I;
    keyMap[SDLK_o] = ol::Key::O;
    keyMap[SDLK_p] = ol::Key::P;
}

static void handleWindowEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            running = false;;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            if (keyMap.find(e.key.keysym.sym) != keyMap.end())
            {
                ol::Key k = keyMap[e.key.keysym.sym];
                keysDown[k] = true;
            }
        }
        else if (e.type == SDL_KEYUP)
        {
            if (keyMap.find(e.key.keysym.sym) != keyMap.end())
            {
                ol::Key k = keyMap[e.key.keysym.sym];
                keysDown[k] = false;
                keysDownConsumed[k] = false;
            }
        }
    }
}

static void init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        ol::error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
    }

    window = SDL_CreateWindow("C++ / SDL2 Galaga", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              224 * 2, 288 * 2, SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        ol::error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        ol::error("SDL_image could not initialize! SDL_image Error: " + std::string(SDL_GetError()));
    }

    //Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        ol::error("SDL_ttf could not initialize! SDL_ttf Error:" + std::string(SDL_GetError()));
    }

    //Create renderer for window
    windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (windowRenderer == NULL)
    {
        ol::error("Renderer could not be created! SDL Error: " + std::string(SDL_GetError()));
    }

    SDL_RenderSetLogicalSize(windowRenderer, 224, 288);

    ol::initAudio();
}

static void close()
{
    //Destroy window
    SDL_DestroyRenderer(windowRenderer);
    SDL_DestroyWindow(window);
    window = NULL;
    windowRenderer = NULL;

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    ol::closeAudio();
}

void ol::startGalagaGame()
{
    init();

    mapKeys();

    ol::Renderer renderer;
    ol::SceneManager sceneManager;

    Duration unprocessedTime(0);
    Duration deltaTime;
    TimePoint currentTime;
    TimePoint previousTime = Clock::now();

    running = true;

    // game loop
    while (running)
    {
        currentTime = Clock::now();
        deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        handleWindowEvents();

        // update
        unprocessedTime += deltaTime;
        while (unprocessedTime > TIME_STEP)
        {
            unprocessedTime -= TIME_STEP;
            sceneManager.update();
            gameElapsedTime += std::chrono::duration_cast<Duration>(TIME_STEP).count();
        }

        // render

        SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 255);
        SDL_RenderClear(windowRenderer);

        sceneManager.draw(renderer);

        SDL_RenderPresent(windowRenderer);

        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }

    close();
}

int main()
{
    ol::startGalagaGame();
	return EXIT_SUCCESS;
}
