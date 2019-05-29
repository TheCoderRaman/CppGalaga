#ifndef SCOREPOINT_HPP
#define SCOREPOINT_HPP

#define SCORE_POINT_Z_ORDER 5

#include "scene/Stage.hpp"

namespace ol
{

    class ScorePoint : public Entity<Stage>
    {
        public:
            ScorePoint(Stage scene);
            virtual ~ScorePoint();

            void update();
            void draw(Renderer& render);

            void show(int point, Vec2 position);

        protected:

        private:
            std::string pointStr_;
            int color_;
            float showTime_;

    };

}

#endif // SCOREPOINT_HPP
