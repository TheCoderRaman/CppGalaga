#include <cmath>
#include "infra/Util.hpp"
#include "entity/Actor.hpp"

ol::Actor::Actor(Stage& scene, int zOrder) : Entity<Stage>(scene, zOrder)
{
    //ctor
    randomAngle_ = ol::random(0.0f, 6.28318f);
    sprite_ = Sprite::EMPTY;
    position_ = Vec2(-50.0f, -50.0f);
    Vec2 size_ = Vec2(16.0f, 16.0f);
    frame_ = 0;
    angleTransitionFactor_ = 0;
    lastStateTime_ = 0;
    isLastStateFinished_ = false;
    isFireEnabled_ = false;
    isHitable_ = true;
    formationMovementType_ = FormationMovementType::HORIZONTAL;
    formationMovementTransitionFactor_ = 0;
    formationCol_ = 1;
    formationRow_ = 1;
    hasReturnedToFormation_ = false;
    pathT_ = 0.0f;
    path_.clear();
    path_.addPoints(PATH_EMPTY);
    followActor_ = nullptr;
    doNothing();
}

ol::Actor::~Actor()
{
    //dtor
}

ol::GameController* ol::Actor::getGameController()
{
    return scene_.getGameController();
}

void ol::Actor::setFormationPosition(int col, int row)
{
    formationCol_ = col;
    formationRow_ = row;
}

ol::Actor::FormationMovementType ol::Actor::getFormationMovementType()
{
    return formationMovementType_;
}

void ol::Actor::setFormationMovementType(FormationMovementType formationMovementType)
{
    formationMovementType_ = formationMovementType;
}

int ol::Actor::getFormationCol()
{
    return formationCol_;
}

int ol::Actor::getFormationRow()
{
    return formationRow_;
}

ol::Vec2& ol::Actor::getFormationScreenPosition()
{
    return formationScreenPosition_;
}

bool ol::Actor::isLastStateFinished()
{
    return isLastStateFinished_;
}

bool ol::Actor::isFireEnabled()
{
    return isFireEnabled_;
}

void ol::Actor::setFireEnabled(bool isFireEnabled)
{
    isFireEnabled_ = isFireEnabled;
}

bool ol::Actor::isDiving()
{
    return isDiving_;
}

bool ol::Actor::hasReturnedToFormation()
{
    return hasReturnedToFormation_;
}

void ol::Actor::setReturnedToFormation(bool hasReturnedToFormation)
{
    if (hasReturnedToFormation_ != hasReturnedToFormation)
    {
        if (hasReturnedToFormation)
        {
            onReturnedToFormation();
        }
        else
        {
            onLeftFormation();
        }
        hasReturnedToFormation_ = hasReturnedToFormation;
    }
}

void ol::Actor::draw(Renderer& renderer)
{
    Entity::draw(renderer);
}

void ol::Actor::update()
{
    updateFormationScreenPosition();
    Entity::update();
}

void ol::Actor::updateDebug()
{
}

void ol::Actor::onMessageReceived(Message message)
{
}

void ol::Actor::updateFrame()
{
    if (path_.isFinished(pathT_))
    {
        angleTransitionFactor_ = clamp(angleTransitionFactor_ + 0.015f, 0.0f, 1.0f);
    }
    else {
        angleTransitionFactor_ = 0.0f;
    }
    float angle = 0.0f;
    Vec2 tangentVector = path_.getTangent(pathT_);
    float movingAngle = atan2(tangentVector.y, tangentVector.x) - 0.1309f;

    float stationalAngle = -1.5708f + 0.5f *
                           sin(randomAngle_ + getGameElapsedTime() * 2.0f);

    angle = angleTransitionFactor_ * stationalAngle +
            (1.0f - angleTransitionFactor_) * movingAngle;

    angle = normalizeAngle(angle);
    frame_ = (1.0f - (angle / (2.0f * 3.14159f))) * 24.0f;
}

void ol::Actor::updateFormationScreenPosition()
{
    // update formationMovementTransitionFactor_
    float sign = formationMovementType_ == FormationMovementType::SCALED ? 1 : -1;

    formationMovementTransitionFactor_ = clamp(
        formationMovementTransitionFactor_ + sign * 0.005f, 0.0f, 1.0f);

    Vec2 pointHorizontal, pointScaled;

    float s = sin(getGameElapsedTime() * 3.0f);

    // horizontal movement
    pointHorizontal.x = 31 + formationCol_ * 18 + 18 * s;
    pointHorizontal.y = 25 + formationRow_ * 18;

    // scaled from 'center' movement
    pointScaled.x = 31 + formationCol_ * 18 - 112;
    pointScaled.y = 25 + formationRow_ * 18 - 43;
    float scale = 1.13f + 0.13f * s;
    pointScaled.x *= scale;
    pointScaled.y *= formationRow_ == 0 ? 1.0f : scale;
    pointScaled.x += 112;
    pointScaled.y += 43;

    // result position
    formationScreenPosition_ = pointHorizontal +
                               formationMovementTransitionFactor_ *
                               (pointScaled - pointHorizontal);
}

void ol::Actor::onLeftFormation()
{
}

void ol::Actor::onReturnedToFormation()
{
}

void ol::Actor::move(std::vector<Vec2> pathPoints, int advance)
{
    path_.clear();
    path_.addPoints(pathPoints);
    pathT_ = 0.0f;

    // advance
    for (int i = 0; i < advance; i++) {
        position_ = path_.advance(pathT_, 1.0f);
    }
    isLastStateFinished_ = false;
    setReturnedToFormation(false);
    setUpdateAction([=] () {
        position_ = path_.advance(pathT_, 2.5f);
        updateFrame();

        if (!isLastStateFinished_ && path_.isFinished(pathT_))
        {
            isLastStateFinished_ = true;
            destroy();
        }
    });
}

void ol::Actor::moveToFormation(std::vector<Vec2> pathPoints,
                                int formationCol, int formationRow, int advance,
                                bool diveInTheMiddleOfPath)
{
    formationCol_ = formationCol;
    formationRow_ = formationRow;

    path_.clear();
    path_.addPoints(pathPoints);
    pathT_ = 0.0f;

    // advance
    for (int i = 0; i < advance; i++) {
        position_ = path_.advance(pathT_, 1.0f);
    }
    float diveMoment = ol::random(0.55f, 0.75f);
    isLastStateFinished_ = false;
    setReturnedToFormation(false);
    isDiving_ = false;
    setUpdateAction([=] () {
        position_ = path_.advance(pathT_, 2.5f);
        updateFrame();

        Vec2& pathLastPoint = path_.getLastPoint();
        pathLastPoint = formationScreenPosition_;

        if (diveInTheMiddleOfPath && pathT_ >=
            path_.getPoints().size() * diveMoment)
        {
            startDiving();
        }
        else if (!isLastStateFinished_ && path_.isFinished(pathT_))
        {
            isLastStateFinished_ = true;
            setReturnedToFormation(true);
        }
    });
}

void ol::Actor::startDiving(bool returnIntheMiddleOfPath)
{
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
    destinationPoint.x = ol::random(8.0f, SCREEN_WIDTH - 8.0f);
    destinationPoint.y = SCREEN_HEIGHT + 32.0f;

    Vec2 direction = destinationPoint - p2;
    float distance = direction.getLength();
    int n = distance / 50;
    direction = direction.normalize();

    bool includeLoop = ol::random() < 0.4f;
    int loopN = ol::random(1, n - 1);
    float loopSize = ol::random(20.0f, 50.0f);

    for (int i = 1; i < n; i++)
    {
        Vec2 p = p2 + (50 * i) * direction;
        p.x += ol::random(-50.0f, 50.0f);
        p.x = clamp(p.x, 8.0f, SCREEN_WIDTH - 8.0f);

        path_.addPoint(p);

        if (includeLoop && loopN == i)
        {
            includeLoop = false;
            addPathLoopPoints(path_, loopSize);
        }
    }

    path_.addPoint(destinationPoint);
    pathT_ = 0.0f;
    float returnMoment = ol::random(0.55f, 0.75f);
    isLastStateFinished_ = false;
    setReturnedToFormation(false);
    isDiving_ = true;
    divingStartTime_ = getGameElapsedTime();
    onDivingStarted();
    setUpdateAction([=] () {
        position_ = path_.advance(pathT_, 2.0f);
        updateFrame();
        if (returnIntheMiddleOfPath &&
            pathT_ >= path_.getPoints().size() * returnMoment)
        {
            returnToFormation();
        }
        else if (!isLastStateFinished_ && path_.isFinished(pathT_))
        {
            isLastStateFinished_ = true;
            isDiving_ = false;
            position_.y = -100.0f;
            onDivingFinished();
        }
    });
}

void ol::Actor::onDivingStarted()
{
}

void ol::Actor::onDivingFinished()
{
    if (ol::random() < 0.75f)
    {
        startDiving();
    }
    else
    {
        returnToFormation();
    }
}

void ol::Actor::returnToFormation()
{
    path_.clear();
    path_.addPoint(position_);

    float angle = -6.28318f * ((frame_ / 24.0f) - 1);
    Vec2 d(30.0f * cos(angle), 30.0f * sin(angle));

    Vec2 p2 = position_ + d;
    path_.addPoint(p2);

    if (d.y > 15.0f)
    {
        float sign = ol::random() < 0.5f ? -1.0f : 1.0f;
        float offsetx = sign * ol::random(30.0f, 60.0f);
        float f = ol::random(20.0f, 40.0f);
        path_.addPoint(Vec2(p2.x + offsetx, p2.y + f));
        path_.addPoint(Vec2(p2.x + 2 * offsetx, p2.y));
    }

    Vec2 destinationPoint;
    destinationPoint.x = 31 + formationCol_ * 18;
    destinationPoint.y = 25 + formationRow_ * 18;

    Vec2 direction = destinationPoint - p2;
    float distance = direction.getLength();
    int n = distance / 50;
    direction = direction.normalize();

    for (int i = 1; i < n; i++)
    {
        Vec2 p = p2 + (50 * i) * direction;
        p.x += ol::random(-10.0f, 10.0f);
        p.y += ol::random(-10.0f, 10.0f);
        path_.addPoint(p);
    }

    path_.addPoint(destinationPoint);
    pathT_ = 0.0f;
    isLastStateFinished_ = false;
    setReturnedToFormation(false);
    isDiving_ = false;
    setUpdateAction([=] () {
        position_ = path_.advance(pathT_, 2.0f);
        updateFrame();

        Vec2& pathLastPoint = path_.getLastPoint();
        pathLastPoint = formationScreenPosition_;

        if (!isLastStateFinished_ && path_.isFinished(pathT_))
        {
            isLastStateFinished_ = true;
            setReturnedToFormation(true);
        }
    });
}

void ol::Actor::disperse()
{
    path_.clear();
    path_.addPoint(position_);

    float angle = -6.28318f * ((frame_ / 24.0f) - 1);
    Vec2 d(75.0f * cos(angle), 75.0f * sin(angle));

    Vec2 p2 = position_ + d;
    path_.addPoint(p2);

    Vec2 destinationPoint(600.0f, 0.0f);
    float f = ol::random(-0.5f, 0.5f);
    destinationPoint = destinationPoint.rotate(angle + f);

    Vec2 direction = destinationPoint - p2;
    float distance = direction.getLength();
    int n = distance / 50;
    direction = direction.normalize();
    for (int i = 1; i < n; i++)
    {
        Vec2 p = p2 + (50 * i) * direction;
        p.x += ol::random(-20.0f, 20.0f);
        p.y += ol::random(-20.0f, 20.0f);
        path_.addPoint(p);
    }
    path_.addPoint(destinationPoint);
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
            doNothing();
        }
    });
}

void ol::Actor::follow(Actor* followActor, bool untilFormation)
{
    if (followActor == nullptr)
    {
        error("followActor can't be nullptr ol::Actor::follow(Actor* followActor) method !");
    }
    followActor_ = followActor;
    int followActorId = followActor->getId();
    pathT_ = 0.0f;
    path_.clear();
    path_.addPoint(Vec2(0.0f, 0.0f));
    isLastStateFinished_ = false;
    setReturnedToFormation(false);
    setUpdateAction([=] () {

        // followActor_ was destroyed ? -> then startDiving()
        if (!scene_.isEntityAlive(followActorId))
        {
            startDiving(ol::random() < 0.5f);
            return;
        }

        if (untilFormation && followActor_->hasReturnedToFormation())
        {
            followActor_ = nullptr;
            float angle = -6.28318f * ((frame_ / 24.0f) - 1);
            Vec2 d(1.0f * cos(angle), 1.0f * sin(angle));
            std::vector<Vec2> path { position_, position_ - d };
            moveToFormation(path, formationCol_, formationRow_, 0.0f);
        }
        else
        {
            Vec2 offset = getFormationScreenPosition() -
                          followActor_->getFormationScreenPosition();

            position_ = followActor_->position_ + offset;
            frame_ = followActor_->getFrame();
            isDiving_ = followActor_->isDiving();
        }
    });
}

void ol::Actor::explode()
{
}

void ol::Actor::fire()
{
}
