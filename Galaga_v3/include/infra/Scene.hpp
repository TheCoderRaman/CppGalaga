#ifndef SCENE_HPP
#define SCENE_HPP

#include "infra/media_layer/Renderer.hpp"

namespace ol
{

    class SceneManager;

    class Scene
    {
        public:

            enum class Id {
                INITIALIZING,
                OL_PRESENTS,
                TITLE,
                STAGE
            };

            Scene(SceneManager& sceneManager) : sceneManager_(sceneManager) { }
            virtual ~Scene() { }

            virtual SceneManager& getSceneManager()
            {
                return sceneManager_;
            }

            virtual void onEnter() = 0;
            virtual void onExit() = 0;

            virtual void update() = 0;
            virtual void draw(Renderer& renderer) = 0;

        protected:
            SceneManager& sceneManager_;

        private:

    };

}

#endif // SCENE_HPP
