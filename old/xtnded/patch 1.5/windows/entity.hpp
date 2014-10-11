/*
===========================================================================
    Copyright (C) riicchhaarrd
    Copyright (C) 1999-2005 Id Software, Inc.

    This file is part of CoDExtended source code.

    CoDExtended source code is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    CoDExtended source code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
===========================================================================
*/
#ifndef ENTITY_H
#define ENTITY_H

#include "include.hpp"

#define MAX_ENTITY_SIZE 1024

#define g_entities 0x201756A0
#define playerstates 0x20242760
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

struct Entity {
    int index;
    BOOL ps = false;
    int base;
    int ptr;
    int nextthink = 0;
    int think = 0;
    int pain = 0;
    int die = 0;
    int use = 0;
    int touch = 0;
    int getPlayerState();
    template <typename T>
    void set(int, T&);
    template <typename T>
    void get(int, T&);
    void switchState();
    void toPlayerState();
    void toEntityState();
    BOOL isPlayer();
    BOOL isEntity();
};

template <typename T>
void Entity::set(int offset, T& v) {
    memcpy((void*)(ptr+offset), &v, sizeof(v));
}

template <typename T>
void Entity::get(int offset, T& v) {
    memcpy(&v, (void*)(ptr+offset), sizeof(v));
}

#endif // ENTITY_H
