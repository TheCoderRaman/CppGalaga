#include <cmath>
#include "infra/Util.hpp"
#include "infra/Resources.hpp"
#include "infra/media_layer/AudioPlayer.hpp"
#include "infra/Hud.hpp"

ol::HudInfo ol::hudInfo { 0, 0, 0, 30000, 30000 };

// struct HudInfo

bool ol::HudInfo::isChallengingStage()
{
    int n = stage;
    if (n > 4)
    {
        n -= 1;
    }
    return (n % 3) == 0;
}

std::string ol::HudInfo::getStageStr()
{
    std::string stageStr = "000" + std::to_string(stage);
    stageStr = stageStr.substr(stageStr.length() - 3, 3);
    return stageStr;
}

std::string ol::HudInfo::getLivesStr()
{
    std::string livesStr = "00" + std::to_string(lives);
    livesStr = livesStr.substr(livesStr.length() - 2, 2);
    return livesStr;
}

std::string ol::HudInfo::getScoreStr()
{
    std::string scoreStr = "000000" + std::to_string(score);
    scoreStr = scoreStr.substr(scoreStr.length() - 6, 6);
    return scoreStr;
}

std::string ol::HudInfo::getHiscoreStr()
{
    std::string hiscoreStr = "000000" + std::to_string(hiscore);
    hiscoreStr = hiscoreStr.substr(hiscoreStr.length() - 6, 6);
    return hiscoreStr;
}

void ol::HudInfo::addScore(int point)
{
    score += point;
    if (score > nextExtraLifeScore)
    {
        lives++;
        nextExtraLifeScore += 70000;
        playSound(Sound::EXTRA_LIFE);
    }
}

void ol::HudInfo::updateHiscore()
{
    if (score > hiscore)
    {
        hiscore = score;
    }
}

// class Hud

ol::Hud::Hud()
{
    //ctor
    isLivesVisible_ = true;
    is1UpBlinkink_ = false;
    stageSymbols_.setStage(0);
    scale_ = 1.2f;
    targetScale_ = 1.2f;
}

ol::Hud::~Hud()
{
    //dtor
}

void ol::Hud::setStage(int stage)
{
    stageSymbols_.setStage(stage);
}

bool ol::Hud::isStageSymbolsFinished()
{
    return stageSymbols_.isFinished();
}

bool ol::Hud::isLivesVisible()
{
    return isLivesVisible_;
}

bool ol::Hud::set1UpBlinking(bool is1UpBlinkink)
{
    is1UpBlinkink_ = is1UpBlinkink;
}

void ol::Hud::setLivesVisible(bool isLivesVisible)
{
    isLivesVisible_ = isLivesVisible;
}

void ol::Hud::update()
{
    float dif = targetScale_  - scale_;
    if (fabs(dif) < 0.01f)
    {
        scale_ = targetScale_;
    }
    else {
        scale_ += dif > 0 ? 0.005f : -0.005f;
    }

    //1.0f + 0.2f * sin(getGameElapsedTime());
    stageSymbols_.update();
}

void ol::Hud::draw(Renderer& renderer)
{
    // score
    renderer.drawText1("          HI-SCORE", getScaledPosition(Vec2(1.0f, 1.0f)), Color::RED);

    if (!is1UpBlinkink_ || (is1UpBlinkink_ && blink()))
    {
        renderer.drawText1("1UP", getScaledPosition(Vec2(1.0f, 1.0f)), Color::RED);
    }

    renderer.drawText1(hudInfo.getScoreStr(), getScaledPosition(Vec2(1.0f, 9.0f)), Color::WHITE);
    renderer.drawText1(hudInfo.getHiscoreStr(), getScaledPosition(Vec2(89.0f, 9.0f)), Color::WHITE);

    // lives
    if (isLivesVisible_) {
        renderer.drawSprite(Sprite::FIGHTER,
                            getScaledPosition(Vec2(0.0f, SCREEN_HEIGHT - 17.0f)),
                            Vec2(96.0f, 0.0f), Vec2(16.0f, 16.0f));

        renderer.drawText1("x" + hudInfo.getLivesStr(),
                           getScaledPosition(Vec2(17.0f, 278.0f)),
                           Color::WHITE);
    }

    stageSymbols_.draw(renderer);
}

ol::Vec2 ol::Hud::getScaledPosition(Vec2 position)
{
    if (scale_ != 1.0f)
    {
        position = position - Vec2(112.0f, 144.0f);
        position = scale_ * position;
        position = position + Vec2(112.0f, 144.0f);
    }
    return position;
}

void ol::Hud::show()
{
    targetScale_ = 1.0f;
}

void ol::Hud::hide()
{
    targetScale_ = 1.2f;
    stageSymbols_.setStage(0);
}
