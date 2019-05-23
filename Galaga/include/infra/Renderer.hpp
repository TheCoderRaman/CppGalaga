#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include "infra/Vec2.hpp"
#include "infra/Resources.hpp"

namespace ol
{

    class Renderer
    {
        public:
            Renderer();
            ~Renderer();

            void setTranslation(Vec2 translation);

            void drawPoint(Vec2 position, int color);
            void drawRectangle(Vec2 position, Vec2 size, int color);
            void drawSprite(Sprite sprite, Vec2 destPosition, Vec2 srcPosition, Vec2 srcSize);
            void drawText1(std::string text, Vec2 position, int color);
            void drawText2(std::string text, Vec2 position, int color);

        protected:
            Vec2 translation_;

        private:
    };

}

#endif // RENDERER_HPP
