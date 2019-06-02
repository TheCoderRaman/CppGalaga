#include <map>
#include <memory>
#include <SDL2/SDL_mixer.h>
#include "infra/Util.hpp"
#include "infra/Resources.hpp"
#include "infra/media_layer/AudioPlayer.hpp"

static bool initialize();

static std::map<ol::Sound, Mix_Chunk*> sounds;
static std::map<ol::Music, Mix_Music*> musics;
static bool initialized = false;

void ol::initAudio()
{
    //Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0)
    {
        ol::error("SDL_mixer could not initialize! SDL_mixer Error: " + std::string(SDL_GetError()));
    }
    Mix_AllocateChannels(20);
    initialized = initialize();
}

void ol::closeAudio()
{
    for (std::pair<ol::Sound, Mix_Chunk*> sound : sounds)
    {
        Mix_FreeChunk(sound.second);
    }
    for (std::pair<ol::Music, Mix_Music*> music : musics)
    {
        Mix_FreeMusic(music.second);
    }
    Mix_Quit();
}

static Mix_Chunk* loadSound(std::string filename)
{
    std::string path = "res/audio/format44k/";

    // load sound
    Mix_Chunk* sound = Mix_LoadWAV((path + filename).c_str());
    if(sound == NULL)
    {
        ol::error("Failed to load sound " + (path + filename) + " !");
    }

    return sound;
}

static Mix_Music* loadMusic(std::string filename)
{
    std::string path = "res/audio/format44k/";

    // load music
    Mix_Music* music = Mix_LoadMUS((path + filename).c_str());
    if(music == NULL)
    {
        ol::error("Failed to load music " + (path + filename) + " !");
    }

    return music;
}

static bool initialize()
{
    sounds[ol::Sound::CHALLENGING_STAGE_SOUND] = loadSound("challenging_stage.wav");
    sounds[ol::Sound::ENEMY_BEAM] = loadSound("enemy_beam.wav");
    sounds[ol::Sound::ENEMY_BEAM_FIGHTER_CAPTURED] = loadSound("enemy_beam_fighter_captured.wav");
    sounds[ol::Sound::ENEMY_BOSS_DESTROYED] = loadSound("enemy_boss_destroyed.wav");
    sounds[ol::Sound::ENEMY_BOSS_FIRST_HIT] = loadSound("enemy_boss_first_hit.wav");
    sounds[ol::Sound::ENEMY_DIVING] = loadSound("enemy_diving.wav");
    sounds[ol::Sound::ENEMY_GUARD_DESTROYED] = loadSound("enemy_guard_destroyed.wav");
    sounds[ol::Sound::ENEMY_METAMORPHOSE] = loadSound("enemy_metamorphose.wav");
    sounds[ol::Sound::ENEMY_MINION_DESTROYED] = loadSound("enemy_minion_destroyed.wav");
    sounds[ol::Sound::EXTRA_LIFE] = loadSound("extra_life.wav");
    sounds[ol::Sound::FIGHTER_CAPTURED] = loadSound("fighter_captured.wav");
    sounds[ol::Sound::FIGHTER_EXPLOSION] = loadSound("fighter_explosion.wav");
    sounds[ol::Sound::FIGHTER_RESCUED] = loadSound("fighter_rescued.wav");
    sounds[ol::Sound::FIGHTER_SHOT] = loadSound("fighter_shot.wav");
    sounds[ol::Sound::FORMATION_SCALED] = loadSound("formation_scaled.wav");
    sounds[ol::Sound::STAGE_SYMBOL] = loadSound("stage_symbol.wav");
    sounds[ol::Sound::START_GAME] = loadSound("start_game.wav");

    musics[ol::Music::YOUTUBE_1K_SUBS_THANKS] = loadMusic("youtube_1k_subs_thanks.wav");
    musics[ol::Music::START_GAME_MUSIC] = loadMusic("start_game_music.wav");
    musics[ol::Music::CHALLENGING_STAGE_MUSIC] = loadMusic("challenge_stage.wav");
    musics[ol::Music::CHALLENGING_STAGE_PERFECT] = loadMusic("challenge_stage_perfect.wav");
    musics[ol::Music::GAME_OVER] = loadMusic("game_over.wav");
    musics[ol::Music::HISCORE] = loadMusic("hiscore.wav");

    return true;
}

void ol::playSound(ol::Sound sound)
{
    if (initialized)
    {
        Mix_Chunk* s = sounds[sound];
        Mix_PlayChannel(sound, s, 0);
    }
}

void ol::stopSound(ol::Sound sound)
{
    if (initialized)
    {
        Mix_HaltChannel(sound);
    }
}

void ol::playMusic(ol::Music music)
{
    if (initialized)
    {
        Mix_Music* m = musics[music];
        Mix_PlayMusic(m, 0);
    }
}

void ol::stopMusic(ol::Music music)
{
    if (initialized)
    {
        Mix_HaltMusic();
    }
}
