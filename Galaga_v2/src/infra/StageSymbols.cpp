#include "infra/Util.hpp"
#include "infra/Hud.hpp"
#include "infra/media_layer/AudioPlayer.hpp"
#include "infra/StageSymbols.hpp"

const ol::StageSymbolInfo ol::STAGE_SYMBOLS_TABLE[] =
    {
        { Sprite::STAGE_SYMBOL_50, 50, Vec2(16, 16) },
        { Sprite::STAGE_SYMBOL_30, 30, Vec2(16, 16) },
        { Sprite::STAGE_SYMBOL_20, 20, Vec2(16, 16) },
        { Sprite::STAGE_SYMBOL_10, 10, Vec2(14, 16) },
        { Sprite::STAGE_SYMBOL_5,   5, Vec2( 8, 16) },
        { Sprite::STAGE_SYMBOL_1,   1, Vec2( 8, 16) }
    };

ol::StageSymbols::StageSymbols() : symbolsCount_ { 0 }
{
    isFinished_ = true;
    //ctor
}

ol::StageSymbols::~StageSymbols()
{
    //dtor
}

void ol::StageSymbols::setStage(int stage)
{
    for (int i = 0; i < 6; i++) {
        symbolsCount_[i] = 0;
        while (stage >= STAGE_SYMBOLS_TABLE[i].stage) {
            stage -= STAGE_SYMBOLS_TABLE[i].stage;
            symbolsCount_[i]++;
        }
    }
    symbolIndex_ = -3;
    lastSymbolIndex_ = -1;
    isFinished_ = false;
}

bool ol::StageSymbols::isFinished()
{
    return isFinished_;
}

int ol::StageSymbols::getStageTotalWidth()
{
    int stageTotalWidth = 0;
    for (int i = 0; i < 6; i++) {
        stageTotalWidth += symbolsCount_[i] * STAGE_SYMBOLS_TABLE[i].size.x;
    }
    return stageTotalWidth;
}

void ol::StageSymbols::update()
{
    symbolIndex_ += 0.075f;
    if (!isFinished_ && static_cast<int>(symbolIndex_) > lastSymbolIndex_)
    {
        lastSymbolIndex_ = static_cast<int>(symbolIndex_);
        if (!hudInfo.isChallengingStage())
        {
            playSound(Sound::STAGE_SYMBOL);
        }
    }
}

void ol::StageSymbols::draw(Renderer& renderer)
{
    float symbolX = SCREEN_WIDTH - getStageTotalWidth();
    float symbolY = SCREEN_HEIGHT - 17;
    int currentSymbolIndex = 0;
    for (int i = 0; i < 6; i++) {
        for (int c = 0; c < symbolsCount_[i]; c++) {
            if (currentSymbolIndex >= symbolIndex_) {
                return;
            }
            renderer.drawSprite(STAGE_SYMBOLS_TABLE[i].sprite,
                                Vec2(symbolX, symbolY), Vec2(0.0f, 0.0f),
                                STAGE_SYMBOLS_TABLE[i].size);

            symbolX += STAGE_SYMBOLS_TABLE[i].size.x;
            currentSymbolIndex++;
        }
    }
    isFinished_ = true;
}
