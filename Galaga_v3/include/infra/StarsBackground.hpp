#ifndef STARSBACKGROUND_HPP
#define STARSBACKGROUND_HPP

#include "infra/Vec2.hpp"
#include "infra/media_layer/Renderer.hpp"

#define STARS_COUNT 100

namespace ol
{

    struct Star
    {
        int color;
        Vec2 position;
        float yVelocity;
        bool blink;
        float blinkTime;
    };

    class StarsBackground final
    {
        public:
            StarsBackground();
            ~StarsBackground();

            void update();
            void draw(Renderer &renderer);

            void move();
            void stop();

        protected:

        private:
            void createStars();

        private:
            Star stars_[STARS_COUNT];
            float scaledVelocityY_;
            float targetScaledVelocityY_;
    };

}

#endif // STARSBACKGROUND_HPP
