#include <iostream>
#include <algorithm>
#include <iterator>
#include "infra/Util.hpp"
#include "infra/Vec2.hpp"
#include "infra/Keyboard.hpp"
#include "infra/media_layer/AudioPlayer.hpp"
#include "infra/SceneManager.hpp"
#include "scene/Stage.hpp"
#include "entity/GameController.hpp"
#include "entity/ScorePoint.hpp"
#include "entity/Fighter.hpp"
#include "entity/FighterExplosion.hpp"
#include "entity/Enemy.hpp"
#include "entity/EnemyBeam.hpp"
#include "entity/EnemyExplosion.hpp"

ol::Stage::Stage(SceneManager& sceneManager) : Scene(sceneManager)
{
    //ctor
    //ol::log("creating Stage Scene.");
}

ol::Stage::~Stage()
{
    //dtor
    //ol::log("destroying Stage Scene.");
}

void ol::Stage::onEnter()
{
    // ol::playMusic(ol::Music::START_GAME);
    gameController_ = spawn<GameController>();
    gameController_->startGame(1);
    //ol::log("Stage onEnter()");
}

void ol::Stage::onExit()
{
    entities_.clear();
    entitiesAdd_.clear();
    gameController_ = nullptr;

    hudInfo.updateHiscore();
    hudInfo.score = 0;
    //ol::log("Stage onExit()");
}

ol::GameController* ol::Stage::getGameController()
{
    return gameController_;
}

static bool compareEntities(const std::shared_ptr<ol::Entity<ol::Stage>>& le,
                            const std::shared_ptr<ol::Entity<ol::Stage>>& re)
{
    if (le->getZOrder() == re->getZOrder())
    {
        return le->getId() < re->getId();
    }
    else
    {
        return le->getZOrder() < re->getZOrder();
    }
}

void ol::Stage::update()
{
    std::list<std::shared_ptr<Entity<Stage>>>::iterator it;
    for (it = entities_.begin(); it != entities_.end(); )
    {
        Entity<Stage>& entity = **it;

        // std::cout << "updating " << typeid(entity).name() << std::endl;

        entity.update();
        entity.updateDebug();
        if (entity.isDestroyed()) {
            it = entities_.erase(it);
        }
        else if (entity.isZOrderChanged())
        {
            entitiesAdd_.push_back(*it);
            it = entities_.erase(it);
            entity.commitZOrderChange();
        }
        else
        {
            it++;
        }
    }

    if (!entitiesAdd_.empty())
    {
        for (auto entity : entitiesAdd_)
        {
            entities_.insert(std::upper_bound(entities_.begin(),
                entities_.end(), entity, compareEntities), entity);
        }
        entitiesAdd_.clear();
    }
}

void ol::Stage::draw(Renderer& renderer)
{
    for (auto entity : entities_)
    {
        entity->draw(renderer);
    }
}

void ol::Stage::broadcastMessage(Message message)
{
    for (auto entity : entities_)
    {
        entity->onMessageReceived(message);
    }
}


