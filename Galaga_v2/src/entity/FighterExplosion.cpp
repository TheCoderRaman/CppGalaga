#include <iostream>
#include "entity/FighterExplosion.hpp"

ol::FighterExplosion::FighterExplosion(Stage& scene) : Entity<Stage>(scene, FIGHTER_EXPLOSION_Z_ORDER)
{
    //ctor
    position_ = Vec2(-50.0f, -50.0f);
    size_ = Vec2(32.0f, 32.0f);
    frame_ = 0;
    sprite_ = Sprite::FIGHTER_EXPLOSION;
    // log("fighter explosion id=" + std::to_string(getId()) + " created.");
}

ol::FighterExplosion::~FighterExplosion()
{
    //dtor
    // log("fighter explosion id=" + std::to_string(getId()) + " destroyed.");
}

void ol::FighterExplosion::update()
{
    frame_ += 0.15f;
    if (frame_ > 3.5f)
    {
        destroy();
        return;
    }
}
