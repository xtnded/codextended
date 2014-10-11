#include "../script.hpp"

// gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod );
/*
    //param TODO
*/

void use( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	int callback = callbackEntityUse;
	ENTITY* entity = game->getEntity(*ent);
	if(entity) {
		if(entity->use)
			callback = entity->use;
	}
	Script_AddEntity(activator);
	int result = Script_ExecEntThread(*ent, 0, callback, 1);
	Script_FreeThread(result);
}

void die(int* self, gentity_t* inflictor, gentity_t* attacker, int damage, unsigned int mod) {
    int callback = callbackEntityKilled;
    ENTITY* ent = game->getEntity(*self);
    if(ent) {
        if(ent->die)
            callback = ent->die;
    }
    //printf(">> DIE [self=%p] [attacker=%p] [damage=%i] [mod=%d]\n", self, attacker, damage, mod);
    if(mod > 0x18)
        Script_AddString((char*)modNames[0]);
    else
        Script_AddString((char*)modNames[mod]);
    Script_AddInt(damage);
    Script_AddEntity(inflictor);
    Script_AddEntity(attacker);
    int result = Script_ExecEntThread(*self, 0, callback, 4);
    Script_FreeThread(result);
}

void pain(int* self, gentity_t* attacker, int damage, vec3_t point) {
    int callback = callbackEntityDamage;
    ENTITY* ent = game->getEntity(*self);
    if(ent) {
        if(ent->pain)
            callback = ent->pain;
    }
    //Com_Printf(">> pain [self=%p] [attacker=%p] [damage=%i] [point=%f %f %f]\n", self, attacker, damage, point);
    Script_AddInt(damage);
	Script_AddVector(point);
	Script_AddEntity(attacker);
    int result = Script_ExecEntThread(*self, 0, callback, 2);
    Script_FreeThread(result);
}

void EntCmd_nextthink(int a1) {
    ENTITY* ent = game->getEntity(a1);
    int next = Script_GetInt(0);
    if(ent) {
        ent->set(EOFF_NEXTTHINK, &next, sizeof(next));
    }
}

void EntCmd_callback(int a1) {
    ENTITY* ent = game->getEntity(a1);
    char* type = Script_GetString(0);
    int handle = 0;
    if(Script_GetNumParam() == 2)
        handle = Script_GetFunc(1);
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

void EntCmd_setBoundCorners(int a1) {
    ENTITY *ent = game->getEntity(a1);
    float c1 = Script_GetFloat(0);
    float c2 = Script_GetFloat(1);
    float c3 = Script_GetFloat(2);
    float c4 = Script_GetFloat(3);
    float c5 = Script_GetFloat(4);
    float c6 = Script_GetFloat(5);
    if(ent) {
        ent->set(280, &c1, sizeof(float));
        ent->set(276, &c2, sizeof(float));
        ent->set(272, &c3, sizeof(float));
        ent->set(268, &c4, sizeof(float));
        ent->set(264, &c5, sizeof(float));
        ent->set(260, &c6, sizeof(float));
    }
    Script_AddUndefined();
}


void EntCmd_setTakeDamage(int entityIndex) {
    ENTITY* ent = game->getEntity(entityIndex);
    if(ent) {
        int take = Script_GetInt(0);
        int p = 0;
        int d = 0;
        ent->set(EOFF_TAKEDAMAGE, &take, sizeof(int));
        if(take) {
            p = (int)pain;
            d = (int)die;
        }
        ent->set(EOFF_PAIN, &p, sizeof(int));
        ent->set(EOFF_DIE, &d, sizeof(int));
    }
    Script_AddUndefined();
}
