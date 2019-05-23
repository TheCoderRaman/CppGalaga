#include "infra/Collider.hpp"

ol::Collider::Collider()
{
    //ctor
}

ol::Collider::Collider(Vec2 p, Vec2 s)
{
    //ctor
    position = p;
    size = s;
}

ol::Collider::~Collider()
{
    //dtor
}

bool ol::Collider::collides(const Collider& c)
{
    float x1 = position.x;
    float y1 = position.y;
    float w1 = size.x;
    float h1 = size.y;
    float x2 = c.position.x;
    float y2 = c.position.y;
    float w2 = c.size.x;
    float h2 = c.size.y;

    if (std::max(x1 + w1, x2 + w2) - std::min(x1, x2) > w1 + w2)
    {
        return false;
    }
    else if (std::max(y1 + h1, y2 + h2) - std::min(y1, y2) > h1 + h2)
    {
        return false;
    }
    return true;
}

