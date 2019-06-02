#ifndef VEC2_HPP
#define VEC2_HPP

#include <iostream>

namespace ol
{

    class Vec2
    {
        public:
            Vec2();
            Vec2(float x, float y);
            ~Vec2();

            Vec2 rotate(float angle);
            Vec2 normalize();
            float getLength();

            friend std::ostream& operator << (std::ostream& os, Vec2 const& v);

            Vec2 operator + (const Vec2 v);
            Vec2 operator - (const Vec2 v);

        public:
            float x;
            float y;
    };

    Vec2 operator * (float s, const Vec2 v);
    Vec2 operator * (const Vec2 v, float s);

}

#endif // VEC2_HPP
