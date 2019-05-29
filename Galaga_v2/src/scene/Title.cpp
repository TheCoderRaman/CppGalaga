#include <iostream>
#include <algorithm>
#include "infra/Util.hpp"
#include "infra/Vec2.hpp"
#include "infra/media_layer/AudioPlayer.hpp"
#include "infra/Keyboard.hpp"
#include "infra/SceneManager.hpp"
#include "scene/Title.hpp"

const std::string ol::Title::CREDIT_TEXT_1 = "(c) 2019 O.L.      ";
const std::string ol::Title::CREDIT_TEXT_2 = "(c) 1981 NAMCO LTD.";
const std::string ol::Title::START_TEXT_2 = "PRESS SPACE TO START";

ol::Title::Title(SceneManager& sceneManager) : Scene(sceneManager)
{
    //ctor
    //ol::log("creating Title Scene.");
}

ol::Title::~Title()
{
    //dtor
    //ol::log("destroying Title Scene.");
}

void ol::Title::onEnter()
{
    translation_ = Vec2(0.0f, SCREEN_HEIGHT);
    sceneManager_.getStarsBackground().move();
    sceneManager_.getHud().show();
    isStartTextVisible_ = false;
    titleShineFrame_ = 0;
}

void ol::Title::onExit()
{
}

void ol::Title::update()
{
    titleShineFrame_ += 0.2f;
    if (titleShineFrame_ > 14.0f)
    {
        titleShineFrame_ = 0;
    }

    if (isStartTextVisible_ && isKeyPressedOnce(Key::FIRE))
    {
        playSound(Sound::START_GAME);
        sceneManager_.changeScene(Scene::Id::STAGE);
    }

    translation_.y = clamp(translation_.y - 1.0f, 0, SCREEN_HEIGHT);
    if (translation_.y < 1.0f)
    {
        sceneManager_.getStarsBackground().stop();
        isStartTextVisible_ = true;
    }
}

void ol::Title::draw(Renderer& renderer)
{
    renderer.setTranslation(translation_);
    renderer.drawSprite(Sprite::TITLE, Vec2(48.0f, 88.0f),
                        Vec2(0.0f, 0.0f), Vec2(128.0f, 61.0f));

    renderer.drawSprite(Sprite::TITLE_SHINE, Vec2(109.0f, 91.0f),
                        Vec2(36.0f * static_cast<int>(titleShineFrame_), 0.0f),
                        Vec2(36.0f, 36.0f));

    renderer.drawText1(CREDIT_TEXT_1, ol::Vec2(36.0f, 240.0f), ol::Color::CYAN);

    renderer.drawText1(CREDIT_TEXT_2, ol::Vec2(36.0f, 252.0f), ol::Color::CYAN);

    renderer.drawSprite(Sprite::NAMCO, Vec2(82.0f, 264.0f),
                        Vec2(0.0f, 0.0f), Vec2(60.0f, 8.0f));
    if (isStartTextVisible_ && blink())
    {
        renderer.drawText1(START_TEXT_2, ol::Vec2(32.0f, 160.0f), ol::Color::WHITE);
    }
    renderer.setTranslation(Vec2(0.0f, 0.0f));
}
