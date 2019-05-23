#include "infra/SceneManager.hpp"
#include "scene/Initializing.hpp"
#include "scene/OlPresents.hpp"
#include "scene/Title.hpp"
#include "scene/Stage.hpp"

ol::SceneManager::SceneManager()
{
    //ctor
    currentScene_ = nullptr;
    nextScene_ = nullptr;
    createAllScenes();
}

ol::SceneManager::~SceneManager()
{
    //dtor
}

ol::StarsBackground& ol::SceneManager::getStarsBackground()
{
    return starsBackground_;
}

ol::Hud& ol::SceneManager::getHud()
{
    return hud_;
}

void ol::SceneManager::createAllScenes()
{
    scenes_.insert(std::make_pair(Scene::Id::INITIALIZING,
                                  std::unique_ptr<Initializing>(new Initializing(*this))));

    scenes_.insert(std::make_pair(Scene::Id::OL_PRESENTS,
                                  std::unique_ptr<OlPresents>(new OlPresents(*this))));

    scenes_.insert(std::make_pair(Scene::Id::TITLE,
                                  std::unique_ptr<Title>(new Title(*this))));

    scenes_.insert(std::make_pair(Scene::Id::STAGE,
                                  std::unique_ptr<Stage>(new Stage(*this))));

    changeScene(Scene::Id::INITIALIZING);
}

void ol::SceneManager::update()
{
    starsBackground_.update();

    if (nextScene_ != nullptr)
    {
        if (currentScene_ != nullptr)
        {
            currentScene_->onExit();
        }
        currentScene_ = nextScene_;
        nextScene_ = nullptr;
        currentScene_->onEnter();
    }

    if (currentScene_ != nullptr)
    {
        currentScene_->update();
    }

    hud_.update();
}

void ol::SceneManager::draw(Renderer& renderer)
{
    starsBackground_.draw(renderer);

    if (currentScene_ != nullptr)
    {
        currentScene_->draw(renderer);
    }

    hud_.draw(renderer);
}

void ol::SceneManager::changeScene(Scene::Id sceneId)
{
    nextScene_ = scenes_[sceneId].get();
}
