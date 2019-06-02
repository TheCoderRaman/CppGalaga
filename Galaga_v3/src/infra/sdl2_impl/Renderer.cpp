#include <map>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "infra/Util.hpp"
#include "infra/Resources.hpp"
#include "infra/media_layer/Renderer.hpp"

extern SDL_Window* window; // note: it's defined in Game.cpp
extern SDL_Renderer* windowRenderer; // note: it's defined in Game.cpp

static SDL_Texture* textFont1;
static SDL_Texture* textFont2;
static std::map<ol::Sprite, SDL_Texture*> sprites;

static SDL_Color intToColor(int c)
{
    SDL_Color color;
    color.r = c & 255;
    color.g = (c >> 8) & 255;
    color.b = (c >> 16) & 255;
    color.a = 255;
    return color;
}

static SDL_Texture* loadFont(std::string filename, int size)
{
    const std::string path = "res/font/";

    // load font and create text surface and render
    TTF_Font* textFont = TTF_OpenFont((path + filename).c_str(), size);
    if(textFont == NULL)
    {
        ol::error("Failed to load font !\n");
    }

    SDL_Color fg = { 255, 255, 255, 255 };

    std::string allCaracters = "";
    for (int c = 0; c < 256; c++)
    {
        int c2 = c;
        if (c2 < 32)
        {
            c2 = ' ';
        }
        allCaracters += static_cast<char>(c2);
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(textFont, allCaracters.c_str(), fg);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(windowRenderer, textSurface);
    SDL_SetTextureColorMod(textTexture, 255, 255, 255);

    SDL_FreeSurface(textSurface);
    TTF_CloseFont(textFont);

    return textTexture;
}

static void loadFonts()
{
    const std::string font1Filename = "PressStart2P.ttf";
    const std::string font2Filename = "PIXELADE.TTF";

    textFont1 = loadFont(font1Filename, 8);
    textFont2 = loadFont(font1Filename, 8);
}

SDL_Texture* loadTexture(std::string path)
{
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL)
    {
        ol::error("Unable to load image " + path + " ! SDL_image Error: " + std::string(IMG_GetError()));
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(windowRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            ol::error("Unable to create texture from " + path + " ! SDL Error: " + std::string(SDL_GetError()));
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

static SDL_Texture* loadSprite(std::string filename)
{
    const std::string path = "res/graphic/";

    SDL_Texture* texture = loadTexture((path + filename).c_str());
    if (texture == NULL)
    {
        ol::error("could not load sprite \"" + path + filename + "\" !");
    }
    return texture;
}

static void loadAllSprites()
{
    sprites[ol::Sprite::TITLE] = loadSprite("title.png");
    sprites[ol::Sprite::TITLE_SHINE] = loadSprite("title_shine.png");
    sprites[ol::Sprite::NAMCO] = loadSprite("namco.png");

    sprites[ol::Sprite::FIGHTER] = loadSprite("fighter.png");
    sprites[ol::Sprite::FIGHTER_CAPTURED] = loadSprite("fighter_captured.png");
    sprites[ol::Sprite::FIGHTER_BULLET] = loadSprite("fighter_bullet.png");
    sprites[ol::Sprite::FIGHTER_EXPLOSION] = loadSprite("fighter_explosion.png");

    sprites[ol::Sprite::ENEMY_1] = loadSprite("enemy_1.png");
    sprites[ol::Sprite::ENEMY_1_HIT] = loadSprite("enemy_1_hit.png");
    sprites[ol::Sprite::ENEMY_2] = loadSprite("enemy_2.png");
    sprites[ol::Sprite::ENEMY_3] = loadSprite("enemy_3.png");
    sprites[ol::Sprite::ENEMY_3_MORPHING] = loadSprite("enemy_3_morphing.png");
    sprites[ol::Sprite::ENEMY_4] = loadSprite("enemy_4.png");
    sprites[ol::Sprite::ENEMY_5] = loadSprite("enemy_5.png");
    sprites[ol::Sprite::ENEMY_6] = loadSprite("enemy_6.png");
    sprites[ol::Sprite::ENEMY_7] = loadSprite("enemy_7.png");
    sprites[ol::Sprite::ENEMY_8] = loadSprite("enemy_8.png");
    sprites[ol::Sprite::ENEMY_9] = loadSprite("enemy_9.png");

    sprites[ol::Sprite::ENEMY_BULLET] = loadSprite("enemy_bullet.png");
    sprites[ol::Sprite::ENEMY_EXPLOSION] = loadSprite("enemy_explosion.png");
    sprites[ol::Sprite::ENEMY_BEAM] = loadSprite("enemy_beam.png");

    sprites[ol::Sprite::STAGE_SYMBOL_1] = loadSprite("stage_symbol_1.png");
    sprites[ol::Sprite::STAGE_SYMBOL_5] = loadSprite("stage_symbol_5.png");
    sprites[ol::Sprite::STAGE_SYMBOL_10] = loadSprite("stage_symbol_10.png");
    sprites[ol::Sprite::STAGE_SYMBOL_20] = loadSprite("stage_symbol_20.png");
    sprites[ol::Sprite::STAGE_SYMBOL_30] = loadSprite("stage_symbol_30.png");
    sprites[ol::Sprite::STAGE_SYMBOL_50] = loadSprite("stage_symbol_50.png");
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

    // destroy all SDL objects
    SDL_DestroyTexture(textFont1);
    SDL_DestroyTexture(textFont2);
    for (std::pair<ol::Sprite, SDL_Texture*> sprite : sprites)
    {
        SDL_DestroyTexture(sprite.second);
    }
}

void ol::Renderer::setTranslation(Vec2 translation)
{
    translation_ = translation;
}

void ol::Renderer::drawPoint(Vec2 position, int color)
{
    Vec2 finalPosition = position + translation_;
    SDL_Color c = intToColor(color);
    SDL_SetRenderDrawColor(windowRenderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawPoint(windowRenderer, finalPosition.x, finalPosition.y);
}

void ol::Renderer::drawRectangle(Vec2 position, Vec2 size, int color)
{
    Vec2 finalPosition = position + translation_;
    SDL_Color c = intToColor(color);
    SDL_SetRenderDrawColor(windowRenderer, c.r, c.g, c.b, c.a);
    SDL_Rect rect2 = {position.x, position.y, size.x, size.y};
    SDL_RenderDrawRect(windowRenderer, &rect2);
}

void ol::Renderer::drawSprite(ol::Sprite sprite, Vec2 dstPosition, Vec2 srcPosition, Vec2 srcSize)
{
    Vec2 finalPosition = dstPosition + translation_;
    SDL_Texture* spr = sprites[sprite];
    SDL_Rect src {srcPosition.x, srcPosition.y, srcSize.x, srcSize.y};
    SDL_Rect dst {finalPosition.x, finalPosition.y, srcSize.x, srcSize.y};
    SDL_RenderCopy(windowRenderer, spr, &src, &dst);
}

void ol::Renderer::drawText1(std::string text, Vec2 position, int color)
{
    Vec2 finalPosition = position + translation_;
    SDL_Color c = intToColor(color);
    SDL_SetTextureColorMod(textFont1, c.r, c.g, c.b);
    for (int i = 0; i < text.size(); i++)
    {
        int c = text[i];
        SDL_Rect src = {8 * c, 0, 8, 8};
        SDL_Rect dst = {finalPosition.x + 8 * i, finalPosition.y, 8, 8};
        SDL_RenderCopy(windowRenderer, textFont1, &src, &dst);
    }
}

void ol::Renderer::drawText2(std::string text, Vec2 position, int color)
{
    // TODO implement smaller font later
    drawText1(text, position, color);
}
