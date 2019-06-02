#include <iostream>
#include <random>
#include <ctime>
#include "infra/Util.hpp"

static std::default_random_engine randomEngine(std::time(nullptr));
static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

float ol::getGameElapsedTime()
{
    return gameElapsedTime;
}

void ol::log(const std::string& message)
{
    std::cout << message << std::endl;
}

void ol::error(const std::string& message)
{
    std::cout << "Error: " << message << std::endl;
    getchar();
    exit(EXIT_FAILURE);
}

float ol::random()
{
    return distribution(randomEngine);
}

float ol::random(float min, float max)
{
    return min + random() * (max - min);
}

int ol::random(int min, int max)
{
    return static_cast<int>(min + random() * (max - min + 1));
}

float ol::clamp(float value, float minv, float maxv)
{
    return std::min(std::max(value, minv), maxv);
}

// Reference: https://stackoverflow.com/questions/1628386/normalise-orientation-between-0-and-360
float ol::normalizeAngle(float angle)
{
    if (angle < 0)
    {
        angle += 2 * 3.14159f;
    }
    return angle;
}

int ol::rgbToInt(int r, int g, int b)
{
    return r + 255 * g + 255 * 255 + b;
}

bool ol::blink()
{
    return static_cast<int>(gameElapsedTime * 5.0f) % 3 > 0;
}
