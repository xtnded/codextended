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

#ifndef CODPATCH_H
#define CODPATCH_H

#include "Entity.hpp"

/*
    Most offsets are the same for each patch
    Notice the very small difference in entityState size between the patches.
*/

struct {
    int VELOCITY = 0x20; //vec3_t
    int ANGLES = 0xC0; //vec3_t
} POFF;

class COD {
    int ENTITY_SIZE = 0x31c;
    int ENTITY_BASE = 0x201756A0;
    Entity entities[MAX_ENTITY_SIZE];
    int patch;
    public:
    COD(int v);
    //~MDLL();
    Entity* getEntities();
    Entity* getEntity(int);
};

extern COD *game;

#endif // CODPATCH_H
