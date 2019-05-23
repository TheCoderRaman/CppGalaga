#ifndef ENEMYEXPLOSION_HPP
#define ENEMYEXPLOSION_HPP

#define ENEMY_EXPLOSION_Z_ORDER 30

#include "scene/Stage.hpp"

namespace ol
{

    class EnemyExplosion : public Entity<Stage>
    {
        public:
            EnemyExplosion(Stage& scene);
            virtual ~EnemyExplosion();

            void update();

        protected:

        private:
    };

}

#endif // ENEMYEXPLOSION_HPP
