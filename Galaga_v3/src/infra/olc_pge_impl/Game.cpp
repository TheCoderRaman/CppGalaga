#include <chrono>
#include <thread>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

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

extern std::map<ol::Key, bool> keysDown;
extern std::map<ol::Key, bool> keysDownConsumed;
std::map<olc::Key, ol::Key> keyMap;

static void mapOlKeys()
{
    keyMap[olc::Key::LEFT] = ol::Key::LEFT;
    keyMap[olc::Key::RIGHT] = ol::Key::RIGHT;
    keyMap[olc::Key::SPACE] = ol::Key::FIRE;
    keyMap[olc::Key::P] = ol::Key::PAUSE;
    keyMap[olc::Key::ESCAPE] = ol::Key::EXIT;
    keyMap[olc::Key::Q] = ol::Key::Q;
    keyMap[olc::Key::W] = ol::Key::W;
    keyMap[olc::Key::E] = ol::Key::E;
    keyMap[olc::Key::R] = ol::Key::R;
    keyMap[olc::Key::T] = ol::Key::T;
    keyMap[olc::Key::Y] = ol::Key::Y;
    keyMap[olc::Key::U] = ol::Key::U;
    keyMap[olc::Key::I] = ol::Key::I;
    keyMap[olc::Key::O] = ol::Key::O;
    keyMap[olc::Key::P] = ol::Key::P;
}

static Duration unprocessedTime(0);
static Duration deltaTime;
static TimePoint currentTime;
static TimePoint previousTime = Clock::now();

class OlcPgeWindow : public olc::PixelGameEngine
{
public:
	OlcPgeWindow()
	{
		sAppName = "Galaga";
	}

	bool OnUserCreate() override
	{
        mapOlKeys();
	    SetPixelMode(olc::Pixel::MASK);

	    //ol::log("OlcPgeWindow created !!!");

		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        Clear(olc::BLACK);
        updateKeyStates();

        currentTime = Clock::now();
        deltaTime = currentTime - previousTime;
        previousTime = currentTime;
        unprocessedTime += deltaTime;
        while (unprocessedTime > TIME_STEP)
        {
            unprocessedTime -= TIME_STEP;
            sceneManager.update();
            ol::gameElapsedTime += std::chrono::duration_cast<Duration>(TIME_STEP).count();
        }

        sceneManager.draw(renderer);

		return true;
	}

private:

    void updateKeyStates()
    {
        for (auto const& key : keyMap)
        {
            auto olcKey = key.first;
            auto olKey = key.second;

            olc::HWButton keyState = this->GetKey(olcKey);
            if (keyState.bPressed)
            {
                keysDown[olKey] = true;
            }
            else if (keyState.bReleased)
            {
                keysDown[olKey] = false;
                keysDownConsumed[olKey] = false;
            }
        }
    }

private:
	std::function<bool (float fElapsedTime)> update_;
    ol::Renderer renderer;
    ol::SceneManager sceneManager;

};

OlcPgeWindow* olcPgeWindow; // also used in Renderer.cpp

void ol::startGalagaGame()
{
    OlcPgeWindow window;
    olcPgeWindow = &window;
	if (window.Construct(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE, SCREEN_SCALE))
	{
		window.Start();
	}
}

int main()
{
    ol::startGalagaGame();
	return EXIT_SUCCESS;
}
