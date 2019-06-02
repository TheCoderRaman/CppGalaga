#include <map>

#include "olcPixelGameEngine.h"

#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"

#include "infra/Util.hpp"
#include "infra/Resources.hpp"
#include "infra/media_layer/AudioPlayer.hpp"

static std::map<ol::Music, int> musics;
static std::map<ol::Sound, int> sounds;

static int loadAudio(std::string filename)
{
    std::string path = "res/audio/format44k/";
    int soundId = olc::SOUND::LoadAudioSample(path + filename);
    if (soundId < 0)
    {
        ol::error("could not load audio \"" + path + filename + "\" !");
    }
    return soundId;
}

static bool initialize()
{
    olc::SOUND::InitialiseAudio(44100, 1, 8, 512);

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

static void playAudio(int soundId)
{
    olc::SOUND::PlaySample(soundId);
}

static void stopAudio(int soundId)
{
    olc::SOUND::StopSample(soundId);
}

static bool initialized = initialize();

void ol::playSound(ol::Sound sound)
{
    if (initialized)
    {
        playAudio(sounds[sound]);
    }
}
void ol::stopSound(ol::Sound sound)
{
    if (initialized)
    {
        stopAudio(sounds[sound]);
    }
}

void ol::playMusic(ol::Music music)
{
    if (initialized)
    {
        playAudio(musics[music]);
    }
}

void ol::stopMusic(ol::Music music)
{
    if (initialized)
    {
        stopAudio(musics[music]);
    }
}
