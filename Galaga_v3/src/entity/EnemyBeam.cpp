#include <iostream>
#include "infra/Util.hpp"
#include "entity/GameController.hpp"
#include "entity/EnemyBeam.hpp"
#include "entity/Fighter.hpp"
#include "entity/Enemy.hpp"

ol::EnemyBeam::EnemyBeam(Stage& scene) : Entity<Stage>(scene, ENEMY_BEAM_Z_ORDER)
{
    //ctor
    enemy_ = nullptr;
    position_ = Vec2(-100.0f, -100.0f);
    size_ = Vec2(46.0f, 0.0f);
    frame_ = 0.0f;
    sprite_ = Sprite::ENEMY_BEAM;
    height_ = 0.0f;
    isFinished_ = false;
    doNothing();
    scene_.getGameController()->onEnemyBeamCreated(this);
    //log("enemy beam id=" + std::to_string(getId()) + "created.";
}

ol::EnemyBeam::~EnemyBeam()
{
    //dtor
    scene_.getGameController()->onFighterCaptureEnd();
    scene_.getGameController()->onEnemyBeamDestroyed(this);
    //log("enemy beam id=" + std::to_string(getId()) + "destroyed.";
}

float ol::EnemyBeam::getHeight()
{
    return height_;
}

ol::Collider ol::EnemyBeam::getCollider()
{
    return Collider(position_, Vec2(46.0f, height_));
}

bool ol::EnemyBeam::isFinished()
{
    return isFinished_;
}

ol::Enemy* ol::EnemyBeam::getEnemy()
{
    return enemy_;
}

ol::Fighter* ol::EnemyBeam::getCapturedFighter()
{
    return capturedFighter_;
}

void ol::EnemyBeam::update()
{
    updateAnimation();
    Entity::update();

    if (enemy_ != nullptr && enemy_->isDestroyed())
    {
        destroy();
    }
}

void ol::EnemyBeam::draw(Renderer& renderer)
{
    Vec2 srcPosition(46.0f * static_cast<int>(frame_), 0.0f);
    Vec2 srcSize(46.0f, height_);
    renderer.drawSprite(sprite_, position_, srcPosition, srcSize);
    //drawCollider(renderer);
}

void ol::EnemyBeam::show(Enemy* enemy)
{
    enemy_ = enemy;
    position_ = Vec2(-23.0f, 8.0f) + enemy->getPosition();
    setUpdateAction([=] ()
    {
        height_ += 1.0f;
        if (height_ > 80.0f)
        {
            height_ = 80.0f;
            wait();
        }
        checkFighterCaptured();
    });
}

void ol::EnemyBeam::updateAnimation()
{
    frame_ += 0.15f;
    if (frame_ > 3.0f)
    {
        frame_ = 0.0f;
    }
}

void ol::EnemyBeam::checkFighterCaptured()
{
    GameController* gameController = scene_.getGameController();
    if (!enemy_->isDestroyed() && !gameController->isFighterDestroyed() &&
        !gameController->isFighterCaptured())
    {
        Fighter* fighter = gameController->getFighter();
        if (fighter->getCollider().collides(getCollider()))
        {
            capturedFighter_ = fighter;
            gameController->onFighterCaptured(this);
            showTime_ = getGameElapsedTime();
        }
    }
}

void ol::EnemyBeam::wait()
{
    showTime_ = getGameElapsedTime();
    setUpdateAction([=] ()
    {
        checkFighterCaptured();
        if (getGameElapsedTime() - showTime_ < 2.5f)
        {
            return;
        }
        hide();
    });
}

void ol::EnemyBeam::hide()
{
    setUpdateAction([=] ()
    {
        height_ -= 1.0f;
        if (height_ < 0.0f)
        {
            height_ = 0.0f;
            if (!scene_.getGameController()->isFighterCaptured())
            {
                enemy_->returnToFormation();
                destroy();
            }
            isFinished_ = true;
            // note: if fighter captured, this entity is destroyed by Gamecontroller
        }
    });
}

