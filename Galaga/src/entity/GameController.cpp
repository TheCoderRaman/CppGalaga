#include "infra/Util.hpp"
#include "infra/AudioPlayer.hpp"
#include "infra/Game.hpp"
#include "infra/SceneManager.hpp"
#include "entity/ScorePoint.hpp"
#include "entity/GameController.hpp"

ol::GameController::GameController(Stage& scene) : Entity<Stage>(scene, GAME_CONTROLLER_Z_ORDER),
                                                   availableGuardEnemiesIds_ { 0 }
{
    //ctor
    currentEnemiesGroup_ = 0;
    lastEnemiesGroup_ = 0;
    fighter_ = nullptr;
    capturedFighter_ = nullptr;
    isFighterCaptureStarted_ = false;
    fighterCaptureLastTime_ = getGameElapsedTime();
    isStartLabelVisible_ = false;
    isCurrentLevelLabelVisible_ = false;
    isReadyLabelVisible_ = false;
    isGameOverLabelVisible_ = false;
    isFighterCapturedLabelVisible_ = false;
    isEnemyBeamActive_ = false;
    gameOverStartTime_ = 0;
    doNothing();
}

ol::GameController::~GameController()
{
    //dtor
}

void ol::GameController::update()
{
    Entity::update();
}

void ol::GameController::updateDebug()
{
}

void ol::GameController::draw(Renderer& renderer)
{
    if (isStartLabelVisible_)
    {
        renderer.drawText1("START", Vec2(92.0f, 128.0f), Color::RED);
    }
    else if (isCurrentLevelLabelVisible_)
    {
        renderer.drawText1("STAGE " + hudInfo.getStageStr(),
                           Vec2(76.0f, 128.0f), Color::CYAN);
    }
    else if (isReadyLabelVisible_)
    {
        renderer.drawText1("READY!", Vec2(88.0f, 128.0f), Color::CYAN);
    }
    else if (isFighterCapturedLabelVisible_)
    {
        renderer.drawText1("FIGHTER CAPTURED", Vec2(48.0f, 128.0f), Color::RED);
    }
    else if (isGameOverLabelVisible_)
    {
        renderer.drawText1("GAME OVER", Vec2(76.0f, 128.0f), Color::RED);
    }
}

void ol::GameController::onMessageReceived(Message message)
{
}

// IMPLEMENTATION

void ol::GameController::startGame(int level)
{
    hudInfo.lives = 3;
    hudInfo.stage = 0;
    hudInfo.score = 0;

    scene_.getSceneManager().getStarsBackground().move();
    scene_.getSceneManager().getHud().set1UpBlinking(true);

    fighter_ = nullptr;

    showStart();
}

// communication entities -> game controller

void ol::GameController::onFighterHit(Entity<Stage>* source)
{
    fighter_->hit(source->getCollider());
    playSound(Sound::FIGHTER_EXPLOSION);
    //log("game controller: onFighterHit()");
}

void ol::GameController::onFighterDestroyed()
{
    fighter_ = nullptr;
    //log("game controller: onFighterDestroyed()");
}

void ol::GameController::onCapturedFighterDestroyed()
{
    capturedFighter_ = nullptr;
    //log("game controller: onCapturedFighterDestroyed()");
}

void ol::GameController::onCapturedFighterHit(Entity<Stage>* source)
{
    capturedFighter_->hit(source->getCollider());
    playSound(Sound::FIGHTER_EXPLOSION);
    log("game controller: onCapturedFighterHit()");
}

void ol::GameController::onFighterCaptureStart()
{
    isFighterCaptureStarted_ = true;
    log("game controller: onFighterCaptureStart()");
}

void ol::GameController::onFighterCaptured(EnemyBeam* enemyBeam)
{
    playSound(Sound::ENEMY_BEAM_FIGHTER_CAPTURED);

    scene_.broadcastMessage(Message::ALL_ENEMIES_STOP_FIRING);
    capturedFighter_ = enemyBeam->getCapturedFighter();
    Enemy* enemy = enemyBeam->getEnemy();

    capturedFighter_->setHitable(false);
    enemy->setHitable(false);

    capturedFighter_->getsPulledByBeam(enemy->getPosition());
    fighter_ = nullptr;

    float waitTime = 0.0f;
    int state = 0;
    setUpdateAction([=] () mutable
    {
        switch (state)
        {
            case 0:
                if (!capturedFighter_->isLastStateFinished())
                {
                    return;
                }
                capturedFighter_->getsCapturedByEnemy();
                state = 1;

            case 1:
                if (!capturedFighter_->isLastStateFinished())
                {
                    return;
                }
                capturedFighter_->setFormationPosition(
                    enemy->getFormationCol(), enemy->getFormationRow() + 1);

                capturedFighter_->follow(enemy, true);
                state = 2;

            case 2:
                if (!enemyBeam->isFinished())
                {
                    return;
                }
                enemyBeam->destroy();
                playSound(Sound::FIGHTER_CAPTURED);
                isFighterCapturedLabelVisible_ = true;
                waitTime = getGameElapsedTime();
                state = 3;

            case 3:
                if (getGameElapsedTime() - waitTime < 3)
                {
                    return;
                }
                scene_.broadcastMessage(Message::ALL_ENEMIES_RETURN_TO_FORMATION);
                enemy->returnToFormation();
                state = 4;

            case 4:
                if (!enemy->hasReturnedToFormation())
                {
                    return;
                }
                capturedFighter_->setFormationPosition(
                    enemy->getFormationCol(), enemy->getFormationRow() - 1);
                capturedFighter_->returnToFormation();
                enemy->setFighterCaptured(true);
                waitTime = getGameElapsedTime();
                state = 5;

            case 5:
                if (getGameElapsedTime() - waitTime < 1 ||
                    !capturedFighter_->hasReturnedToFormation())
                {
                    return;
                }
                capturedFighter_->follow(enemy, false);
                isFighterCapturedLabelVisible_ = false;
                capturedFighter_->setHitable(true);
                enemy->setHitable(true);
                onFighterCaptureEnd();
                tryNextLife();
                state = 6;

            case 6:
                break;
        }
    });
    //log("game controller: onFighterCaptured()");
}

void ol::GameController::onFighterCaptureEnd()
{
    isFighterCaptureStarted_ = false;
    fighterCaptureLastTime_ = getGameElapsedTime();
    //log("game controller: onFighterCaptureEnd()");
}

void ol::GameController::onFighterRescued()
{
    playSound(Sound::FIGHTER_RESCUED);

    int state = 0;
    bool nextLife = false;
    setUpdateAction([=] () mutable
    {
        switch (state)
        {
            case 0:
                scene_.broadcastMessage(Message::ALL_ENEMIES_RETURN_TO_FORMATION);
                fighter_->setHitable(false);
                capturedFighter_->setHitable(false);

                fighter_->doNothing();
                capturedFighter_->rescued();
                state = 1;

            case 1:
                if (!capturedFighter_->isLastStateFinished())
                {
                    return;
                }
                fighter_->startDualMode();
                capturedFighter_->startDualMode();
                state = 2;

            case 2:
                if (!fighter_->isLastStateFinished() ||
                    !capturedFighter_->isLastStateFinished() ||
                    !isAllEnemiesReturnedToFormation())
                {
                    return;
                }
                fighter_->commitDualMode();
                capturedFighter_->commitDualMode();

                fighter_->setHitable(true);
                fighter_->allowPlaying();

                startEnemiesDiving();

                state = 3;

            case 3:
                break;
        };
    });
    //log("game controller: onFighterRescued()");
}

void ol::GameController::onFighterBulletCreated(FighterBullet* fighterBullet)
{
    playSound(Sound::FIGHTER_SHOT);
    fighterBulletsIds_.insert(fighterBullet->getId());
    // log("game controller: fighter bullet created.");

}

void ol::GameController::onFighterBulletDestroyed(FighterBullet* fighterBullet)
{
    fighterBulletsIds_.erase(fighterBullet->getId());
    // log("game controller: fighter bullet destroyed.");
}

void ol::GameController::onFighterBulletHit(FighterBullet* fighterBullet)
{
    fighterBullet->hit();
    // log("game controller: fighter bullet hit.");
}

void ol::GameController::onEnemyCreated(Enemy* enemy)
{
    enemiesIds_.insert(enemy->getId());
    // log("game controller: enemy created.");
}

void ol::GameController::onEnemyHit(Enemy* enemy)
{
    if (enemy->isBoss())
    {
        if (enemy->getLife() < 2)
        {
            playSound(Sound::ENEMY_BOSS_DESTROYED);
        }
        else
        {
            playSound(Sound::ENEMY_BOSS_FIRST_HIT);
        }
    }
    else if (enemy->isGuard())
    {
        playSound(Sound::ENEMY_GUARD_DESTROYED);
    }
    else // minion
    {
        playSound(Sound::ENEMY_MINION_DESTROYED);
    }
    enemy->hit();
}

void ol::GameController::onEnemyDestroyed(Enemy* enemy)
{
    enemiesIds_.erase(enemy->getId());
    enemiesInFormationIds_.erase(enemy->getId());
    if (enemy->getFormationRow() == 2)
    {
        availableGuardEnemiesIds_[enemy->getFormationCol()] = 0;
    }

    // add score
    if (enemy->isBoss())
    {
        if (enemy->isDiving())
        {
            if (enemy->getScorePoint() > 0)
            {
                scene_.spawn<ScorePoint>()->show(enemy->getScorePoint(), enemy->getPosition());
            }
            hudInfo.addScore(enemy->getScorePoint());
        }
        else
        {
            hudInfo.addScore(150);
        }
    }
    else if (enemy->isGuard())
    {
        hudInfo.addScore(enemy->isDiving() ? 160 : 80);
    } // minion
    else
    {
        hudInfo.addScore(enemy->isDiving() ? 100 : 50);
    }

    // log("game controller: enemy destroyed.");
}

void ol::GameController::onEnemyLeftFormation(Enemy* enemy)
{
    enemiesInFormationIds_.erase(enemy->getId());
    if (enemy->getFormationRow() == 2)
    {
        availableGuardEnemiesIds_[enemy->getFormationCol()] = 0;
    }
    // log("game controller: onEnemyLeftFormation()");
}

void ol::GameController::onEnemyReturnedToFormation(Enemy* enemy)
{
    enemiesInFormationIds_.insert(enemy->getId());
    if (enemy->getFormationRow() == 2)
    {
        availableGuardEnemiesIds_[enemy->getFormationCol()] = enemy->getId();
    }
    // log("game controller: onEnemyReturnedToFormation()");
}

void ol::GameController::onEnemyBeamCreated(EnemyBeam* enemyBeam)
{
    playSound(Sound::ENEMY_BEAM);
    isEnemyBeamActive_ = true;
}

void ol::GameController::onEnemyBeamDestroyed(EnemyBeam* enemyBeam)
{
    isEnemyBeamActive_ = false;
}

void ol::GameController::onEnemyBulletCreated(EnemyBullet* enemyBullet)
{
    enemyBulletsIds_.insert(enemyBullet->getId());
    // log("game controller: enemy bullet created.");
}

void ol::GameController::onEnemyBulletDestroyed(EnemyBullet* enemyBullet)
{
    enemyBulletsIds_.erase(enemyBullet->getId());
    // log("game controller: enemy bullet destroyed.");
}

void ol::GameController::onEnemyBulletHit(EnemyBullet* enemyBullet)
{
    enemyBullet->destroy();
    // log("game controller: enemy bullet hit.");
}

bool ol::GameController::hasMoreEnemies()
{
    return currentEnemiesGroup_ < lastEnemiesGroup_;
}

void ol::GameController::spawnNextEnemies()
{
    static std::map<int, const std::vector<Vec2>*> paths
    {
        { 1, &PATH_ENEMY_PATTERN_1 },
        { 2, &PATH_ENEMY_PATTERN_2 },
        { 3, &PATH_ENEMY_PATTERN_3 },
        { 4, &PATH_ENEMY_PATTERN_4 },
    };

    for (auto enemyInfo : enemiesGroup[currentEnemiesGroup_])
    {
        // TODO adjust this probabilty later according to the current stage
        float divingProbability = hudInfo.stage == 1 ? 0.0 : 0.25f;
        Enemy* enemy = scene_.spawn<Enemy>();
        enemy->moveToFormation(*paths[enemyInfo.pathId], enemyInfo.formationCol,
                               enemyInfo.formationRow, enemyInfo.order * 18.0f,
                               ol::random() < divingProbability);
        if (enemy->isBoss())
        {
            enemy->setLife(2);
        }
    }

    currentEnemiesGroup_++;
}

//Enemy* selectRandomEnemyInFormation();

bool ol::GameController::isChallengingStage()
{
    return hudInfo.stage % 3 == 0;
}

ol::Fighter* ol::GameController::getFighter()
{
    return fighter_;
}

void ol::GameController::setFighter(Fighter* fighter)
{
    fighter_ = fighter;
}

ol::Fighter* ol::GameController::getCapturedFighter()
{
    return capturedFighter_;
}

void ol::GameController::setCapturedFighter(Fighter* capturedFighter)
{
    capturedFighter_ = capturedFighter;
}

bool ol::GameController::isFighterCaptureAllowed()
{
    return !isFighterCaptureStarted_ && capturedFighter_ == nullptr &&
           (!isFighterDestroyed() && !fighter_->isDualMode());
}

bool ol::GameController::isFighterCaptured()
{
    return capturedFighter_ != nullptr;
}

bool ol::GameController::isCapturedFighterDestroyed()
{
    return capturedFighter_ == nullptr || capturedFighter_->isDestroyed();
}

bool ol::GameController::isFighterDestroyed()
{
    return fighter_ == nullptr || fighter_->isDestroyed();
}

int ol::GameController::getFighterBulletsCount()
{
    return fighterBulletsIds_.size();
}

bool ol::GameController::isAllFighterBulletsDestroyed()
{
    return fighterBulletsIds_.size() == 0;
}

bool ol::GameController::isAllEnemiesReturnedToFormation()
{
    return enemiesInFormationIds_.size() == enemiesIds_.size();
}

bool ol::GameController::isAllEnemiesDestroyed()
{
    return enemiesIds_.size() == 0;
}

bool ol::GameController::isAllEnemyBulletsDestroyed()
{
    return enemyBulletsIds_.size() == 0;
}

int ol::GameController::getEnemiesCount()
{
    return enemiesIds_.size();
}

int ol::GameController::getEnemiesInFormationCount()
{
    return enemiesInFormationIds_.size();
}

int ol::GameController::getEnemyBulletsCount()
{
    return enemyBulletsIds_.size();
}

// game flow

void ol::GameController::showStart()
{

    float waitTime = 0.0f;
    int state = 0;
    setUpdateAction([=] () mutable {
        switch (state)
        {
            case 0:
                waitTime = getGameElapsedTime();
                state = 1;

            case 1:
                if (getGameElapsedTime() - waitTime < 2.0f)
                {
                    return;
                }
                playMusic(Music::START_GAME_MUSIC);
                isStartLabelVisible_ = true;
                waitTime = getGameElapsedTime();
                state = 2;

            case 2:
                if (getGameElapsedTime() - waitTime < 7.0f)
                {
                    return;
                }
                isStartLabelVisible_ = false;
                showCurrentLevel();
                state = 3;

            case 3:
                break;
        };
    });
}

void ol::GameController::showCurrentLevel()
{
    currentEnemiesGroup_ = 0;
    lastEnemiesGroup_ = 5;

    hudInfo.stage++;

    float waitTime = 0.0f;
    int state = 0;
    setUpdateAction([=] () mutable {
        switch (state)
        {
            case 0:
                isCurrentLevelLabelVisible_ = true;
                scene_.getSceneManager().getHud().setStage(hudInfo.stage);
                waitTime = getGameElapsedTime();
                state = 1;

            case 1:
                if (getGameElapsedTime() - waitTime < 3.0f)
                {
                    return;
                }
                isCurrentLevelLabelVisible_ = false;
                tryNextLife();
                state = 2;

            case 2:
                break;
        };
    });
}

void ol::GameController::spawnInitialEnemies()
{
    fighter_->setFireEnabled(true);
    scene_.broadcastMessage(Message::ALL_ENEMIES_START_FIRING);
    scene_.broadcastMessage(Message::FORMATION_MOVEMENT_HORIZONTAL);

    float waitTime = 0.0f;
    int state = 0;
    setUpdateAction([=] () mutable {

        if (isFighterDestroyed())
        {
            evaluateGameState();
            return;
        }

        switch (state)
        {
            case 0:
                if (hasMoreEnemies())
                {
                    spawnNextEnemies();
                }
                else
                {
                    startEnemiesDiving();
                    return;
                }
                waitTime = getGameElapsedTime();
                state = 1;

            case 1:
                if (getGameElapsedTime() - waitTime < 3.0f)
                {
                    return;
                }
                state = 0;
        };
    });
}

void ol::GameController::startEnemiesDiving()
{
    scene_.broadcastMessage(Message::FORMATION_MOVEMENT_SCALED);
    playSound(Sound::FORMATION_SCALED);

    float waitTime = 0.0f;
    int state = 0;
    setUpdateAction([=] () mutable {

        if (isFighterDestroyed() ||
            isAllEnemiesDestroyed())
        {
            evaluateGameState();
        }

        switch (state)
        {
            case 0:
                if (enemiesInFormationIds_.size() > 0)
                {
                    auto it = enemiesInFormationIds_.begin();
                    advance(it, ol::random(0, enemiesInFormationIds_.size() - 1));

                    //log("selected diving enemy " + std::to_string(*it) + " ...");

                    Enemy* enemy = scene_.getEntityById<Enemy>(*it);

                    if (enemy->isBoss() && isFighterCaptureAllowed() &&
                        getGameElapsedTime() - fighterCaptureLastTime_ > 10.0f &&
                        ol::random() < 0.35f)
                    {
                        enemy->tryToCaptureFighter();
                    }
                    else if (enemy->isBoss())
                    {
                        int guardCount = 0;
                        int bossFormationCol = enemy->getFormationCol();
                        for (int i = -1; i <= 1; i++)
                        {
                            int guardId = availableGuardEnemiesIds_[bossFormationCol - i];
                            if (guardId > 0 && ol::random() < 0.5f)
                            {
                                Enemy* guard = scene_.getEntityById<Enemy>(guardId);
                                guard->follow(enemy, true);
                                guard->onDivingStarted();
                                guardCount++;
                            }
                        }
                        static int bossDivingScoreTable[] = { 400, 800, 1600, 2000 };
                        enemy->setScorePoint(bossDivingScoreTable[guardCount]);
                        enemy->startDiving(false);
                    }
                    else if (enemy->isMinion())
                    {
                        enemy->startDiving(true);
                    }
                    else // guard
                    {
                        enemy->startDiving(false);
                    }

                    playSound(Sound::ENEMY_DIVING);
                }

                waitTime = getGameElapsedTime() + ol::random(0.3f, 2.0f);
                state = 1;

            case 1:
                if (getGameElapsedTime() < waitTime)
                {
                    return;
                }
                state = 0;
        };
    });
}

void ol::GameController::tryNextLife()
{
    float waitTime = 0.0f;
    int state = 0;
    setUpdateAction([=] () mutable {
        switch (state)
        {
            case 0:
                if (!isAllEnemiesReturnedToFormation())
                {
                    return;
                }

                if (fighter_ != nullptr)
                {
                    spawnInitialEnemies();
                    return;
                }
                else if (hudInfo.lives == 0)
                {
                    startGameOver();
                    return;
                }

                hudInfo.lives--;
                fighter_ = scene_.spawn<Fighter>();
                fighter_->nextLife();
                state = 1;

            case 1:
                if (!fighter_->isLastStateFinished())
                {
                    return;
                }
                isReadyLabelVisible_ = true;
                fighter_->allowPlaying();
                waitTime = getGameElapsedTime();
                state = 2;

            case 2:
                if (getGameElapsedTime() - waitTime < 3.0f ||
                    !isAllEnemiesReturnedToFormation())
                {
                    return;
                }
                isReadyLabelVisible_ = false;
                fighter_->setHitable(true);
                fighter_->setFireEnabled(true);
                scene_.broadcastMessage(Message::ALL_ENEMIES_START_FIRING);
                spawnInitialEnemies();
                state = 3;

            case 3:
                break;
        }
    });
    //log("game controller: tryNextLife()");
}

void ol::GameController::startGameOver()
{
    float waitTime = 0.0f;
    int state = 0;
    setUpdateAction([=] () mutable {
        switch (state)
        {
            case 0:
                isGameOverLabelVisible_ = true;
                playMusic(Music::GAME_OVER);
                waitTime = getGameElapsedTime();
                state = 1;

            case 1:
                if (getGameElapsedTime() - waitTime < 3.0f)
                {
                    return;
                }
                scene_.broadcastMessage(Message::ALL_ACTORS_DISPERSE);
                scene_.getSceneManager().getHud().hide();
                waitTime = getGameElapsedTime();
                state = 2;

            case 2:
                if (getGameElapsedTime() - waitTime < 5.0f)
                {
                    return;
                }
                isGameOverLabelVisible_ = false;
                waitTime = getGameElapsedTime();
                state = 3;

            case 3:
                if (getGameElapsedTime() - waitTime < 3.0f)
                {
                    return;
                }
                scene_.getSceneManager().getHud().set1UpBlinking(false);
                scene_.getSceneManager().changeScene(Scene::Id::OL_PRESENTS);
                state = 4;

            case 4:
                break;
        };
    });
}

void ol::GameController::evaluateGameState()
{
    scene_.getSceneManager().getHud().setStage(0);
    if (!isFighterDestroyed())
    {
        fighter_->setFireEnabled(false);
    }
    scene_.broadcastMessage(Message::ALL_ENEMIES_STOP_FIRING);
    float waitTime = 0.0f;
    int state = 0;
    setUpdateAction([=] () mutable {
        switch (state)
        {
            case 0:
                if (isEnemyBeamActive_) // wait enemy / enemy beam animation finish
                {
                    return;
                }
                scene_.broadcastMessage(Message::ALL_ENEMIES_RETURN_TO_FORMATION);
                waitTime = getGameElapsedTime();
                state = 1;

            case 1:
                if (getGameElapsedTime() - waitTime < 1.5f)
                {
                    return;
                }

                if (!isAllEnemiesReturnedToFormation() ||
                    !isAllFighterBulletsDestroyed() ||
                    !isAllEnemyBulletsDestroyed())
                {
                    return;
                }

                if (isFighterDestroyed())
                {
                    tryNextLife();
                }
                else
                {
                    showCurrentLevel();
                }
                state = 2;

            case 2:
                break;

        };
    });
    // log("GameController::evaluateGameState()");
}
