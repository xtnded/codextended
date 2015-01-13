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

int callbackEntityDamage, callbackEntityKilled;

#if 0

static void use( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	int callback;
	ENTITY* entity = game->getEntity(ent->s.number);
	if(!entity)
		return;
	if(!entity->use)
		return;
	callback = entity->use;
	Scr_AddEntity(activator);
	int result = Scr_ExecEntThread(ent->s.number, 0, callback, 1);
	Scr_FreeThread(result);
}

#endif

static void die(gentity_t* self, gentity_t* inflictor, gentity_t* attacker, int damage, unsigned int mod) {
	if(!callbackEntityKilled) {
		Scr_Error("ERROR: EntityDeath(eAttacker, eInflictor, iDamage, sMeansOfDeath) was not found!");
		return;
	}
    //printf(">> DIE [self=%p] [attacker=%p] [damage=%i] [mod=%d]\n", self, attacker, damage, mod);
    if(mod > 0x18)
        Scr_AddString((char*)modNames[0]);
    else
        Scr_AddString((char*)modNames[mod]);
    Scr_AddInt(damage);
    Scr_AddEntity(inflictor);
    Scr_AddEntity(attacker);
    int result = Scr_ExecEntThread(self->s.number, 0, callbackEntityKilled, 4);
    Scr_FreeThread(result);
}

static void pain(gentity_t* self, gentity_t* attacker, int damage, vec3_t point) {
	if(!callbackEntityDamage) {
		Scr_Error("ERROR: EntityDamage(eAttacker, vPoint, iDamage) was not found!");
		return;
	}
    //Com_Printf(">> pain [self=%p] [attacker=%p] [damage=%i] [point=%f %f %f]\n", self, attacker, damage, point);
    Scr_AddInt(damage);
	Scr_AddVector(point);
	Scr_AddEntity(attacker);
    int result = Scr_ExecEntThread(self->s.number, 0, callbackEntityDamage, 2);
    Scr_FreeThread(result);
}

#if 0
// maybe later again
void ScriptEnt_callback(int a1) {
    ENTITY* ent = game->getEntity(a1);
    char* type = Scr_GetString(0);
    int handle = 0;
    if(Scr_GetNumParam() == 2)
        handle = Scr_GetFunc(1);
    if(!ent)
        return;
	int use_as_int = 0;
    if(strcmp(type, "think") == 0) {
        ent->think = handle;
	} else if(!strcmp(type, "use")) {
		if(handle)
			use_as_int = (int)use;
        ent->set(EOFF_USE, &use_as_int, sizeof(int));
		ent->use = handle;
    } else if(strcmp(type, "pain") == 0 || strcmp(type, "damage") == 0) {
        ent->pain = handle;
    } else if(strcmp(type, "die") == 0 || strcmp(type, "killed") == 0) {
        ent->die = handle;
    }
}
#endif

void ScriptEnt_SetLight(int ent) {
	gentity_t *e = &g_entities[ent];
	int r, g, b, i;
	r = Scr_GetInt(0);
	g = Scr_GetInt(1);
	b = Scr_GetInt(2);
	i = Scr_GetInt(3);
	
	e->s.constantLight = r | ( g << 8 ) | ( b << 16 ) | ( i << 24 );
}

void ScriptEnt_PlayAnim(int ent) {
	gentity_t *e = &g_entities[ent];
}

void ScriptEnt_SetBounds(int entityNum) {
    float width = Scr_GetFloat(0);
    float height = Scr_GetFloat(1);
    gentity_t *ent = &g_entities[entityNum];
	
	vec3_t mins = {-width,-width,0};
	vec3_t maxs = {width,width,height};
	
	VectorCopy(mins,ent->mins);
	VectorCopy(maxs,ent->maxs);
	VectorCopy(mins,ent->absmin);
	VectorCopy(maxs,ent->absmax);
}

void ScriptEnt_SetMins(int num) {
	gentity_t *ent = &g_entities[num];
	vec3_t vec;
	Scr_GetVector(0, vec);
	VectorCopy(vec,ent->mins);
}

void ScriptEnt_SetMaxs(int num) {
	gentity_t *ent = &g_entities[num];
	vec3_t vec;
	Scr_GetVector(0, vec);
	VectorCopy(vec,ent->maxs);
}

void ScriptEnt_SetAbsMax(int num) {
	gentity_t *ent = &g_entities[num];
	vec3_t vec;
	Scr_GetVector(0, vec);
	VectorCopy(vec,ent->absmax);
}

void ScriptEnt_SetAbsMin(int num) {
	gentity_t *ent = &g_entities[num];
	vec3_t vec;
	Scr_GetVector(0, vec);
	VectorCopy(vec,ent->absmin);
}

void ScriptEnt_SetTakeDamage(int entityIndex) {
    gentity_t *ent = &g_entities[entityIndex];
	int flag = Scr_GetInt(0);
	int p = (int)pain, d = (int)die;
	
	if(!flag)
		p = d = 0;
	ent->takedamage = flag;
	ENT_SET(ent, EOFF_PAIN, &p, sizeof(int));
	ENT_SET(ent, EOFF_DIE, &d, sizeof(int));
}
