#include <iostream>
#include <cstdlib>
#include <cmath>
#include "infra/Util.hpp"
#include "infra/Path.hpp"

ol::Path::Path()
{
    //ctor
    lastPoint = nullptr;
}

ol::Path::~Path()
{
    //dtor
}

ol::Vec2& ol::Path::getLastPoint()
{
    return *lastPoint;
}

void ol::Path::addPoint(Vec2 point)
{
    points_.push_back(point);
    lastPoint = &points_[points_.size() - 1];
}

void ol::Path::addPoints(const std::vector<Vec2>& points)
{
    for (auto point : points)
    {
        addPoint(point);
    }
}

std::vector<ol::Vec2>& ol::Path::getPoints()
{
    return points_;
}

void ol::Path::clear()
{
    points_.clear();
    lastPoint = nullptr;
}

void ol::Path::draw(Renderer& renderer)
{
    if (points_.empty())
    {
        return;
    }
    float t = 0.0f;
    while (t < points_.size() - 1)
    {
        Vec2 p = advance(t, 3.0f);
        renderer.drawPoint(p, Color::WHITE);
    }
    for (auto p : points_)
    {
        renderer.drawPoint(p, Color::RED);
    }
}

ol::Vec2 ol::Path::getControlPoint(int i)
{
    if (i < 0)
    {
        i = 0;
    }
    if (i > points_.size() - 1)
    {
        i = points_.size() - 1;
    }
    return points_[i];
}

ol::Vec2 ol::Path::getPoint(float t)
{
    Vec2 a, b, c, d, r;
    int i = (int) t;
    a = getControlPoint(i - 1);
    b = getControlPoint(i + 0);
    c = getControlPoint(i + 1);
    d = getControlPoint(i + 2);
    t -= i;
    float tt = t * t;
    float ttt = t * tt;
    r = 0.5f * (a * (-ttt + 2 * tt - t) +
               b * (3 * ttt - 5 * tt + 2) +
               c * (-3 * ttt + 4 * tt + t) +
               d * (ttt - tt));
    return r;
}

ol::Vec2 ol::Path::getTangent(float t)
{
    Vec2 a, b, c, d, r;
    int i = (int) t;
    a = getControlPoint(i - 1);
    b = getControlPoint(i + 0);
    c = getControlPoint(i + 1);
    d = getControlPoint(i + 2);
    t -= i;
    float tt = t * t;
    float ttt = t * tt;
    r = 0.5f * (a * (-3 * tt + 4 * t - 1) +
                b * (9 * tt - 10 * t) +
                c * (-9 * tt + 8 * t + 1) +
                d * (3 * tt - 2 * t));
    return r;
}

ol::Vec2 ol::Path::advance(float &currentT, float desiredDistance)
{
    Vec2 currentP, nextP, distance;
    nextP = currentP = getPoint(currentT);
    float distanceLength = 0;
    while (currentT < points_.size() - 1 &&
           distanceLength < desiredDistance)
    {
        currentT += 0.002f;
        nextP = getPoint(currentT);
        distance = nextP - currentP;
        distanceLength = distance.getLength();
    }
    return nextP;
}

bool ol::Path::isFinished(float t)
{
    return t >= points_.size() - 1;
}

// auxiliar functions

void ol::addPathLoopPoints(Path& path, float radius)
{
    float loopSide = ol::random() < 0.5f ? -1 : 1; // left or right loop
    float pathT = path.getPoints().size() - 1;
    Vec2 point = path.getPoint(pathT);
    Vec2 tangentVector = path.getTangent(pathT);
    tangentVector = tangentVector.normalize();
    Vec2 lastPoint = point + 30 * tangentVector;
    tangentVector = tangentVector.rotate(loopSide * 3.14159f / 2);
    float angle = atan2(tangentVector.y, tangentVector.x);
    Vec2 center = point + radius * tangentVector;
    int n = 8;
    float angleStep = 6.28318f / n;
    angle += loopSide * angleStep - loopSide * 3.14159f;
    for (int i = 1; i < n; i++)
    {
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        path.addPoint(center + Vec2(x, y));
        angle += loopSide * angleStep;
    }
    path.addPoint(point);
    path.addPoint(lastPoint);
}

// predefined paths

const std::vector<ol::Vec2> ol::PATH_EMPTY { {0.0f, 0.0f} };

const std::vector<ol::Vec2> ol::PATH_FIGHTER_NEXT_LIFE {
    {8.0f, 280.0f}, {13.0f, 240.0f}, {40.0f, 210.0f}, {80.0f, 300.0f}, {100.0f, 300.0f},
    {110.0f, 274.0f}, {112.0f, 256.0f} };

const std::vector<ol::Vec2> ol::PATH_ENEMY_PATTERN_0 {
    {30.0f, -150.0f}, {170.0f, -50.0f}, {60.0f, 0.0f}, {30.0f, 60.0f}, {170.0f, 160.0f},
    {70.0f, 180.0f}, {120.0f, 80.0f} };

const std::vector<ol::Vec2> ol::PATH_ENEMY_PATTERN_1 {
    { 90.0f, -150.0f }, { 93.0f, 1.0f }, { 97.0f, 21.0f }, { 112.0f, 57.0f },
    { 150.0f, 100.0f }, { 190.0f, 130.0f }, { 200.0f, 149.0f }, { 192.0f, 167.0f },
    { 170.0f, 179.0f }, { 147.0f, 167.0f }, { 140.0f, 145.0f }, { 139.0f, 131.0f } };

const std::vector<ol::Vec2> ol::PATH_ENEMY_PATTERN_2 {
    { 224.0f - 90.0f, -150.0f }, { 224.0f - 93.0f, 1.0f }, { 224.0f - 97.0f, 21.0f }, { 224.0f - 112.0f, 57.0f },
    { 224.0f - 150.0f, 100.0f }, { 224.0f - 190.0f, 130.0f }, { 224.0f - 200.0f, 149.0f }, { 224.0f - 192.0f, 167.0f },
    { 224.0f - 170.0f, 179.0f }, { 224.0f - 147.0f, 167.0f }, { 224.0f - 140.0f, 145.0f }, { 224.0f - 139.0f, 131.0f } };

const std::vector<ol::Vec2> ol::PATH_ENEMY_PATTERN_3 {
    { -150.0f, 228.0f }, { -20.0f, 228.0f }, { 0.0f, 228.0f }, { 29.0f, 217.0f },
    { 61.0f, 193.0f }, { 84.0f, 168.0f }, { 95.0f, 148.0f }, { 94.0f, 130.0f },
    { 81.0f, 115.0f }, { 60.0f, 118.0f }, { 50.0f, 136.0f }, { 56.0f, 153.0f },
    { 74.0f, 159.0f }, { 93.0f, 151.0f }, { 94.0f, 130.0f } };

const std::vector<ol::Vec2> ol::PATH_ENEMY_PATTERN_4 {
    { 224.0f - -150.0f, 228.0f }, { 224.0f - -20.0f, 228.0f }, { 224.0f - 0.0f, 228.0f }, { 224.0f - 29.0f, 217.0f },
    { 224.0f - 61.0f, 193.0f }, { 224.0f - 84.0f, 168.0f }, { 224.0f - 95.0f, 148.0f }, { 224.0f - 94.0f, 130.0f },
    { 224.0f - 81.0f, 115.0f }, { 224.0f - 60.0f, 118.0f }, { 224.0f - 50.0f, 136.0f }, { 224.0f - 56.0f, 153.0f },
    { 224.0f - 74.0f, 159.0f }, { 224.0f - 93.0f, 151.0f }, { 224.0f - 94.0f, 130.0f } };
