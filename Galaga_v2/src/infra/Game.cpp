#include <chrono>
#include <thread>
#include <infra/Util.hpp>
#include <infra/Keyboard.hpp>
#include <infra/AudioPlayer.hpp>
#include <infra/Game.hpp>
#include <infra/Display.hpp>
#include <infra/Renderer.hpp>
#include <infra/StarsBackground.hpp>
#include <infra/SceneManager.hpp>

typedef std::chrono::duration<float> Duration;
typedef std::chrono::steady_clock Clock;
typedef Clock::time_point TimePoint;

static const std::chrono::microseconds TIME_STEP(16667); // = 1.0f / 60.0f

float ol::gameElapsedTime = 0; // declared in Util.hpp

void ol::startGalagaGame()
{
    ol::Display display;
    ol::Renderer renderer;
    //ol::playMusic(ol::Music::START_GAME);
    //ol::playSound(ol::Sound::FIGHTER_SHOT);
    ol::SceneManager sceneManager;

    Duration unprocessedTime(0);
    Duration deltaTime;
    TimePoint currentTime;
    TimePoint previousTime = Clock::now();

    // game loop
    while (display.isOpen())
    {
        currentTime = Clock::now();
        deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        display.handleWindowEvents();

        // update
        unprocessedTime += deltaTime;
        while (unprocessedTime > TIME_STEP)
        {
            unprocessedTime -= TIME_STEP;
            sceneManager.update();
            gameElapsedTime += std::chrono::duration_cast<Duration>(TIME_STEP).count();
        }

        // render
        display.clear();
        sceneManager.draw(renderer);
        display.show();

        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }

}
