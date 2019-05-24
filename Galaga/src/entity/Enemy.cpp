#include <cmath>
#include "infra/Keyboard.hpp"
#include "infra/Display.hpp"
#include "entity/GameController.hpp"
#include "entity/ScorePoint.hpp"
#include "entity/Enemy.hpp"
#include "entity/EnemyBeam.hpp"
#include "entity/EnemyExplosion.hpp"

std::vector<std::vector<ol::EnemyInfo>> ol::enemiesGroup
{
    { { 1, 4, 2, 0 }, { 1, 5, 2, 1 }, { 1, 4, 3, 2 }, { 1, 5, 3, 3 },
      { 2, 4, 4, 0 }, { 2, 5, 4, 1 }, { 2, 4, 5, 2 }, { 2, 5, 5, 3 } },

    { { 3, 3, 2, 0 }, { 3, 3, 3, 2 }, { 3, 6, 2, 4 }, { 3, 6, 3, 6 },
      { 3, 3, 1, 1 }, { 3, 4, 1, 3 }, { 3, 5, 1, 5 }, { 3, 6, 1, 7 } },

    { { 4, 7, 2, 0 }, { 4, 2, 2, 1 }, { 4, 8, 2, 2 }, { 4, 1, 2, 3 },
      { 4, 7, 3, 4 }, { 4, 2, 3, 5 }, { 4, 8, 3, 6 }, { 4, 1, 3, 7 } },

    { { 2, 6, 4, 0 }, { 2, 3, 4, 1 }, { 2, 7, 4, 2 }, { 2, 2, 4, 3 },
      { 2, 6, 5, 4 }, { 2, 3, 5, 5 }, { 2, 7, 5, 6 }, { 2, 2, 5, 7 } },

    { { 1, 8, 4, 0 }, { 1, 1, 4, 1 }, { 1, 9, 4, 2 }, { 1, 0, 5, 3 },
      { 1, 8, 5, 4 }, { 1, 1, 5, 5 }, { 1, 9, 5, 6 }, { 1, 0, 4, 7 } },
};

std::map<int, ol::Sprite> ol::Enemy::rowEnemyMap
{
    {  1, Sprite::ENEMY_1 },
    {  2, Sprite::ENEMY_2 },
    {  3, Sprite::ENEMY_2 },
    {  4, Sprite::ENEMY_3 },
    {  5, Sprite::ENEMY_3 },
    {  6, Sprite::ENEMY_4 },
    {  7, Sprite::ENEMY_5 },
    {  8, Sprite::ENEMY_6 },
    {  9, Sprite::ENEMY_7 },
    { 10, Sprite::ENEMY_8 },
    { 11, Sprite::ENEMY_9 }
};

ol::Enemy::Enemy(Stage& stage) : Actor(stage, ENEMY_Z_ORDER)
{
    //ctor
    sprite_ = Sprite::ENEMY_1;
    setSize(Vec2(16, 16));
    frame_ = 6.0f;
    isFighterCaptured_ = false;
    isFireEnabled_ = true;
    availableBulletsCount_ = 0;
    life_ = 1;
    scorePoint_ = 0;
    doNothing();
    scene_.getGameController()->onEnemyCreated(this);
}

ol::Enemy::~Enemy()
{
    //dtor
    scene_.getGameController()->onEnemyDestroyed(this);
}

bool ol::Enemy::isBoss()
{
    return formationRow_ == 1;
}

bool ol::Enemy::isGuard()
{
    return formationRow_ == 2 || formationRow_ == 3;
}

bool ol::Enemy::isMinion()
{
    return formationRow_ == 4 || formationRow_ == 5;
}

bool ol::Enemy::isSpecial()
{
    return formationRow_ > 5;
}

bool ol::Enemy::isFighterCaptured()
{
    return isFighterCaptured_;
}

void ol::Enemy::setFighterCaptured(bool isFighterCaptured)
{
    isFighterCaptured_ = isFighterCaptured;
}

int ol::Enemy::getLife()
{
    return life_;
}

void ol::Enemy::setLife(int life)
{
    life_ = life;
}

int ol::Enemy::getScorePoint()
{
    return scorePoint_;
}

void ol::Enemy::setScorePoint(int scorePoint)
{
    scorePoint_ = scorePoint;
}

void ol::Enemy::update()
{
    Actor::update();
    checkCollisions();
    decideWhenFire();
}

void ol::Enemy::checkCollisions()
{
    GameController* gameController = scene_.getGameController();
    if (!gameController->isFighterDestroyed() && !isDestroyed_)
    {
        Fighter* fighter = gameController->getFighter();
        if (fighter->isHitable() && !fighter->isCaptured() &&
            fighter->getCollider().collides(getCollider()))
        {
            gameController->onFighterHit(this);
            gameController->onEnemyHit(this);
        }
    }
}

void ol::Enemy::updateDebug()
{
}

void ol::Enemy::draw(Renderer& renderer)
{
    if (isBoss() && life_ <= 1)
    {
        drawSprite(renderer, Sprite::ENEMY_1_HIT, position_, frame_);
    }
    else
    {
        drawSprite(renderer, rowEnemyMap[formationRow_], position_, frame_);
    }
    //drawCollider(renderer);
    //path_.draw(renderer);
}

void ol::Enemy::onMessageReceived(Message message)
{
    switch (message)
    {
        case Message::ALL_ENEMIES_START_FIRING:
            setFireEnabled(true);
            break;

        case Message::ALL_ENEMIES_STOP_FIRING:
            setFireEnabled(false);
            break;

        case Message::ALL_ENEMIES_RETURN_TO_FORMATION:
            if (!hasReturnedToFormation())
            {
                returnToFormation();
            }
            break;

        case Message::ALL_ACTORS_DISPERSE:
            disperse();
            break;

        case Message::FORMATION_MOVEMENT_HORIZONTAL:
            formationMovementType_ = FormationMovementType::HORIZONTAL;
            break;

        case Message::FORMATION_MOVEMENT_SCALED:
            formationMovementType_ = FormationMovementType::SCALED;
            break;

    }
}

void ol::Enemy::onLeftFormation()
{
    changeZOrder(60);
    scene_.getGameController()->onEnemyLeftFormation(this);
}

void ol::Enemy::onReturnedToFormation()
{
    changeZOrder(50);
    scene_.getGameController()->onEnemyReturnedToFormation(this);
}

void ol::Enemy::onDivingStarted()
{
    availableBulletsCount_ = ol::random(0, 3);
    fireRandomTime_ = getGameElapsedTime() + ol::random(0.5f, 2.0f);
}

void ol::Enemy::onDivingFinished()
{
    if (!isBoss() && ol::random() < 0.75f)
    {
        startDiving();
    }
    else
    {
        returnToFormation();
    }
}

void ol::Enemy::tryToCaptureFighter()
{
    scene_.getGameController()->onFighterCaptureStart();

    path_.clear();
    path_.addPoint(position_);

    float angle = -6.28318f * ((frame_ / 24.0f) - 1);
    Vec2 d(30.0f * cos(angle), 30.0f * sin(angle));

    Vec2 p2 = position_ + d;
    path_.addPoint(p2);

    if (d.y < -15.0f)
    {
        float offsetx = ol::random() < 0.5f ? -25.0f : 25.0f;
        path_.addPoint(Vec2(p2.x + offsetx, p2.y));
    }

    Vec2 destinationPoint;
    destinationPoint.x = ol::random(23.0f, Display::SCREEN_WIDTH - 23.0f);
    destinationPoint.y = 159.0f;

    Vec2 direction = destinationPoint - p2;
    float distance = direction.getLength();
    int n = distance / 50;
    direction = direction.normalize();

    for (int i = 1; i < n; i++)
    {
        Vec2 p = p2 + (50 * i) * direction;
        p.x += ol::random(-50.0f, 50.0f);
        p.x = clamp(p.x, 8.0f, Display::SCREEN_WIDTH - 8.0f);

        path_.addPoint(p);
    }

    path_.addPoint(destinationPoint);
    path_.addPoint(destinationPoint + Vec2(0.0f, 1.0f));
    pathT_ = 0.0f;
    isLastStateFinished_ = false;
    setReturnedToFormation(false);
    isDiving_ = false;
    setUpdateAction([=] () {
        position_ = path_.advance(pathT_, 2.0f);
        updateFrame();
        if (!isLastStateFinished_ && path_.isFinished(pathT_))
        {
            isLastStateFinished_ = true;
            scene_.spawn<EnemyBeam>()->show(this);
            doNothing();
        }
    });
}

void ol::Enemy::decideWhenFire()
{
    if (isFireEnabled_ && isDiving_ && availableBulletsCount_ > 0 &&
        getGameElapsedTime() > fireRandomTime_ &&
        getGameElapsedTime() - fireLastTime_ > 0.2f &&
        position_.y > 75.0f)
    {
        fire();
        availableBulletsCount_--;
    }
}

void ol::Enemy::fire()
{
    scene_.spawn<EnemyBullet>()->setPosition(position_ + Vec2(0.0f, 8.0f));
    fireLastTime_ = getGameElapsedTime();
}

void ol::Enemy::hit()
{
    GameController* gameController = scene_.getGameController();
    life_--;
    if (life_ <= 0)
    {
        if (!isFighterCaptured_ ||
            (isFighterCaptured_ && gameController->isCapturedFighterDestroyed()))
        {
            explode();
        }
        // captured fighter was rescued
        else if (!gameController->isFighterDestroyed()
                 && !gameController->isCapturedFighterDestroyed())
        {
            scene_.getGameController()->onFighterRescued();
            explode();
        }

        // note: in the current implementation, if captured fighter
        //       was rescued when current playing fighter
        //       was already destroyed, then just ignore this hit
    }
}

void ol::Enemy::explode()
{
    scene_.spawn<EnemyExplosion>()->setPosition(position_);
    destroy();
}
