#ifndef FIGHTEREXPLOSION_HPP
#define FIGHTEREXPLOSION_HPP

#define FIGHTER_EXPLOSION_Z_ORDER 20

#include "scene/Stage.hpp"

namespace ol
{

    class FighterExplosion : public Entity<Stage>
    {
        public:
            FighterExplosion(Stage& scene);
            virtual ~FighterExplosion();

            void update();

        protected:

        private:
    };

}

#endif // FIGHTEREXPLOSION_HPP
