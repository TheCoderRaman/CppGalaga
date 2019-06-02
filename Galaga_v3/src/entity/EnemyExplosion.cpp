#include <iostream>
#include "entity/EnemyExplosion.hpp"

#define ENEMY_EXPLOSION_Z_BUFFER 10

ol::EnemyExplosion::EnemyExplosion(Stage& scene) : Entity<Stage>(scene, ENEMY_EXPLOSION_Z_BUFFER)
{
    //ctor
    position_ = Vec2(-50.0f, -50.0f);
    size_ = Vec2(32.0f, 32.0f);
    frame_ = 0;
    sprite_ = Sprite::ENEMY_EXPLOSION;
    // log("enemy explosion id=" + std::to_string(getId()) + " created.");
}

ol::EnemyExplosion::~EnemyExplosion()
{
    //dtor
    // log("enemy explosion id=" + std::to_string(getId()) + " destroyed.");
}

void ol::EnemyExplosion::update()
{
    frame_ += 0.2f;
    if (frame_ > 4.5f)
    {
        destroy();
        return;
    }
}
