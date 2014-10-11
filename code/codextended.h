/*
	This file is part of CoDExtended.

    CoDExtended is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CoDExtended is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CoDExtended.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef CODEXTENDED_H
#define CODEXTENDED_H

#include "shared.h"
#include "script.h"
#include "entity.h"

typedef struct {
    ENTITY entities[MAX_ENTITY_SIZE];
    ENTITY* (*getEntities)();
    ENTITY* (*getEntity)(unsigned short);
	/*~CODEXTENDED() {
		//printf("Call of Duty destructor.\n");
	}*/
} CODEXTENDED;

extern CODEXTENDED* game;

#endif // CODEXTENDED_H
