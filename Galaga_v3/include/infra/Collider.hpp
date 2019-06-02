#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "infra/Vec2.hpp"

namespace ol
{

    class Collider
    {
        public:
            Collider();
            Collider(Vec2 p, Vec2 s);
            virtual ~Collider();

            bool collides(const Collider& c);

        public:
            Vec2 position;
            Vec2 size;

        protected:

        private:
    };

}

#endif // COLLIDER_HPP
