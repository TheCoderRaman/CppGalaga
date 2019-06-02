#ifndef RESOURCES_HPP
#define RESOURCES_HPP

namespace ol
{

    enum class Sprite
    {
        EMPTY,
        TITLE,
        TITLE_SHINE,
        NAMCO,

        FIGHTER,
        FIGHTER_CAPTURED,
        FIGHTER_BULLET,
        FIGHTER_EXPLOSION,

        ENEMY_1, // boss
        ENEMY_1_HIT, // boss hit
        ENEMY_2, // guard
        ENEMY_3, // minion
        ENEMY_3_MORPHING, // minion morphing
        ENEMY_4,
        ENEMY_5,
        ENEMY_6,
        ENEMY_7,
        ENEMY_8,
        ENEMY_9,

        ENEMY_BULLET,
        ENEMY_EXPLOSION,
        ENEMY_BEAM,

        STAGE_SYMBOL_1,
        STAGE_SYMBOL_5,
        STAGE_SYMBOL_10,
        STAGE_SYMBOL_20,
        STAGE_SYMBOL_30,
        STAGE_SYMBOL_50,
    };

    enum Sound
    {
        CHALLENGING_STAGE_SOUND,
        ENEMY_BEAM,
        ENEMY_BEAM_FIGHTER_CAPTURED,
        ENEMY_BOSS_DESTROYED,
        ENEMY_BOSS_FIRST_HIT,
        ENEMY_DIVING,
        ENEMY_GUARD_DESTROYED,
        ENEMY_METAMORPHOSE,
        ENEMY_MINION_DESTROYED,
        EXTRA_LIFE,
        FIGHTER_CAPTURED,
        FIGHTER_EXPLOSION,
        FIGHTER_RESCUED,
        FIGHTER_SHOT,
        FORMATION_SCALED,
        STAGE_SYMBOL,
        START_GAME,
    };

    enum Music
    {
        YOUTUBE_1K_SUBS_THANKS,
        START_GAME_MUSIC,
        CHALLENGING_STAGE_MUSIC,
        CHALLENGING_STAGE_PERFECT,
        GAME_OVER,
        HISCORE,
    };

}

#endif // RESOURCES_HPP
