#include <map>
#include <memory>
#include <SFML/Audio.hpp>
#include "infra/Util.hpp"
#include "infra/Resources.hpp"
#include "infra/media_layer/AudioPlayer.hpp"

#define AUDIO_POOL_SIZE 30

static std::map<ol::Sound, std::shared_ptr<sf::SoundBuffer>> sounds;
static std::map<ol::Music, std::shared_ptr<sf::SoundBuffer>> musics;
static sf::Sound players[AUDIO_POOL_SIZE];

static std::shared_ptr<sf::SoundBuffer> loadAudio(std::string filename)
{
    std::string path = "res/audio/";
    std::shared_ptr<sf::SoundBuffer> soundBuffer(new sf::SoundBuffer());
    if (!soundBuffer->loadFromFile(path + filename))
    {
        ol::error("could not load audio \"" + path + filename + "\" !");
    }
    return soundBuffer;
}

static bool initialize()
{
    sounds[ol::Sound::CHALLENGING_STAGE_SOUND] = loadAudio("challenging_stage.wav");
    sounds[ol::Sound::ENEMY_BEAM] = loadAudio("enemy_beam.wav");
    sounds[ol::Sound::ENEMY_BEAM_FIGHTER_CAPTURED] = loadAudio("enemy_beam_fighter_captured.wav");
    sounds[ol::Sound::ENEMY_BOSS_DESTROYED] = loadAudio("enemy_boss_destroyed.wav");
    sounds[ol::Sound::ENEMY_BOSS_FIRST_HIT] = loadAudio("enemy_boss_first_hit.wav");
    sounds[ol::Sound::ENEMY_DIVING] = loadAudio("enemy_diving.wav");
    sounds[ol::Sound::ENEMY_GUARD_DESTROYED] = loadAudio("enemy_guard_destroyed.wav");
    sounds[ol::Sound::ENEMY_METAMORPHOSE] = loadAudio("enemy_metamorphose.wav");
    sounds[ol::Sound::ENEMY_MINION_DESTROYED] = loadAudio("enemy_minion_destroyed.wav");
    sounds[ol::Sound::EXTRA_LIFE] = loadAudio("extra_life.wav");
    sounds[ol::Sound::FIGHTER_CAPTURED] = loadAudio("fighter_captured.wav");
    sounds[ol::Sound::FIGHTER_EXPLOSION] = loadAudio("fighter_explosion.wav");
    sounds[ol::Sound::FIGHTER_RESCUED] = loadAudio("fighter_rescued.wav");
    sounds[ol::Sound::FIGHTER_SHOT] = loadAudio("fighter_shot.wav");
    sounds[ol::Sound::FORMATION_SCALED] = loadAudio("formation_scaled.wav");
    sounds[ol::Sound::STAGE_SYMBOL] = loadAudio("stage_symbol.wav");
    sounds[ol::Sound::START_GAME] = loadAudio("start_game.wav");

    musics[ol::Music::YOUTUBE_1K_SUBS_THANKS] = loadAudio("youtube_1k_subs_thanks.wav");
    musics[ol::Music::START_GAME_MUSIC] = loadAudio("start_game_music.wav");
    musics[ol::Music::CHALLENGING_STAGE_MUSIC] = loadAudio("challenge_stage.wav");
    musics[ol::Music::CHALLENGING_STAGE_PERFECT] = loadAudio("challenge_stage_perfect.wav");
    musics[ol::Music::GAME_OVER] = loadAudio("game_over.wav");
    musics[ol::Music::HISCORE] = loadAudio("hiscore.wav");

    return true;
}

static void playAudio(sf::SoundBuffer* soundBuffer)
{
    for (int i = 0; i < AUDIO_POOL_SIZE; i++)
    {
        if (players[i].getStatus() != sf::Sound::Playing)
        {
            players[i].setBuffer(*soundBuffer);
            players[i].play();
            break;
        }
    }
}

static void stopAudio(sf::SoundBuffer* soundBuffer)
{
    for (int i = 0; i < AUDIO_POOL_SIZE; i++)
    {
        if (players[i].getBuffer() == soundBuffer)
        {
            players[i].stop();
        }
    }
}

static bool initialized = initialize();

void ol::playSound(ol::Sound sound)
{
    if (initialized)
    {
        playAudio(sounds[sound].get());
    }
}

void ol::stopSound(ol::Sound sound)
{
    if (initialized)
    {
        stopAudio(sounds[sound].get());
    }
}

void ol::playMusic(ol::Music music)
{
    if (initialized)
    {
        playAudio(musics[music].get());
    }
}

void ol::stopMusic(ol::Music music)
{
    if (initialized)
    {
        stopAudio(musics[music].get());
    }
}
