#include <iostream>
#include <cmath>
#include "infra/Util.hpp"
#include "infra/StarsBackground.hpp"

ol::StarsBackground::StarsBackground()
{
    //ctor
    scaledVelocityY_ = 1.0f;
    targetScaledVelocityY_ = 1.0f;
    createStars();
}

ol::StarsBackground::~StarsBackground()
{
    //dtor
}

void ol::StarsBackground::createStars()
{
    int index = 0;
    for (int v = 0; v < 3; v++)
    {
        for (int i = 0; i < STARS_COUNT / 3; i++)
        {
            int r = ol::random(64, 255);
            int g = ol::random(64, 255);
            int b = ol::random(64, 255);
            stars_[index].color = ol::rgbToInt(r, g, b);
            stars_[index].position.x = ol::random(0, SCREEN_WIDTH);

            stars_[index].position.y = ol::random(0, SCREEN_HEIGHT) -
                                       2 * SCREEN_HEIGHT;

            stars_[index].yVelocity = (v + 1);
            stars_[index].blink = i > STARS_COUNT / 6;
            stars_[index].blinkTime = ol::random();
            index++;
        }
    }
}

void ol::StarsBackground::update()
{
    // update vertical velocity scale to move or stop the stars smoothly
    float dif = targetScaledVelocityY_ - scaledVelocityY_;
    if (fabs(dif) < 0.02)
    {
        scaledVelocityY_ = targetScaledVelocityY_;
    }
    else if (dif < 0)
    {
        scaledVelocityY_ -= 0.015f;
    }
    else
    {
        scaledVelocityY_ += 0.015f;
    }

    // update all stars
    for (int i = 0; i < STARS_COUNT; i++)
    {
        stars_[i].position.y += scaledVelocityY_ * stars_[i].yVelocity;
        stars_[i].blinkTime += 0.025f;
        if (stars_[i].position.y > ol::SCREEN_HEIGHT)
        {
            stars_[i].position.y = -1;
        }
    }
}

void ol::StarsBackground::draw(Renderer& renderer)
{
    for (int i = 0; i < STARS_COUNT; i++)
    {
        if (!stars_[i].blink || (stars_[i].blink &&
            ((int) stars_[i].blinkTime) % 2 == 0))
        {
            renderer.drawPoint(stars_[i].position, stars_[i].color);
        }
    }
}

void ol::StarsBackground::move()
{
    targetScaledVelocityY_ = 1.0f;
}

void ol::StarsBackground::stop()
{
    targetScaledVelocityY_ = 0.0f;
}
