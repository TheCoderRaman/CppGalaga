#ifndef ENEMYBEAM_HPP
#define ENEMYBEAM_HPP

#define ENEMY_BEAM_Z_ORDER 25

#include "scene/Stage.hpp"
#include "entity/Enemy.hpp"
#include "entity/Fighter.hpp"

namespace ol
{

    class EnemyBeam : public Entity<Stage>
    {
        public:
            EnemyBeam(Stage& scene);
            virtual ~EnemyBeam();

            float getHeight();
            Collider getCollider();
            bool isFinished();

            Enemy* getEnemy();
            Fighter* getCapturedFighter();

            void update();
            void draw(Renderer& render);

            void show(Enemy* enemy);

        protected:

        private:
            void updateAnimation();
            void checkFighterCaptured();
            void wait();
            void hide();

        private:
            Enemy* enemy_;
            Fighter* capturedFighter_;
            float height_;
            float showTime_;
            bool isFinished_;
    };

}

#endif // ENEMYBEAM_HPP
