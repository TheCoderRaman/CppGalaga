#ifndef HUD_HPP
#define HUD_HPP

#include "infra/Renderer.hpp"
#include "infra/StageSymbols.hpp"

namespace ol
{

    struct HudInfo
    {
        int stage;
        int lives;
        int score;
        int hiscore;
        int nextExtraLifeScore;

        std::string getStageStr();
        std::string getLivesStr();
        std::string getScoreStr();
        std::string getHiscoreStr();

        void addScore(int point);
        void updateHiscore();
    };

    extern HudInfo hudInfo;

    class Hud
    {
        public:
            Hud();
            virtual ~Hud();

            void setStage(int stage);
            bool isStageSymbolsFinished();

            bool isLivesVisible();
            void setLivesVisible(bool isLivesVisible);

            bool set1UpBlinking(bool is1UpBlinkink);

            void update();

            void draw(Renderer& renderer);

            void show();
            void hide();

        protected:

        private:
            Vec2 getScaledPosition(Vec2 position);

            StageSymbols stageSymbols_;
            bool isLivesVisible_;
            bool is1UpBlinkink_;
            float scale_;
            float targetScale_;
    };

}

#endif // HUD_HPP
