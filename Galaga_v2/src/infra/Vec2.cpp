#include <cmath>
#include "infra/Vec2.hpp"

ol::Vec2::Vec2()
{
    //ctor
}

ol::Vec2::~Vec2()
{
    //dtor
}

ol::Vec2::Vec2(float xs, float ys)
{
    x = xs;
    y = ys;
}

ol::Vec2 ol::Vec2::rotate(float angle)
{
    Vec2 nv;
    float c = cos(angle);
    float s = sin(angle);
    nv.x = x * c - y * s;
    nv.y = x * s + y * c;
    return nv;
}

float ol::Vec2::getLength()
{
    return sqrt(x * x + y * y);
}

ol::Vec2 ol::Vec2::normalize()
{
    Vec2 vr(x, y);
    float length = getLength();
    if (length != 0)
    {
        vr = (1 / length) * *this;
    }
    return vr;
}

namespace ol
{
    std::ostream& operator << (std::ostream& os, ol::Vec2 const& v)
    {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }
}

ol::Vec2 ol::Vec2::operator + (const Vec2 v)
{
    return ol::Vec2(x + v.x, y + v.y);
}

ol::Vec2 ol::Vec2::operator - (const Vec2 v)
{
    return ol::Vec2(x - v.x, y - v.y);
}

ol::Vec2 ol::operator * (float s, const ol::Vec2 v)
{
    return ol::Vec2(s * v.x, s * v.y);
}

ol::Vec2 ol::operator * (const ol::Vec2 v, float s)
{
    return ol::Vec2(s * v.x, s * v.y);
}
