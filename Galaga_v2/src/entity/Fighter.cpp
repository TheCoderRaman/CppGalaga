#include <cmath>
#include "infra/Util.hpp"
#include "infra/Path.hpp"
#include "infra/Keyboard.hpp"
#include "infra/media_layer/AudioPlayer.hpp"
#include "entity/GameController.hpp"
#include "entity/Fighter.hpp"
#include "entity/FighterBullet.hpp"
#include "entity/FighterExplosion.hpp"

ol::Fighter::Fighter(Stage& stage) : Actor(stage, FIGHTER_Z_ORDER)
{
    //ctor
    sprite_ = ol::Sprite::FIGHTER;
    setPosition(Vec2(-50, -50));
    setSize(Vec2(16, 16));
    frame_ = 0;
    isDualMode_ = false;
    isFireEnabled_ = false;
    isCaptured_ = false;
    isHitable_ = false;
    pathT_ = 0.0f;
    fireLastTime_ = 0.0f;
    formationMovementType_ = FormationMovementType::HORIZONTAL;
    doNothing();
    //log("fighter id=" + std::to_string(getId()) + " created.");
}

ol::Fighter::~Fighter()
{
    //dtor
    if (isCaptured_)
    {
        scene_.getGameController()->onCapturedFighterDestroyed();
    }
    else
    {
        scene_.getGameController()->onFighterDestroyed();
    }
    //log("fighter id=" + std::to_string(getId()) + " destroyed.");
}

ol::Collider ol::Fighter::getCollider()
{
    if (isDualMode_)
    {
        Vec2 position(position_.x - size_.x, position_.y - 0.5f * size_.y);
        Vec2 size(size_.x * 2, size_.y);
        return Collider(position, size);
    }
    else
    {
        return Collider(position_ - 0.5f * size_, size_);
    }
}

void ol::Fighter::onLeftFormation()
{
    changeZOrder(60);
}

void ol::Fighter::onReturnedToFormation()
{
    changeZOrder(50);
}

void ol::Fighter::update()
{
    Actor::update();
}

void ol::Fighter::updateDebug()
{
}

void ol::Fighter::draw(Renderer& renderer)
{
    if (isDualMode_)
    {
        drawSprite(renderer, sprite_, position_ - Vec2(8, 0), frame_);
        drawSprite(renderer, sprite_, position_ + Vec2(7, 0), frame_);
    }
    else
    {
        drawSprite(renderer, sprite_, position_, frame_);
    }

    //drawCollider(renderer);
    //path_.draw(renderer);
}

void ol::Fighter::onMessageReceived(Message message)
{
    switch (message)
    {
        case Message::FORMATION_MOVEMENT_HORIZONTAL:
            formationMovementType_ = FormationMovementType::HORIZONTAL;
            break;

        case Message::FORMATION_MOVEMENT_SCALED:
            formationMovementType_ = FormationMovementType::SCALED;
            break;

        case Message::ALL_ACTORS_DISPERSE:
            disperse();
            break;
    }
}

bool ol::Fighter::isDualMode()
{
    return isDualMode_;
}

bool ol::Fighter::isCaptured()
{
    return isCaptured_;
}

void ol::Fighter::nextLife()
{
    path_.clear();
    path_.addPoints(PATH_FIGHTER_NEXT_LIFE);
    pathT_ = 0.0f;
    isLastStateFinished_ = false;
    isDiving_ = false;
    setUpdateAction([=] () {
        position_ = path_.advance(pathT_, 2.0f);
        updateFrame();
        if (!isLastStateFinished_ && path_.isFinished(pathT_))
        {
            isLastStateFinished_ = true;
            doNothing();
        }
    });
}

void ol::Fighter::allowPlaying()
{
    frame_ = 6;
    position_.y = SCREEN_HEIGHT - size_.y * 2;
    isLastStateFinished_ = false;
    isDiving_ = false;
    setUpdateAction([=] () {
        if (isKeyPressed(Key::LEFT))
        {
            position_.x -= 2;
        }
        else if (isKeyPressed(Key::RIGHT))
        {
            position_.x += 2;
        }

        if (isFireEnabled_ && isKeyPressedOnce(Key::FIRE))
        {
            // limit number of bullets player can fire
            int bulletsCount = scene_.getGameController()->getFighterBulletsCount();
            if (getGameElapsedTime() - fireLastTime_ > 0.15f &&
                (!isDualMode_ && bulletsCount < 2) ||
                (isDualMode_ && bulletsCount < 4) )
            {
                fire();
            }
        }

        // limit horizontal movement
        int k = isDualMode_ ? 16 : 8;
        position_.x = position_.x < k ? k : position_.x;
        position_.x = position_.x > SCREEN_WIDTH - k ?
                      SCREEN_WIDTH - k : position_.x;
    });
}

void ol::Fighter::fire()
{
    if (isDualMode_)
    {
        scene_.spawn<FighterBullet>()->setPosition(position_ - Vec2(7.0f, 12.0f));
        scene_.spawn<FighterBullet>()->setPosition(position_ - Vec2(-8.0f, 12.0f));
    }
    else
    {
        scene_.spawn<FighterBullet>()->setPosition(position_ - Vec2(0.0f, 12.0f));
    }
    fireLastTime_ = getGameElapsedTime();
}

void ol::Fighter::getsPulledByBeam(Vec2 enemyPosition)
{
    path_.clear();
    path_.addPoint(position_);
    path_.addPoint(Vec2(enemyPosition.x, position_.y - 38));
    path_.addPoint(Vec2(enemyPosition.x, position_.y - 78));
    pathT_ = 0.0f;
    isHitable_ = false;
    isLastStateFinished_ = false;
    isDiving_ = false;
    setUpdateAction([=] ()
    {
        position_ = path_.advance(pathT_, 1.25f);
        float f = frame_ + 0.75f;
        frame_ = f > 23 ? 0 : f; // keep rotating

        if (!isLastStateFinished_ && path_.isFinished(pathT_) &&
            static_cast<int>(frame_) == 7)
        {
            isLastStateFinished_ = true;
        }
    });
}

void ol::Fighter::getsCapturedByEnemy()
{
    // note: now position_ & frame_ is controlled by enemy
    isCaptured_ = true;
    sprite_ = Sprite::FIGHTER_CAPTURED;
    isLastStateFinished_ = false;
    isDiving_ = false;
    setUpdateAction([=] ()
    {
        float f = frame_ + 0.75f;
        frame_ = f > 23 ? 0 : f; // keep rotating
        if (!isLastStateFinished_ && static_cast<int>(frame_) == 6)
        {
            isLastStateFinished_ = true;
            doNothing();
        }
    });
}

void ol::Fighter::rescued()
{
    // note: now position_ & frame_ is controlled by enemy
    sprite_ = Sprite::FIGHTER;
    lastStateTime_ = getGameElapsedTime();
    isLastStateFinished_ = false;
    isDiving_ = false;
    setUpdateAction([=] ()
    {
        float f = frame_ + 0.75f;
        frame_ = f > 23 ? 0 : f; // keep rotating
        if (!isLastStateFinished_ && static_cast<int>(frame_) == 6 &&
            getGameElapsedTime() - lastStateTime_ > 2.0f)
        {
            isLastStateFinished_ = true;
            doNothing();
        }
    });
}

void ol::Fighter::startDualMode()
{
    frame_ = 6;
    path_.clear();
    path_.addPoint(position_);
    if (isCaptured_)
    {
        path_.addPoint(Vec2(119, position_.y));
        path_.addPoint(Vec2(119, 256));
    }
    else
    {
        path_.addPoint(Vec2(104, 256));
    }
    pathT_ = 0.0f;
    isLastStateFinished_ = false;
    isDiving_ = false;
    setUpdateAction([=] ()
    {
        position_ = path_.advance(pathT_, 1.0f);
        if (!isLastStateFinished_ && path_.isFinished(pathT_))
        {
            isLastStateFinished_ = true;
            doNothing();
        }
    });
}

void ol::Fighter::commitDualMode()
{
    if (isCaptured_)
    {
        destroy();
    }
    else
    {
        isDualMode_ = true;
        position_.x += 8;
    }
}

void ol::Fighter::hit(Collider collider)
{
    if (isDualMode_)
    {
        Collider left(position_ - Vec2(16.0f, 8.0f), size_);
        if (left.collides(collider))
        {
            isDualMode_ = false;
            Vec2 p = position_ - Vec2(8.0f, 0.0f);
            scene_.spawn<FighterExplosion>()->setPosition(p);
            position_.x += 7;
            return;
        }

        Collider right(position_ - Vec2(0, 8.0f), size_);
        if (right.collides(collider))
        {
            isDualMode_ = false;
            Vec2 p = position_ + Vec2(7.0f, 0.0f);
            scene_.spawn<FighterExplosion>()->setPosition(p);
            position_.x -= 8;
            return;
        }
    }
    else
    {
        explode();
    }
}

void ol::Fighter::explode()
{
    scene_.spawn<FighterExplosion>()->setPosition(position_);
    destroy();
}

void ol::Fighter::onDivingStarted()
{
    log("ol::Fighter::onDivingStarted()");
}

void ol::Fighter::onDivingFinished()
{
    log("ol::Fighter::onDivingFinished()");
}
