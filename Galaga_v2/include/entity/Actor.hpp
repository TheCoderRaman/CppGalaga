#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "infra/Vec2.hpp"
#include "infra/Collider.hpp"
#include "infra/Path.hpp"
#include "infra/Resources.hpp"
#include "infra/media_layer/Renderer.hpp"
#include "infra/Entity.hpp"
#include "scene/Stage.hpp"

namespace ol
{
    class Actor : public Entity<Stage>
    {
        public:
            enum class FormationMovementType
            {
                HORIZONTAL,
                SCALED
            };

            Actor(Stage& scene, int zOrder);
            virtual ~Actor();

            GameController* getGameController();

            int getFormationCol();
            int getFormationRow();
            void setFormationPosition(int col, int row);

            FormationMovementType getFormationMovementType();
            void setFormationMovementType(FormationMovementType formationMovementType);
            Vec2& getFormationScreenPosition();

            bool hasReturnedToFormation();
            bool isDiving();
            bool isLastStateFinished();

            bool isFireEnabled();
            void setFireEnabled(bool isFireEnabled);

            void draw(Renderer& renderer);

            void update();
            void updateDebug();

            void onMessageReceived(Message message);

            virtual void onLeftFormation();
            virtual void onReturnedToFormation();

            virtual void move(std::vector<Vec2> pathPoints, int advance);
            virtual void moveToFormation(std::vector<Vec2> pathPoints, int formationCol, int formationRow, int advance, bool diveInTheMiddleOfPath = false);
            virtual void startDiving(bool returnIntheMiddleOfPath = false);
            virtual void onDivingStarted();
            virtual void onDivingFinished();
            virtual void returnToFormation();
            virtual void disperse();
            virtual void follow(Actor* followActor, bool untilFormation = false);
            virtual void explode();
            virtual void fire();

        protected:
            void setReturnedToFormation(bool hasReturnedToFormation);
            virtual void updateFrame();
            virtual void updateFormationScreenPosition();

        protected:
            float randomAngle_;

            float angleTransitionFactor_;
            float lastStateTime_;
            bool isLastStateFinished_;
            bool isDiving_;
            float divingStartTime_;
            bool hasReturnedToFormation_;
            FormationMovementType formationMovementType_;
            float formationMovementTransitionFactor_;
            int formationCol_;
            int formationRow_;
            Vec2 formationScreenPosition_;

            Path path_;
            float pathT_;

            bool isFireEnabled_;
            float fireLastTime_;

            Actor* followActor_;

        private:

    };

}

#endif // ACTOR_HPP
