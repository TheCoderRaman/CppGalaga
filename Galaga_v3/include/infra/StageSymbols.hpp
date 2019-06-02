#ifndef STAGESYMBOLS_HPP
#define STAGESYMBOLS_HPP

#include "infra/media_layer/Renderer.hpp"
#include "infra/Resources.hpp"

namespace ol
{

    struct StageSymbolInfo
    {
        Sprite sprite;
        int stage;
        Vec2 size;
    };

    extern const StageSymbolInfo STAGE_SYMBOLS_TABLE[];

    class StageSymbols
    {
        public:
            StageSymbols();
            virtual ~StageSymbols();

            void setStage(int stage);
            bool isFinished();
            int getStageTotalWidth();

            void update();

            void draw(Renderer& renderer);

        protected:

        private:
            int symbolsCount_[6];
            double symbolIndex_;
            int lastSymbolIndex_;
            bool isFinished_;
    };

}


#endif // STAGESYMBOLS_HPP
