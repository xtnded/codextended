#include "../script.hpp"

// gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod );
/*
    //param TODO
*/

/*
void die(int* self, int* inflictor, int* attacker, int damage, unsigned int mod) {
    int callback = callbackEntityKilled;
    ENTITY* ent = game->getEntity(*self);
    if(ent) {
        if(ent->die)
            callback = ent->die;
    }
    Script_AddInt(*attacker);
    Script_AddInt(*inflictor);
    Script_AddInt(damage);
    if(mod > 0x18)
        Script_AddString((char*)modNames[0]);
    else
        Script_AddString((char*)modNames[mod]);
    int result = Scr_ExecEntThread(*self, 0, callback, 4);
    Scr_FreeThread(result);
}

void pain(int* self, int* attacker, int damage, vec3_t point) {
    int callback = callbackEntityDamage;
    ENTITY* ent = game->getEntity(*self);
    if(ent) {
        if(ent->pain)
            callback = ent->pain;
    }
    //Com_Printf(">> pain [self=%p] [attacker=%p] [damage=%i] [point=%f %f %f]\n", self, attacker, damage, point);
    Script_AddInt(damage);
    Script_AddInt(*attacker);
    int result = Scr_ExecEntThread(*self, 0, callback, 2);
    Scr_FreeThread(result);
}

void GScr_nextthink(int a1) {
    ENTITY* ent = game->getEntity(a1);
    int next = Script_GetInt(0);
    if(ent) {
        ent->set(EOFF_NEXTTHINK, next);
    }
}

void GScr_callback(int a1) {
    ENTITY* ent = game->getEntity(a1);
    char* type = Script_GetString(0);
    int handle = 0;
    if(Script_GetNumParam() == 2)
        handle = Script_GetFunc(1);
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
*/

void EntCmd_setBounds(int a1) {
    ENTITY* ent = game->getEntity(a1);
    float width = Script_GetFloat(0);
    float height = Script_GetFloat(1);
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
        ent->set(280, &height, sizeof(float));
        ent->set(276, &width, sizeof(float));
        ent->set(272, &width, sizeof(float));
        width *= -1.0;
        ent->set(264, &width, sizeof(float));
        ent->set(260, &width, sizeof(float));
    }
    Script_AddUndefined();
}
/*
void EntCmd_setTakeDamage(int entityIndex) {
    ENTITY* ent = game->getEntity(entityIndex);
    if(ent) {
        int take = Script_GetInt(0);
        int p = 0;
        int d = 0;
        ent->set(EOFF_TAKEDAMAGE, take, sizeof(int));
        if(take) {
            p = (int)pain;
            d = (int)die;
        }
        ent->set(EOFF_PAIN, p, sizeof(int));
        ent->set(EOFF_DIE, d, sizeof(int));
    }
    Script_AddUndefined();
}*/
