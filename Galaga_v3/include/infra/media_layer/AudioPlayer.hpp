#ifndef AUDIO_PLAYER_HPP
#define AUDIO_PLAYER_HPP

#include "infra/Resources.hpp"

namespace ol
{

    void initAudio();
    void closeAudio();

    void playSound(ol::Sound sound);
    void stopSound(ol::Sound sound);

    void playMusic(ol::Music music);
    void stopMusic(ol::Music music);

}

#endif // AUDIO_PLAYER_HPP
