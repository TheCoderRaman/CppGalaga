#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <memory>
#include <map>
#include "infra/media_layer/Renderer.hpp"
#include "infra/StarsBackground.hpp"
#include "infra/Hud.hpp"
#include "infra/Scene.hpp"

namespace ol
{

    class SceneManager final
    {
        public:
            SceneManager();
            ~SceneManager();

            StarsBackground& getStarsBackground();
            Hud& getHud();

            void update();
            void draw(Renderer& renderer);
            void changeScene(Scene::Id sceneId);

        protected:

        private:

            void createAllScenes();

            StarsBackground starsBackground_;
            Hud hud_;

            std::map<Scene::Id, std::unique_ptr<Scene>> scenes_;
            Scene* currentScene_;
            Scene* nextScene_;
    };

}


#endif // SCENEMANAGER_HPP
