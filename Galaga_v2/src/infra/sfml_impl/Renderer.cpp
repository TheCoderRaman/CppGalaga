#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "infra/Util.hpp"
#include "infra/Resources.hpp"
#include "infra/media_layer/Renderer.hpp"

extern sf::RenderWindow* window; // note: it's defined in Display.cpp

static sf::Font textFont1;
static sf::Font textFont2;
static std::map<ol::Sprite, std::shared_ptr<sf::Sprite>> sprites;
static std::vector<std::shared_ptr<sf::Texture>> textures;

static sf::Color intToColor(int c)
{
    int r = c & 255;
    int g = (c >> 8) & 255;
    int b = (c >> 16) & 255;
    return sf::Color(r, g, b, 255);
}

static void loadFonts()
{
    const std::string path = "res/font/";
    const std::string font1Filename = "PressStart2P.ttf";
    const std::string font2Filename = "PIXELADE.TTF";

    if (!textFont1.loadFromFile(path + font1Filename))
    {
        ol::error("could not load font \"" + path + "PressStart2P.ttf" + "\" !");
    }
    ((sf::Texture&) textFont1.getTexture(8)).setSmooth(false);

    if (!textFont2.loadFromFile(path + font2Filename))
    {
        ol::error("could not load font \"" + path + font2Filename + "\" !");
    }
    ((sf::Texture&) textFont2.getTexture(12)).setSmooth(false);
}

static std::shared_ptr<sf::Sprite> loadSprite(std::string filename)
{
    const std::string path = "res/graphic/";

    std::shared_ptr<sf::Texture> texture(new sf::Texture());
    textures.push_back(texture);

    if (!texture->loadFromFile(path + filename))
    {
        ol::error("could not load sprite \"" + path + filename + "\" !");
    }
    return std::make_shared<sf::Sprite>(*texture.get());
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
}

void ol::Renderer::setTranslation(Vec2 translation)
{
    //translation_ = translation;

    sf::View view = window->getView();
    view.reset(sf::FloatRect(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT));
    view.move(-translation.x, -translation.y);
    window->setView(view);
}

void ol::Renderer::drawPoint(Vec2 position, int color)
{
    static sf::CircleShape pointRenderer(0.5f);
    Vec2 finalPosition = position + translation_;
    pointRenderer.setFillColor(intToColor(color));
    pointRenderer.setPosition(finalPosition.x, finalPosition.y);
    window->draw(pointRenderer);
}

void ol::Renderer::drawRectangle(Vec2 position, Vec2 size, int color)
{
    static sf::RectangleShape rectangleRenderer;
    Vec2 finalPosition = position + translation_;
    rectangleRenderer.setPosition(finalPosition.x, finalPosition.y);
    rectangleRenderer.setSize(sf::Vector2f(size.x, size.y));
    rectangleRenderer.setFillColor(sf::Color::Transparent);
    rectangleRenderer.setOutlineThickness(1.0f);
    rectangleRenderer.setOutlineColor(intToColor(color));
    window->draw(rectangleRenderer);
}

void ol::Renderer::drawSprite(ol::Sprite sprite, Vec2 dstPosition, Vec2 srcPosition, Vec2 srcSize)
{
    Vec2 finalPosition = dstPosition + translation_;
    sf::Sprite& spr = *sprites[sprite];
    spr.setPosition(finalPosition.x, finalPosition.y);
    spr.setTextureRect(sf::IntRect(
        static_cast<int>(srcPosition.x), static_cast<int>(srcPosition.y),
        static_cast<int>(srcSize.x), static_cast<int>(srcSize.y)));
    spr.setOrigin(0.0f, 0.0f);
    window->draw(spr);

}

void ol::Renderer::drawText1(std::string text, Vec2 position, int color)
{
    static sf::Text textRenderer;
    Vec2 finalPosition = position + translation_;
    textRenderer.setColor(intToColor(color));
    textRenderer.setFont(textFont1);
    textRenderer.setCharacterSize(8);
    textRenderer.setString(text);
    textRenderer.setPosition(finalPosition.x, finalPosition.y);
    window->draw(textRenderer);
}

void ol::Renderer::drawText2(std::string text, Vec2 position, int color)
{
    static sf::Text textRenderer;
    Vec2 finalPosition = position + translation_;
    textRenderer.setColor(intToColor(color));
    textRenderer.setFont(textFont2);
    textRenderer.setCharacterSize(12);
    textRenderer.setString(text);
    textRenderer.setPosition(finalPosition.x, finalPosition.y - 5);
    window->draw(textRenderer);
}
