#ifndef OLPRESENTS_HPP
#define OLPRESENTS_HPP

#include "infra/Path.hpp"
#include "infra/Scene.hpp"

namespace ol
{
    struct Letter
    {
        std::string character;
        Path path;
        float pathT;
    };

    class OlPresents : public Scene
    {
        public:
            OlPresents(SceneManager& sceneManager);
            virtual ~OlPresents();

            void onEnter();
            void onExit();

            void update();
            void draw(Renderer& renderer);

        protected:

        private:

            static const std::string OL_PRESENTS_TEXT;
            static const std::string YT_1K_TEXT_1;
            static const std::string YT_1K_TEXT_2;

            bool isYt1KTextVisible_;

            bool isLettersVisible_;
            bool isLettersPathFinished_;
            std::vector<Letter> letters_;
            float lettersFinishedTime_;
    };

}

#endif // OLPRESENTS_HPP
