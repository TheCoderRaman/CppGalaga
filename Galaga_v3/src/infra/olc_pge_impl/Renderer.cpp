#include <map>

#include "olcPixelGameEngine.h"

#include "infra/Util.hpp"
#include "infra/Resources.hpp"
#include "infra/media_layer/Renderer.hpp"

class OlcPgeWindow : public olc::PixelGameEngine
{
};

extern OlcPgeWindow* olcPgeWindow; // note: it's defined in Game.cpp

// can't use static map in linux ?
static std::map<ol::Sprite, olc::Sprite*>* sprites;

static olc::Pixel intToColor(int c)
{
    return olc::Pixel(static_cast<uint32_t>(c));
}

static void loadFonts()
{
    // TODO load font text 2
}

static olc::Sprite* loadSprite(std::string filename)
{
    const std::string path = "res/graphic/";
    olc::Sprite* sprite = new olc::Sprite();
    if (!sprite->LoadFromFile(path + filename))
    {
        ol::error("could not load sprite \"" + path + filename + "\" !");
    }
    return sprite;
}

static void loadAllSprites()
{
    sprites = new std::map<ol::Sprite, olc::Sprite*>;

    std::map<ol::Sprite, olc::Sprite*>& spr = *sprites;

    spr[ol::Sprite::TITLE] = loadSprite("title.png");
    spr[ol::Sprite::TITLE_SHINE] = loadSprite("title_shine.png");
    spr[ol::Sprite::NAMCO] = loadSprite("namco.png");

    spr[ol::Sprite::FIGHTER] = loadSprite("fighter.png");
    spr[ol::Sprite::FIGHTER_CAPTURED] = loadSprite("fighter_captured.png");
    spr[ol::Sprite::FIGHTER_BULLET] = loadSprite("fighter_bullet.png");
    spr[ol::Sprite::FIGHTER_EXPLOSION] = loadSprite("fighter_explosion.png");

    spr[ol::Sprite::ENEMY_1] = loadSprite("enemy_1.png");
    spr[ol::Sprite::ENEMY_1_HIT] = loadSprite("enemy_1_hit.png");
    spr[ol::Sprite::ENEMY_2] = loadSprite("enemy_2.png");
    spr[ol::Sprite::ENEMY_3] = loadSprite("enemy_3.png");
    spr[ol::Sprite::ENEMY_3_MORPHING] = loadSprite("enemy_3_morphing.png");
    spr[ol::Sprite::ENEMY_4] = loadSprite("enemy_4.png");
    spr[ol::Sprite::ENEMY_5] = loadSprite("enemy_5.png");
    spr[ol::Sprite::ENEMY_6] = loadSprite("enemy_6.png");
    spr[ol::Sprite::ENEMY_7] = loadSprite("enemy_7.png");
    spr[ol::Sprite::ENEMY_8] = loadSprite("enemy_8.png");
    spr[ol::Sprite::ENEMY_9] = loadSprite("enemy_9.png");

    spr[ol::Sprite::ENEMY_BULLET] = loadSprite("enemy_bullet.png");
    spr[ol::Sprite::ENEMY_EXPLOSION] = loadSprite("enemy_explosion.png");
    spr[ol::Sprite::ENEMY_BEAM] = loadSprite("enemy_beam.png");

    spr[ol::Sprite::STAGE_SYMBOL_1] = loadSprite("stage_symbol_1.png");
    spr[ol::Sprite::STAGE_SYMBOL_5] = loadSprite("stage_symbol_5.png");
    spr[ol::Sprite::STAGE_SYMBOL_10] = loadSprite("stage_symbol_10.png");
    spr[ol::Sprite::STAGE_SYMBOL_20] = loadSprite("stage_symbol_20.png");
    spr[ol::Sprite::STAGE_SYMBOL_30] = loadSprite("stage_symbol_30.png");
    spr[ol::Sprite::STAGE_SYMBOL_50] = loadSprite("stage_symbol_50.png");
}

ol::Renderer::Renderer()
{
    //ctor
    translation_ = ol::Vec2(0, 0);
    loadFonts();
    loadAllSprites();
}

ol::Renderer::~Renderer()
{
    //dtor
}

void ol::Renderer::setTranslation(Vec2 translation)
{
    translation_ = translation;
}

void ol::Renderer::drawPoint(Vec2 position, int color)
{
    Vec2 finalPosition = position + translation_;
    olc::Pixel pixel(static_cast<uint32_t>(0xFF000000 + color));

    olcPgeWindow->Draw(static_cast<int32_t>(finalPosition.x),
                      static_cast<int32_t>(finalPosition.y), pixel);
}

void ol::Renderer::drawRectangle(Vec2 position, Vec2 size, int color)
{
    Vec2 finalPosition = position + translation_;
    olc::Pixel pixel(static_cast<uint32_t>(0xFF000000 + color));

    olcPgeWindow->DrawRect(static_cast<int32_t>(finalPosition.x),
                           static_cast<int32_t>(finalPosition.y),
                           static_cast<int32_t>(size.x),
                           static_cast<int32_t>(size.y), pixel);
}

void ol::Renderer::drawSprite(ol::Sprite sprite, Vec2 dstPosition, Vec2 srcPosition, Vec2 srcSize)
{
    Vec2 finalPosition = dstPosition + translation_;
    olc::Sprite* spr = (*sprites)[sprite];

    olcPgeWindow->DrawPartialSprite(static_cast<int32_t>(finalPosition.x),
                                    static_cast<int32_t>(finalPosition.y), spr,
                                    static_cast<int32_t>(srcPosition.x),
                                    static_cast<int32_t>(srcPosition.y),
                                    static_cast<int32_t>(srcSize.x),
                                    static_cast<int32_t>(srcSize.y),
                                    static_cast<int32_t>(1));
}

void ol::Renderer::drawText1(std::string text, Vec2 position, int color)
{
    Vec2 finalPosition = position + translation_;
    olc::Pixel pixel(static_cast<uint32_t>(0xFF000000 + color));

    olcPgeWindow->DrawString(static_cast<int32_t>(finalPosition.x),
                             static_cast<int32_t>(finalPosition.y),
                             text, pixel);
}

void ol::Renderer::drawText2(std::string text, Vec2 position, int color)
{
    // TODO implement smaller font later

    Vec2 finalPosition = position + translation_;
    olc::Pixel pixel(static_cast<uint32_t>(0xFF000000 + color));

    olcPgeWindow->DrawString(static_cast<int32_t>(finalPosition.x),
                             static_cast<int32_t>(finalPosition.y),
                             text, pixel);
}

