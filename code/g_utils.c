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

#include "script.h"
#include "server.h"
#include "bg_public.h"

G_FreeEntity_t G_FreeEntity;
G_Spawn_t G_Spawn;
G_SetAngle_t G_SetAngle;
G_SetOrigin_t G_SetOrigin;

void G_InitGentity(gentity_t *e) {
	e->inuse = qtrue;
	e->classname = scr_const->noclass;
	e->s.number = ((int)e - (int)g_entities / GENTITY_SIZE);
	e->ownerNum = ENTITYNUM_NONE;
	e->eventTime = 0;
	e->freeAfterEvent = qfalse;
}

void _Scr_FreeEntity(gentity_t *ent) {
	scr_entityfield_t *fields = (scr_entityfield_t*)( GAME("vmMain") + 0x28094 );
	for(int i = 0; fields[i].name; i++) {
		if(fields[i].type == 3)
			Scr_SetString((int*)((unsigned)ent + fields[i].offset), 0);
	}
	
	int i;
	for(i = 0; i < 6; i++) {
		*(unsigned char*)(i + (unsigned)ent + 748) = 0;
		Scr_SetString((int*)((unsigned)ent + (i*2) + 754), 0);
	}
	
	void (*FreeEntityNum)(int, int) = (void(*)(int,int isHudElem))GAME("Scr_FreeEntityNum");
	FreeEntityNum(ent->s.number, 0);
}

gentity_t *G_TempEntity(vec3_t origin, int event) {
	gentity_t       *e;
	vec3_t snapped;

	e = G_Spawn();
	e->s.eType = ET_EVENTS + event;

	e->classname = scr_const->tempEntity;
	
	e->eventTime = LEVELTIME;
	e->r_eventTime = LEVELTIME;
	e->freeAfterEvent = qtrue;

	VectorCopy( origin, snapped );
	SnapVector( snapped );      // save network bandwidth
	G_SetOrigin( e, snapped );

	T_LinkEntity( e );

	return e;
}