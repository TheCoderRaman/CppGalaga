#include "entity/ScorePoint.hpp"

ol::ScorePoint::ScorePoint(Stage scene) : Entity<Stage>(scene, SCORE_POINT_Z_ORDER)
{
    //ctor
    pointStr_ = "";
    position_ = Vec2(-100.0f, -100.0f);
    showTime_ = 0.0f;
}

ol::ScorePoint::~ScorePoint()
{
    //dtor
}

void ol::ScorePoint::update()
{
    if (showTime_ > 0.0f && getGameElapsedTime() - showTime_ > 1.5f)
    {
        destroy();
    }
}

void ol::ScorePoint::draw(Renderer& render)
{
    render.drawText2(pointStr_, position_, color_);
}

void ol::ScorePoint::show(int point, Vec2 position)
{
    if (point < 200)
    {
        color_ = Color::CYAN;
    }
    else if (point < 800)
    {
        color_ = Color::RED;
    }
    else if (point < 1600)
    {
        color_ = Color::YELLOW;
    }
    else
    {
        color_ = Color::WHITE;
    }
    pointStr_ = std::to_string(point);
    position_ = position - Vec2(2.0f * pointStr_.size(), 3.0f);
    showTime_ = getGameElapsedTime();
}
