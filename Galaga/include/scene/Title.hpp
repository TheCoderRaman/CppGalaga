#ifndef TITLE_HPP
#define TITLE_HPP

#include "infra/Vec2.hpp"
#include "infra/Scene.hpp"

namespace ol
{

    class Title : public Scene
    {
        public:
            Title(SceneManager& sceneManager);
            virtual ~Title();

            void onEnter();
            void onExit();

            void update();
            void draw(Renderer& renderer);

        protected:

        private:
            float startTime_;
            Vec2 translation_;

            float titleShineFrame_;

            static const std::string CREDIT_TEXT_1;
            static const std::string CREDIT_TEXT_2;
            static const std::string START_TEXT_2;
            bool isStartTextVisible_;
    };

}

#endif // TITLE_HPP
