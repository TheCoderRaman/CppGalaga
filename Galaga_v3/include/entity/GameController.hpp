#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#define GAME_CONTROLLER_Z_ORDER 1000

#include <set>
#include "infra/Collider.hpp"
#include "infra/Entity.hpp"
#include "scene/Stage.hpp"
#include "entity/Fighter.hpp"
#include "entity/FighterBullet.hpp"
#include "entity/EnemyBullet.hpp"
#include "entity/EnemyBeam.hpp"

namespace ol
{

    class GameController final : public Entity<Stage>
    {
        public:
            GameController(Stage& scene);
            virtual ~GameController();

            void update();
            void updateDebug();

            void draw(Renderer& renderer);

            void onMessageReceived(Message message);

            void startGame(int level);

            // communication entities -> game controller

            void onFighterHit(Entity<Stage>* source);
            void onCapturedFighterDestroyed();
            void onFighterDestroyed();

            void onCapturedFighterHit(Entity<Stage>* source);
            void onFighterCaptureStart();
            void onFighterCaptured(EnemyBeam* enemyBeam);
            void onFighterCaptureEnd();

            void onFighterRescued();

            void onFighterBulletCreated(FighterBullet* fighterBullet);
            void onFighterBulletDestroyed(FighterBullet* fighterBullet);
            void onFighterBulletHit(FighterBullet* fighterBullet);

            void onEnemyCreated(Enemy* enemy);
            void onEnemyHit(Enemy* enemy);
            void onEnemyDestroyed(Enemy* enemy);
            void onEnemyLeftFormation(Enemy* enemy);
            void onEnemyReturnedToFormation(Enemy* enemy);
            void onEnemyBeamCreated(EnemyBeam* enemyBeam);
            void onEnemyBeamDestroyed(EnemyBeam* enemyBeam);

            void onEnemyBulletCreated(EnemyBullet* enemyBullet);
            void onEnemyBulletDestroyed(EnemyBullet* enemyBullet);
            void onEnemyBulletHit(EnemyBullet* enemyBullet);

            // notes:
            // communication game controller -> [ broadcast message  ] -> all entities
            //            or game controller -> [call method directly] -> entity

            //       NOT ALLOWED ! => entity -> [call method directly] -> entity

            bool hasMoreEnemies();
            void spawnNextEnemies();

            //Enemy* selectRandomEnemyInFormation();

            bool isChallengingStage();

            Fighter* getFighter();
            void setFighter(Fighter* fighter);

            Fighter* getCapturedFighter();
            void setCapturedFighter(Fighter* capturedFighter);

            bool isFighterCaptureAllowed();
            bool isFighterCaptured();
            bool isCapturedFighterDestroyed();
            bool isFighterDestroyed();
            bool isAllFighterBulletsDestroyed();
            int getFighterCount();
            int getFighterBulletsCount();

            bool isAllEnemiesReturnedToFormation();
            bool isAllEnemiesDestroyed();
            bool isAllEnemyBulletsDestroyed();
            int getEnemiesCount();
            int getEnemiesInFormationCount();
            int getEnemyBulletsCount();

        protected:

        private:
            int level_;

            int currentEnemiesGroup_;
            int lastEnemiesGroup_;

            float elapsedTime_;

            bool isFighterCaptureStarted_;
            float fighterCaptureLastTime_;
            Fighter* fighter_;
            Fighter* capturedFighter_;

            std::set<int> fighterBulletsIds_;
            std::set<int> enemiesIds_;
            std::set<int> enemiesInFormationIds_;
            std::set<int> enemyBulletsIds_;
            bool isEnemyBeamActive_;

            // to help boss enemy to decide wich guards enemies it will drag together
            int availableGuardEnemiesIds_[10];

            std::map<int, int> challengingStageEnemiesTeamCount_; // (teamId, enemyCount)
            int challengingStageTotalEnemiesCount_;
            int challengingStageTotalEnemiesHit_;

            bool isStartLabelVisible_;
            bool isCurrentLevelLabelVisible_;
            bool isReadyLabelVisible_;
            bool isGameOverLabelVisible_;
            bool isFighterCapturedLabelVisible_;

            // challenging stage labels
            bool isNumberOfHitsLabelVisible_;
            bool isNumberOfHitsNumberLabelVisible_;
            bool isBonusLabelVisible_;
            bool isPerfectLabelVisible_;

            float gameOverStartTime_;

        // game flow
        private:

            void showStart();
            void showCurrentLevel();

            void spawnChallengingStageEnemies();
            void spawnInitialEnemies();

            void startEnemiesDiving();
            void startGameOver();
            void tryNextLife();

            void evaluateChallengingStage();
            void evaluateGameState(); // next life, game over or next stage
    };

}

#endif // GAMECONTROLLER_HPP
