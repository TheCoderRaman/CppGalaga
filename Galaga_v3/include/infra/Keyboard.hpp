#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "infra/Resources.hpp"

namespace ol
{

    enum class Key {
        LEFT,
        RIGHT,
        FIRE,
        PAUSE,
        EXIT,
        Q, W, E, R, T, Y, U, I, O, P // for debugging purposes
    };

    bool isKeyPressed(Key key);
    bool isKeyPressedOnce(Key key);

}

#endif // KEYBOARD_HPP

