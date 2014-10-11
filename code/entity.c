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
#include "entity.h"

int gentities;
extern ENTITY* this_entity;

int Entity_getPlayerState() {
    if(!this_entity->base)
        return 0;
    return *(int*)(this_entity->base+EOFF_PLAYERSTATE);
}

qboolean Entity_isPlayer() {
    int client = *(int*)(this_entity->base+EOFF_PLAYERSTATE);
    return client==0;
}

qboolean Entity_isEntity() {
    return((*(int*)this_entity->base) == this_entity->index);
}

void Entity_switchState() {
    if(this_entity->ps)
        this_entity->ptr = this_entity->getPlayerState();
    else
        this_entity->ptr = this_entity->base;
    this_entity->ps = !this_entity->ps;
}

void Entity_toPlayerState() {
    this_entity->ptr = this_entity->getPlayerState();
    this_entity->ps = true;
}

void Entity_toEntityState() {
    this_entity->ptr = this_entity->base;
    this_entity->ps = false;
}

void Entity_set(int offset, void* v, unsigned int len) {
    memcpy((void*)(this_entity->ptr+offset), v, len);
}

void Entity_get(int offset, void* v, unsigned int len) {
    memcpy(v, (void*)(this_entity->ptr+offset), len);
}

void Entity_setFuncPointers(ENTITY* ent) { //because we're using C and no use of classes ->set directly -Richard
	ent->get = &Entity_get;
	ent->set = &Entity_set;
	ent->toEntityState = &Entity_toEntityState;
	ent->toPlayerState = &Entity_toPlayerState;
	ent->switchState = &Entity_switchState;
	ent->getPlayerState = &Entity_getPlayerState;
	ent->isEntity = &Entity_isEntity;
	ent->isPlayer = &Entity_isPlayer;
}
