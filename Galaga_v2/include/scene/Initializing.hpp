#ifndef INITIALIZING_HPP
#define INITIALIZING_HPP

#include "infra/Path.hpp"
#include "infra/Scene.hpp"

namespace ol
{

    class Initializing : public Scene
    {
        public:
            Initializing(SceneManager& sceneManager);
            virtual ~Initializing();

            void onEnter();
            void onExit();

            void update();
            void draw(Renderer& renderer);

        protected:

        private:
            float startTime_;

    };

}

#endif // INITIALIZING_HPP
