#include <SFML/Graphics.hpp>
#include "infra/Display.hpp"
#include "infra/Keyboard.hpp"

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

ol::Display::Display()
{
    window = new sf::RenderWindow(
        sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "C++ / SFML Galaga");

    window->setSize(sf::Vector2u(
         (SCREEN_WIDTH * SCREEN_SCALE),
         (SCREEN_HEIGHT * SCREEN_SCALE) ) );

    mapKeys();
}

ol::Display::~Display()
{
    delete window;
}

bool ol::Display::isOpen()
{
    return window->isOpen();
}

void ol::Display::handleWindowEvents()
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
                Key k = keyMap[event.key.code];
                keysDown[k] = true;
            }
        }
        else if (event.type == sf::Event::KeyReleased)
        {
            if (keyMap.find(event.key.code) != keyMap.end())
            {
                Key k = keyMap[event.key.code];
                keysDown[k] = false;
                keysDownConsumed[k] = false;
            }
        }
    }
}

void ol::Display::clear()
{
    window->clear();
}

void ol::Display::show()
{
    window->display();
}
