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

#include "scr_playermethod.hpp"

void GScr_setVelocity(int entityIndex) {
    vec3_t vec;
    Scr_GetVector(0, vec);
    Entity *ent = game->getEntity(entityIndex);
    if(ent) {
        ent->toPlayerState();
        ent->set(POFF.VELOCITY, vec);
        ent->toEntityState();
    }
    Scr_AddUndefined();
}

void GScr_getVelocity(int entityIndex) {
    vec3_t vec;
    Entity *ent = game->getEntity(entityIndex);
    if(ent) {
        ent->toPlayerState();
        ent->get(POFF.VELOCITY, vec);
        ent->toEntityState();
        Scr_AddVector(vec);
    }
}

void GScr_getPlayerAngles(int entityIndex) {
    Entity *ent = game->getEntity(entityIndex);
    if(ent) {
        vec3_t angles;
        ent->get(POFF.ANGLES, angles);
        Scr_AddVector(angles);
    }
}
