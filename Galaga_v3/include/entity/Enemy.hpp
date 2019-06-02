#ifndef ENEMY_HPP
#define ENEMY_HPP

#define ENEMY_Z_ORDER 50

#include <vector>
#include <map>
#include "entity/Actor.hpp"

namespace ol
{

    struct EnemyInfo
    {
        int pathId;
        int formationCol;
        int formationRow;
        int order;
    };

    extern std::vector<std::vector<EnemyInfo>> enemiesGroup;

    class Enemy : public Actor
    {
        public:

            static std::map<int, Sprite> rowEnemyMap;

            Enemy(Stage& scene);
            virtual ~Enemy();

            bool isBoss();
            bool isGuard();
            bool isMinion();
            bool isSpecial();

            bool isFighterCaptured();
            void setFighterCaptured(bool isFighterCaptured);

            int getLife();
            void setLife(int life);

            int getScorePoint();
            void setScorePoint(int scorePoint);

            int getTeamId();
            void setTeamId(int teamId);

            bool isMorphing();
            void setMorphing(bool isMorphing);

            void draw(Renderer& renderer);
            void update();
            void updateDebug();

            void onMessageReceived(Message message);

            void onLeftFormation();
            void onReturnedToFormation();
            void onDivingStarted();
            void onDivingFinished();

            void decideWhenFire();
            void fire();
            void tryToCaptureFighter();
            void hit();
            void explode();
            void startMorphing(); // this enemy turns into three

        protected:
            void checkCollisions();

        private:
            bool isFighterCaptured_;
            int life_;

            int availableBulletsCount_;
            float fireRandomTime_;

            int scorePoint_;

            int teamId_;

            bool isMorphing_;
    };

}

#endif // ENEMY_HPP
