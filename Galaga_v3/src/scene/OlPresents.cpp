#include <iostream>
#include <algorithm>
#include "infra/Util.hpp"
#include "infra/Vec2.hpp"
#include "infra/Keyboard.hpp"
#include "infra/media_layer/AudioPlayer.hpp"
#include "infra/SceneManager.hpp"
#include "scene/OlPresents.hpp"

const std::string ol::OlPresents::OL_PRESENTS_TEXT = "O.L. PRESENTS";
const std::string ol::OlPresents::YT_1K_TEXT_1 = "Special +1K YouTube Subs !";
const std::string ol::OlPresents::YT_1K_TEXT_2 = "THANKS TO ALL SUBSCRIBERS :)";

ol::OlPresents::OlPresents(SceneManager& sceneManager) : Scene(sceneManager)
{
    //ctor
    for (int i = 0; i < OL_PRESENTS_TEXT.size(); i++)
    {
        Letter letter;
        letter.character = OL_PRESENTS_TEXT.substr(i, 1);
        letter.path.clear();
        letter.path.addPoint(Vec2(112.0f, 450.0f));
        letter.path.addPoint(Vec2(112.0f + 60.0f, 350.0f));
        letter.path.addPoint(Vec2(112.0f - 20.0f, 250.0f));
        letter.path.addPoint(Vec2(112.0f + 20.0f, 210.0f));
        letter.path.addPoint(Vec2(112.0f, 180.0f));
        letter.path.addPoint(Vec2(i * 8 + 60.0f, 128.0f));
        letters_.push_back(letter);
    }
    std::random_shuffle(letters_.begin(), letters_.end());
    //ol::log("creating OlPresents Scene.");
}

ol::OlPresents::~OlPresents()
{
    //dtor
    //ol::log("destroying OlPresents Scene.");
}

void ol::OlPresents::onEnter()
{
    for (int i = 0; i < letters_.size(); i++)
    {
        Letter& letter = letters_[i];
        letter.pathT = 0.0f;
        int k = 10 * (OL_PRESENTS_TEXT.size() - i);
        for (int a = 0; a < k; a++)
        {
            letter.path.advance(letter.pathT, 1.0f);
        }
    }
    isLettersPathFinished_ = false;
    isYt1KTextVisible_ = false;
    isLettersVisible_ = true;
}

void ol::OlPresents::onExit()
{
}

void ol::OlPresents::update()
{
    if (isLettersPathFinished_)
    {
        if  (getGameElapsedTime() - lettersFinishedTime_ > 6.0f)
        {
            sceneManager_.changeScene(Scene::Id::TITLE);
        }
        else if  (getGameElapsedTime() - lettersFinishedTime_ > 5.0f)
        {
            isLettersVisible_ = false;
            isYt1KTextVisible_ = false;
        }
        else if  (getGameElapsedTime() - lettersFinishedTime_ > 1.0f)
        {
            if (!isYt1KTextVisible_)
            {
                // playMusic(Music::YOUTUBE_1K_SUBS_THANKS);
            }
            isYt1KTextVisible_ = true;
        }
        return;
    }

    isLettersPathFinished_ = true;
    for (int i = 0; i < letters_.size(); i++)
    {
        Letter& letter = letters_[i];
        letter.path.advance(letter.pathT, 2.0f);
        isLettersPathFinished_ &= letter.path.isFinished(letter.pathT);
    }

    if (isLettersPathFinished_)
    {
        lettersFinishedTime_ = getGameElapsedTime();
    }
}

void ol::OlPresents::draw(Renderer& renderer)
{
    if (isLettersVisible_)
    {
        for (int i = 0; i < letters_.size(); i++)
        {
            Letter& letter = letters_[i];
            Vec2 position = letter.path.getPoint(letter.pathT);
            renderer.drawText1(letter.character, position, Color::WHITE);
            // letter.path.draw(renderer);
        }
    }
    if (isYt1KTextVisible_)
    {
        //renderer.drawText1(YT_1K_TEXT_1, Vec2(8.0f, 140.0f), Color::RED);
        //renderer.drawText1(YT_1K_TEXT_2, Vec2(0.0f, 160.0f), Color::YELLOW);
    }
}
