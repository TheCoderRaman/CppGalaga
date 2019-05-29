#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
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

sf::RenderWindow* window; // also used in Renderer.cpp

extern std::map<ol::Key, bool> keysDown;
extern std::map<ol::Key, bool> keysDownConsumed;
std::map<sf::Keyboard::Key, ol::Key> keyMap;

static void mapKeys()
{
    keyMap[sf::Keyboard::Key::Left] = ol::Key::LEFT;
    keyMap[sf::Keyboard::Key::Right] = ol::Key::RIGHT;
    keyMap[sf::Keyboard::Key::Space] = ol::Key::FIRE;
    keyMap[sf::Keyboard::Key::P] = ol::Key::PAUSE;
    keyMap[sf::Keyboard::Key::Escape] = ol::Key::EXIT;
    keyMap[sf::Keyboard::Key::Q] = ol::Key::Q;
    keyMap[sf::Keyboard::Key::W] = ol::Key::W;
    keyMap[sf::Keyboard::Key::E] = ol::Key::E;
    keyMap[sf::Keyboard::Key::R] = ol::Key::R;
    keyMap[sf::Keyboard::Key::T] = ol::Key::T;
    keyMap[sf::Keyboard::Key::Y] = ol::Key::Y;
    keyMap[sf::Keyboard::Key::U] = ol::Key::U;
    keyMap[sf::Keyboard::Key::I] = ol::Key::I;
    keyMap[sf::Keyboard::Key::O] = ol::Key::O;
    keyMap[sf::Keyboard::Key::P] = ol::Key::P;
}

static void handleWindowEvents()
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window->close();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (keyMap.find(event.key.code) != keyMap.end())
            {
                ol::Key k = keyMap[event.key.code];
                keysDown[k] = true;
            }
        }
        else if (event.type == sf::Event::KeyReleased)
        {
            if (keyMap.find(event.key.code) != keyMap.end())
            {
                ol::Key k = keyMap[event.key.code];
                keysDown[k] = false;
                keysDownConsumed[k] = false;
            }
        }
    }
}

void ol::startGalagaGame()
{
    sf::RenderWindow windowLocal(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "C++ / SFML Galaga");

    window = &windowLocal;

    window->setSize(sf::Vector2u(
         (SCREEN_WIDTH * SCREEN_SCALE),
         (SCREEN_HEIGHT * SCREEN_SCALE) ) );

    mapKeys();

    ol::Renderer renderer;
    ol::SceneManager sceneManager;

    Duration unprocessedTime(0);
    Duration deltaTime;
    TimePoint currentTime;
    TimePoint previousTime = Clock::now();

    // game loop
    while (window->isOpen())
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
        window->clear();
        sceneManager.draw(renderer);
        window->display();

        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
}

int main()
{
    ol::startGalagaGame();
	return EXIT_SUCCESS;
}
