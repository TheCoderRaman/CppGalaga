#include <iostream>
#include "infra/Util.hpp"
#include "entity/GameController.hpp"
#include "entity/EnemyBullet.hpp"
#include "entity/Fighter.hpp"

ol::EnemyBullet::EnemyBullet(Stage& scene) : Entity<Stage>(scene, ENEMY_BULLET_Z_ORDER)
{
    //ctor
    position_ = Vec2(-50, -50);
    size_ = Vec2(3.0f, 8.0f);
    vx_ = random(-0.25f, 0.25f);
    frame_ = 0;
    sprite_ = Sprite::ENEMY_BULLET;
    scene_.getGameController()->onEnemyBulletCreated(this);
    // log("enemy bullet id=" + std::to_string(getId()) + " created.");
}

ol::EnemyBullet::~EnemyBullet()
{
    //dtor
    scene_.getGameController()->onEnemyBulletDestroyed(this);
    // log("enemy bullet id=" + std::to_string(getId()) + " destroyed.");
}

void ol::EnemyBullet::update()
{
    position_.x += vx_;
    position_.y += 3;
    if (position_.y > SCREEN_HEIGHT + 16) {
        destroy();
    }

    checkCollisions();
}

void ol::EnemyBullet::checkCollisions()
{
    GameController* gameController = scene_.getGameController();
    if (!gameController->isFighterDestroyed() && !isDestroyed_)
    {
        Fighter* fighter = gameController->getFighter();
        if (fighter->isHitable() && !fighter->isCaptured() &&
            fighter->getCollider().collides(getCollider()))
        {
            gameController->onFighterHit(this);
            gameController->onEnemyBulletHit(this);
        }
    }
}

void ol::EnemyBullet::hit()
{
    destroy();
}
