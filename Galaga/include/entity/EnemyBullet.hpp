#ifndef ENEMYBULLET_HPP
#define ENEMYBULLET_HPP

#define ENEMY_BULLET_Z_ORDER 70

#include "scene/Stage.hpp"

namespace ol
{

    class EnemyBullet : public Entity<Stage>
    {
        public:
            EnemyBullet(Stage& scene);
            virtual ~EnemyBullet();

            void update();

            void hit();

        protected:
            void checkCollisions();

        private:
            float vx_;
    };

}

#endif // ENEMYBULLET_HPP
