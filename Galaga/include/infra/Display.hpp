#ifndef DISPLAY_HPP
#define DISPLAY_HPP

namespace ol
{

    class Display
    {
        public:
            const static int SCREEN_WIDTH = 224;
            const static int SCREEN_HEIGHT = 288;
            constexpr static float SCREEN_SCALE = 2.0f;

            Display();
            virtual ~Display();
            bool isOpen();
            void handleWindowEvents();
            void clear();
            void show();

        protected:

        private:

    };

}

#endif // DISPLAY_HPP
