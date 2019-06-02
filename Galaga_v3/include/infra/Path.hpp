#ifndef PATH_HPP
#define PATH_HPP

#include <vector>
#include "infra/Vec2.hpp"
#include "infra/media_layer/Renderer.hpp"

namespace ol
{

    class Path
    {
        public:
            Path();
            virtual ~Path();

            void clear();
            void addPoint(Vec2 point);
            void addPoints(const std::vector<Vec2>& points);
            std::vector<Vec2>& getPoints();

            Vec2& getLastPoint();
            Vec2 getControlPoint(int i);
            Vec2 getPoint(float t);
            Vec2 getTangent(float t);

            void draw(Renderer& renderer);

            Vec2 advance(float &currentT, float distance);

            bool isFinished(float t);

        protected:

        private:
            std::vector<Vec2> points_;
            Vec2* lastPoint;
    };

    // auxiliar functions

    std::vector<Vec2> createRandomChallengingStagePoints();

    void addPathLoopPoints(Path& path, float radius);


    // predefined paths

    extern const std::vector<Vec2> PATH_EMPTY;
    extern const std::vector<Vec2> PATH_FIGHTER_NEXT_LIFE;
    extern const std::vector<Vec2> PATH_ENEMY_PATTERN_0;
    extern const std::vector<Vec2> PATH_ENEMY_PATTERN_1;
    extern const std::vector<Vec2> PATH_ENEMY_PATTERN_2;
    extern const std::vector<Vec2> PATH_ENEMY_PATTERN_3;
    extern const std::vector<Vec2> PATH_ENEMY_PATTERN_4;

}

#endif // PATH_HPP
