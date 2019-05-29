#include <map>
#include "infra/Keyboard.hpp"

std::map<ol::Key, bool> keysDown;
std::map<ol::Key, bool> keysDownConsumed;

bool ol::isKeyPressed(ol::Key key)
{
    return keysDown[key];
}

bool ol::isKeyPressedOnce(ol::Key key)
{
    if (keysDown[key] && !keysDownConsumed[key])
    {
        keysDownConsumed[key] = true;
        return true;
    }
    return false;
}
