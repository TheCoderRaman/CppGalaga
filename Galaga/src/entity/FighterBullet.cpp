#include <iostream>
#include "entity/GameController.hpp"
#include "entity/FighterBullet.hpp"

ol::FighterBullet::FighterBullet(Stage& scene) : Entity<Stage>(scene, FIGHTER_BULLET_Z_ORDER)
{
    //ctor
    position_ = Vec2(50.0f, 50.0f);
    size_ = Vec2(3.0f, 8.0f);
    frame_ = 0;
    sprite_ = Sprite::FIGHTER_BULLET;
    scene_.getGameController()->onFighterBulletCreated(this);
    //log("fighter bullet id=" + std::to_string(getId()) + " created.");
}

ol::FighterBullet::~FighterBullet()
{
    //dtor
    scene_.getGameController()->onFighterBulletDestroyed(this);
    //log("fighter bullet id=" + std::to_string(getId()) + " destroyed.");
}

void ol::FighterBullet::update()
{
    position_.y -= 3;
    if (position_.y < -16) {
        destroy();
    }

    checkCollisions();
}

void ol::FighterBullet::checkCollisions()
{
    // collides with captured fighter
    GameController* gameController = scene_.getGameController();
    if (!gameController->isCapturedFighterDestroyed() && !isDestroyed_)
    {
        Fighter* capturedFighter = gameController->getCapturedFighter();
        if (capturedFighter->isHitable() && capturedFighter->isCaptured() &&
            capturedFighter->getCollider().collides(getCollider()))
        {
            gameController->onCapturedFighterHit(this);
            gameController->onFighterBulletHit(this);
        }
    }

    // collides with enemies
    Enemy* enemy = scene_.checkCollision<Enemy>(this);
    if (enemy != nullptr && !isDestroyed_)
    {
            gameController->onFighterBulletHit(this);
            gameController->onEnemyHit(enemy);
    }
}

void ol::FighterBullet::hit()
{
    destroy();
}

