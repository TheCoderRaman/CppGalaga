#ifndef UTIL_HPP
#define UTIL_HPP

#include<string>

namespace ol
{
    const static int SCREEN_WIDTH = 224;
    const static int SCREEN_HEIGHT = 288;
    constexpr static float SCREEN_SCALE = 2.0f;

    extern float gameElapsedTime; // defined in Game.cpp

    // usefull for blinking, synchronizing animations, etc.
    float getGameElapsedTime();

    void log(const std::string& message);

    // show error message and exit
    void error(const std::string& message);

    // generate random float number between [0, 1)
    float random();

    // generate random float number between [min, max)
    float random(float min, float max);

    // generate random int number between [min, max]
    int random(int min, int max);

    float clamp(float value, float minv, float maxv);

    float normalizeAngle(float angle);

    // http://ww2.onvacations.co/excel-vba-color-constants
    enum Color
    {
        BLACK = 0,
        BLUE = 16711680,
        CYAN = 16776960,
        GREEN = 65280,
        MAGENTA = 16711935,
        RED = 255,
        WHITE = 16777215,
        YELLOW = 65535
    };

    int rgbToInt(int r, int g, int b);

    bool blink();

}

#endif // UTIL_HPP
