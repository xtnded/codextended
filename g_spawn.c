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

extern gitem_t *bg_itemlist;

typedef struct {
	char    *name;
	void ( *spawn )( gentity_t *ent );
} spawn_t;

void SP_misc_portal_camera(gentity_t*);
void SP_misc_portal_surface(gentity_t*);

spawn_t custom_spawns[] = {
	{"misc_portal_camera", SP_misc_portal_camera},
	{"misc_portal_surface", SP_misc_portal_surface},
	{NULL, NULL}
};

int (*G_SpawnFloat)( const char *key, const char *defaultString, float *out);
int (*G_SpawnString)( const char *key, const char *defaultString, char **out);
void (*G_SpawnItem)(gentity_t*,gitem_t*);

int G_CallSpawnEntity(gentity_t *ent) {
	char *classname = NULL;

	if(ent->classname) {
		classname = SL_ConvertToString(ent->classname);
		gitem_t *item = NULL;
		gitem_t *start = bg_itemlist;
		start += 12;
		for(item = start; item->classname; item += 12) {
			if(!strcmp(item->classname, classname)) {
				G_SpawnItem(ent, item);
				return 1;
			}
		}
		
		spawn_t *sp = (spawn_t*)GAME("spawns");
		
		for(;sp->name!=NULL;sp++) {
			if(!strcmp(sp->name, classname)) {
				sp->spawn(ent);
				return 1;
			}
		}
		
		for(sp = custom_spawns; sp->name != NULL; sp++) {
			if(!strcmp(sp->name, classname)) {
				cprintf(PRINT_UNDERLINE,"[SPAWN] %s",classname);
				sp->spawn(ent);
				return 1;
			}
		}
	}
	cprintf(PRINT_UNDERLINE | PRINT_WARN, "%s doesn't have a spawn function\n", SL_ConvertToString(ent->classname));
	return 0;
}

void SP_misc_portal_camera(gentity_t *ent) {
}

void SP_misc_portal_surface(gentity_t *ent) {
	VectorClear( ent->mins );
	VectorClear( ent->maxs );
	T_LinkEntity( ent );

	#define SVF_PORTAL 0x40
	ent->svFlags = ent->spawnflags;
	ent->s.eType = ET_PORTAL;

	#if 0
	if ( !ent->target ) {
		VectorCopy( ent->s.origin, ent->s.origin2 );
	} else {
		ent->think = locateCamera;
		ent->nextthink = level.time + 100;
	}
	#endif
	VectorCopy(ent->currentOrigin, ent->s.origin2);
}

void init_g_spawn() {
	G_SpawnFloat = (int(*)(const char*,const char*,float*))GAME("G_SpawnFloat");
	G_SpawnString = (int(*)(const char*,const char*,char**))GAME("G_SpawnString");
	G_SpawnItem = (void(*)(gentity_t*,gitem_t*))GAME("G_SpawnItem");
	
	__jmp( GAME("G_CallSpawnEntity"), (int)G_CallSpawnEntity);
}