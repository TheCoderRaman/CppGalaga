#ifndef FIGHTERBULLET_HPP
#define FIGHTERBULLET_HPP

#define FIGHTER_BULLET_Z_ORDER 40

#include "scene/Stage.hpp"

namespace ol
{

    class FighterBullet : public Entity<Stage>
    {
        public:
            FighterBullet(Stage& scene);
            virtual ~FighterBullet();

            void update();

            void hit();

        protected:
            void checkCollisions();

        private:
    };

}

#endif // FIGHTERBULLET_HPP
