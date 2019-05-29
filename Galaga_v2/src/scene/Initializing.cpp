#include <iostream>
#include "infra/Util.hpp"
#include "infra/Vec2.hpp"
#include "infra/Collider.hpp"
#include "infra/Keyboard.hpp"
#include "infra/SceneManager.hpp"
#include "scene/Initializing.hpp"

ol::Initializing::Initializing(SceneManager& sceneManager) : Scene(sceneManager)
{
    //ol::log("creating Initializing Scene.");
}

ol::Initializing::~Initializing()
{
    //dtor
    //ol::log("destroying Initializing Scene.");
}

void ol::Initializing::onEnter()
{
    startTime_ = getGameElapsedTime();
}

void ol::Initializing::onExit()
{
}

void ol::Initializing::update()
{
    if (getGameElapsedTime() - startTime_ > 4.0f)
    {
        sceneManager_.changeScene(Scene::Id::OL_PRESENTS);
    }
}

void ol::Initializing::draw(Renderer& renderer)
{
}
