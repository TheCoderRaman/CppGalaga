#ifndef FIGHTER_HPP
#define FIGHTER_HPP

#define FIGHTER_Z_ORDER 10

#include "entity/Actor.hpp"

namespace ol
{

    class Fighter : public Actor
    {
        public:
            Fighter(Stage& scene);
            virtual ~Fighter();

            Collider getCollider();

            void onLeftFormation();
            void onReturnedToFormation();

            void update();
            void updateDebug();

            void draw(Renderer& renderer);

            void onMessageReceived(Message message);

            bool isDualMode();

            bool isCaptured();

            void nextLife();
            void allowPlaying();
            void fire();
            void getsPulledByBeam(Vec2 enemyPosition);
            void getsCapturedByEnemy();
            void rescued();
            void startDualMode();
            void commitDualMode();
            void hit(Collider collider);
            void explode();

        protected:

        private:
            bool isDualMode_;
            bool isCaptured_;
    };

}

#endif // FIGHTER_HPP
