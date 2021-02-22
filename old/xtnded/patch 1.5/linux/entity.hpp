#ifndef ENTITY_H
#define ENTITY_H

#include "game/shared.hpp"

extern int gentities;

#define MAX_ENTITY_SIZE 1024
#define PLAYERSTATE_SIZE 0x22cc
#define GENTITY_SIZE 0x31c
#define PLAYERSTATE_OFFSET 0xCADF4

typedef enum {
    EOFF_S_ETYPE = 4,
    EOFF_EFLAGS = 8,
    EOFF_PLAYERSTATE = 348,
    EOFF_TAKEDAMAGE = 373,
    EOFF_NEXTTHINK = 516,
    EOFF_THINK = 520,
    EOFF_USE = 536,
    EOFF_PAIN = 540,
    EOFF_DIE = 544,
    EOFF_HEALTH = 568,
    EOFF_FLAGS = 388,
    EOFF_R_CONTENTS = 284,
    EOFF_R_SVFLAGS = 244,
    EOFF_TEAMMASTER = 620,
    EOFF_TEAMCHAIN = 616,
    EOFF_TEAM = 480,
    EOFF_INUSE = 356,
    EOFF_CLASSNAME = 380, //it's a string index of scr_
    EOFF_R_OWNERNUM = 336,
    EOFF_EVENTTIME = 392,
    EOFF_FREEAFTEREVENT = 396,
    EOFF_WAIT = 624,
    EOFF_RANDOM = 628,
    EOFF_CLIENTNUM = 144,
    EOFF_ORIGIN = 312,
    EOFF_ANGLES = 324,
    EOFF_PHYSICSOBJECT = 357,
    EOFF_UNLINKAFTEREVENT = 400,

    EOFF_GROUNDENTITYNUM = 124,
    EOFF_LOOPSOUND = 132,
    EOFF_SOUND_OPENING = 359,
    EOFF_SOUND_CLOSING = 360,
    EOFF_SOUND_OPEN_END = 361,
    EOFF_SOUND_OPEN_LOOP = 363,
    EOFF_SOUND_CLOSE_LOOP = 364,
    EOFF_SOUND_LOCKED = 365,
    EOFF_SOUND_OPENING_QUIET = 366,
    EOFF_SOUND_OPEN_QUIET_END = 367,
    EOFF_SOUND_CLOSING_QUIET = 368,
    EOFF_SOUND_CLOSE_QUIET_END = 369,

    EOFF_MOVERSTATE = 376,
    EOFF_SPAWNFLAGS = 384,
    EOFF_DAMAGE = 576,
    EOFF_SPEED = 488,
    EOFF_SPLASHDAMAGE = 580,
    EOFF_R_CURRENTORIGIN = 312,
    EOFF_PARENT = 416,
    EOFF_SPLASHRADIUS = 584,
    EOFF_SPLASHMETHODOFDEATH = 588,
    EOFF_R_EVENTTIME = 340

} ENTITY_OFFSET;

typedef enum {
    POFF_VELOCITY = 0x20,
    POFF_ANGLES = 0xC0
} PLAYER_OFFSET;

struct ENTITY {
    int index;
    bool ps;
    int base;
    int ptr;
    int nextthink;
    int think;
    int pain;
    int die;
    int use;
    int touch;
    int getPlayerState();
    void set(int, void*, unsigned int);
    void get(int, void*, unsigned int);
    void switchState();
    void toPlayerState();
    void toEntityState();
    bool isPlayer();
    bool isEntity();
};

#endif // ENTITY_H
