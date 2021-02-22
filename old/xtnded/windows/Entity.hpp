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

#define PLAYERSTATE_SIZE 0x22cc
#define GENTITY_SIZE 0x31c
#define PLAYERSTATE_OFFSET 0xCADF4

static int g_entities = 0x201756A0;
static int playerstates = 0x20242760;

/*
    :: Will be done later

typedef struct entityState_s {
    int number;
    int eType;
    int eFlags;
    char _gc[10];
    vec3_t origin;
} entityState_t

typedef struct gentity_s gentity_t;
typedef struct gclient_s gclient_t;

struct gentity_s {
    //size 788
    entityState_t s;
} gentity_t;
*/

struct {
    int PLAYERSTATE = 0x15C;
    int TAKEDAMAGE = 0x175;
} EOFF;

struct Entity {
    int index;
    BOOL ps = false;
    int base;
    int ptr;
    int getPlayerState();
    template <typename T>
    void set(int, T&);
    template <typename T>
    void get(int, T&);
    void switchState();
    void toPlayerState();
    void toEntityState();
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
