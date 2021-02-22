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

#include "script.hpp"

// gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod );
/*
    //param TODO
*/
void die(int* self, int* inflictor, int* attacker, int damage, unsigned int mod) {
    int callback = callbackEntityKilled;
    Entity *ent = game->getEntity(*self);
    if(ent) {
        if(ent->die)
            callback = ent->die;
    }
    Scr_AddInt(*attacker);
    Scr_AddInt(*inflictor);
    Scr_AddInt(damage);
    if(mod > 0x18)
        Scr_AddString((char*)modNames[0]);
    else
        Scr_AddString((char*)modNames[mod]);
    int result = Scr_ExecEntThread(*self, 0, callback, 4);
    Scr_FreeThread(result);
}

void pain(int* self, int* attacker, int damage, vec3_t point) {
    int callback = callbackEntityDamage;
    Entity *ent = game->getEntity(*self);
    if(ent) {
        if(ent->pain)
            callback = ent->pain;
    }
    //Com_Printf(">> pain [self=%p] [attacker=%p] [damage=%i] [point=%f %f %f]\n", self, attacker, damage, point);
    Scr_AddInt(damage);
    Scr_AddInt(*attacker);
    int result = Scr_ExecEntThread(*self, 0, callback, 2);
    Scr_FreeThread(result);
}

void GScr_nextthink(int a1) {
    Entity *ent = game->getEntity(a1);
    int next = Scr_GetInt(0);
    if(ent) {
        ent->set(EOFF_NEXTTHINK, next);
    }
}

void GScr_callback(int a1) {
    Entity *ent = game->getEntity(a1);
    char* type = Scr_GetString(0);
    int handle = 0;
    if(Scr_GetNumParam() == 2)
        handle = Scr_GetFunc(1);
    if(!ent)
        return;
    if(strcmp(type, "think") == 0) {
        ent->think = handle;
    } else if(strcmp(type, "pain") == 0 || strcmp(type, "damage") == 0) {
        ent->pain = handle;
    } else if(strcmp(type, "die") == 0 || strcmp(type, "killed") == 0) {
        ent->die = handle;
    }
}

void ScrCmd_setBounds(int a1) {
    Entity *ent = game->getEntity(a1);
    float width = Scr_GetFloat(0);
    float height = Scr_GetFloat(1);
    if(ent) {
        /*
            int width, height;
            stackGetParamInt(2, &width);
            stackGetParamInt(3, &height);
            *(float*)(GENTITY_SIZE * a1 + g_entities + 280) = height;
            *(float*)(GENTITY_SIZE * a1 + g_entities + 276) = width;
            *(float*)(GENTITY_SIZE * a1 + g_entities + 272) = width;
            *(float*)(GENTITY_SIZE * a1 + g_entities + 264) = -width;
            *(float*)(GENTITY_SIZE * a1 + g_entities + 260) = -width;
            //syscall(53, (int*)(GENTITY_SIZE * a1 + g_entities));
        */
        ent->set(280, height);
        ent->set(276, width);
        ent->set(272, width);
        width *= -1.0;
        ent->set(264, width);
        ent->set(260, width);
    }
    Scr_AddUndefined();
}

void ScrCmd_setTakeDamage(int entityIndex) {
    Entity *ent = game->getEntity(entityIndex);
    if(ent) {
        int take = Scr_GetInt(0);
        int p = 0;
        int d = 0;
        ent->set(EOFF_TAKEDAMAGE, take);
        if(take) {
            p = (int)pain;
            d = (int)die;
        }
        ent->set(EOFF_PAIN, p);
        ent->set(EOFF_DIE, d);
    }
    Scr_AddUndefined();
}
