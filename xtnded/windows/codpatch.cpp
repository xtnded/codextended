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

#include "codpatch.hpp"

COD *game;

COD::COD(int v) {
    this->patch = v;
    if(patch == 1) {
        this->ENTITY_BASE = 0x2016F220;
        this->ENTITY_SIZE = 0x314;
        EOFF.PLAYERSTATE = 0x158;
        EOFF.TAKEDAMAGE = 0x171;
        syscall = ( int ( QDECL * )( int, ... ) )0x460230;
    } else {
        syscall = ( int ( QDECL * )( int, ... ) )0x4654C0;
    }
    for(int i = 0; i < MAX_ENTITY_SIZE; i++) {
        this->entities[i].index = i;
        this->entities[i].base = ENTITY_BASE + ENTITY_SIZE * i;
        this->entities[i].ptr = ENTITY_BASE + ENTITY_SIZE * i;
    }
}

Entity* COD::getEntities() {
    return entities;
}

Entity* COD::getEntity(int index) {
    return &entities[index];
}
