#include "../script.hpp"
#include "../../server/server.hpp"

void PlayerCmd_getip(int a1) {
	/*char userinfo[MAX_STRING_CHARS];
	getuserinfo(a1, userinfo, sizeof(userinfo));
	
	char* ip = Info_ValueForKey(userinfo, "ip");
	if(NULL == ip) {
		Script_AddString("");
	} else {
		char* tok = strtok(ip, ":");
		if(tok)
			Script_AddString(tok);
		else
			Script_AddString("");
	}*/
	client_t* cl = getclient(a1);
	if(cl) {
		char ip[16];
		get_client_ip(a1, &ip[0]);
		Script_AddString(ip);
	}
}

void PlayerCmd_renamebot(int a1) {
	char* key = Script_GetString(0);
	char userinfo[MAX_STRING_CHARS];
	getuserinfo(a1, userinfo, sizeof(userinfo));
	
	/*char* value = Info_ValueForKey(userinfo, key);
	if(value == NULL)
		Script_AddString("");
	else
		Script_AddString(value);*/
	Info_SetValueForKey(userinfo, "name", key);
	setuserinfo(a1, userinfo);
	
	client_t* cl = getclient(a1);
	if(cl) {
		memcpy(&cl->name, key, 32);
		cl->name[31] = '\0';
	}
}

void PlayerCmd_ispure(int a1) {
	if(a1 < 0 || a1 >= 64)
		return;
	Script_AddBool(xtnded_clients[a1].pure);
}

void PlayerCmd_kickbot(int a1) { //weird playercmd > bot
	client_t* cl = getclient(a1);
	if(cl) {
		SV_DropClient(cl, "");
		cl->state = CS_FREE;
	}
}

void PlayerCmd_isbot(int a1) {
	client_t* cl = getclient(a1);
	if(cl) {
		if(cl->remoteAddress.type == NA_BOT)
			Script_AddBool(true);
		else
			Script_AddBool(false);
	} else {
		Script_AddBool(false);
	}
}

void PlayerCmd_DropClient(int a1) {
	char* reason = Script_GetString(0);
	if(Script_GetNumParam() > 0)
	SV_DropClient(getclient(a1), reason);
	else
	SV_DropClient(getclient(a1), NULL);
}

void PlayerCmd_SendServerCommand(int a1) {
	char* cmd = Script_GetString(0);
	SV_SendServerCommand(getclient(a1), 1, cmd);
}

void PlayerCmd_GetUserInfoKey(int a1) {
	char* key = Script_GetString(0);
	char userinfo[MAX_STRING_CHARS];
	getuserinfo(a1, userinfo, sizeof(userinfo));
	
	char* value = Info_ValueForKey(userinfo, key);
	if(value == NULL)
		Script_AddString("");
	else
		Script_AddString(value);
}

void PlayerCmd_setVelocity(int entityIndex) {
    vec3_t vec;
    Script_GetVector(0, vec);
    ENTITY* ent = game->getEntity(entityIndex);
    if(ent) {
        ent->toPlayerState();
        ent->set(POFF_VELOCITY, vec, sizeof(vec));
        ent->toEntityState();
    }
    Script_AddUndefined();
}

void PlayerCmd_getVelocity(int entityIndex) {
    vec3_t vec;
    ENTITY* ent = game->getEntity(entityIndex);
    if(ent) {
        ent->toPlayerState();
        ent->get(POFF_VELOCITY, vec, sizeof(vec));
        ent->toEntityState();
        Script_AddVector(vec);
    }
}

void PlayerCmd_getInt(int a1) {
    ENTITY* ent = game->getEntity(a1);
	if(ent) {
		int off = Script_GetInt(0);
		int flag = Script_GetInt(1);
		if(flag)
		ent->toPlayerState();
		int v;
        ent->get(off, &v, sizeof(v));
		Script_AddInt(v);
		if(flag)
		ent->toEntityState();
    }
}

void PlayerCmd_setInt(int a1) {
    ENTITY* ent = game->getEntity(a1);
	if(ent) {
		int off = Script_GetInt(0);
		int v = Script_GetInt(1);
		int flag = Script_GetInt(2);
		if(flag)
		ent->toPlayerState();
        ent->set(off, &v, sizeof(v));
		if(flag)
		ent->toEntityState();
    }
}

void PlayerCmd_getbyte(int a1) {
    ENTITY* ent = game->getEntity(a1);
	if(ent) {
		int off = Script_GetInt(0);
		int flag = Script_GetInt(1);
		if(flag)
		ent->toPlayerState();
		byte v;
        ent->get(off, &v, sizeof(v));
		Script_AddInt(v);
		if(flag)
		ent->toEntityState();
    }
}

void PlayerCmd_setbyte(int a1) {
    ENTITY* ent = game->getEntity(a1);
	if(ent) {
		int off = Script_GetInt(0);
		byte v = (byte)Script_GetInt(1);
		int flag = Script_GetInt(2);
		if(flag)
		ent->toPlayerState();
        ent->set(off, &v, sizeof(v));
		if(flag)
		ent->toEntityState();
    }
}

void PlayerCmd_getfloat(int a1) {
    ENTITY* ent = game->getEntity(a1);
	if(ent) {
		int off = Script_GetInt(0);
		int flag = Script_GetInt(1);
		if(flag)
		ent->toPlayerState();
		float v;
        ent->get(off, &v, sizeof(v));
		Script_AddFloat(v);
		if(flag)
		ent->toEntityState();
    }
}

void PlayerCmd_setfloat(int a1) {
    ENTITY* ent = game->getEntity(a1);
	if(ent) {
		int off = Script_GetInt(0);
		float v = Script_GetFloat(1);
		int flag = Script_GetInt(2);
		if(flag)
		ent->toPlayerState();
        ent->set(off, &v, sizeof(v));
		if(flag)
		ent->toEntityState();
    }
}

/*
    why not just use one function for them
    //richard
*/

void PlayerCmd_forwardButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.forwardmove & 0x7f) == 0x7f) {
            Script_AddBool(true);
            return;
		}
    }
    Script_AddBool(false);
}

void PlayerCmd_leanLeftButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if(cl->lastUsercmd.wbuttons & 0x10) {
            Script_AddBool(true);
            return;
		}
    }
    Script_AddBool(false);
}

void PlayerCmd_leanRightButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if(cl->lastUsercmd.wbuttons & 0x20) {
            Script_AddBool(true);
            return;
		}
    }
    Script_AddBool(false);
}

void PlayerCmd_aimButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if(cl->lastUsercmd.buttons & 0x10) {
            Script_AddBool(true);
            return;
		}
    }
    Script_AddBool(false);
}


void PlayerCmd_reloadButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if(cl->lastUsercmd.wbuttons & 0x8) {
            Script_AddBool(true);
            return;
		}
    }
    Script_AddBool(false);
}


void PlayerCmd_backButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.forwardmove & 0x81) == 0x81) {
            Script_AddBool(true);
            return;
		}
    }
    Script_AddBool(false);
}

void PlayerCmd_leftButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.rightmove & 0x81) == 0x81) {
            Script_AddBool(true);
            return;
		}
    }
    Script_AddBool(false);
}

void PlayerCmd_rightButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.rightmove & 0x7f) == 0x7f) {
            Script_AddBool(true);
            return;
		}
    }
    Script_AddBool(false);
}

void PlayerCmd_moveupButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.upmove & 0x7f) == 0x7f) {
            Script_AddBool(true);
            return;
		}
    }
    Script_AddBool(false);
}

void PlayerCmd_movedownButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.upmove & 0x81) == 0x81) {
            Script_AddBool(true);
            return;
		}
    }
    Script_AddBool(false);
}

void PlayerCmd_getPlayerAngles(int entityIndex) {
    ENTITY* ent = game->getEntity(entityIndex);
    if(ent) {
        vec3_t angles;
        ent->toPlayerState();
        ent->get(POFF_ANGLES, angles, sizeof(angles));
        ent->toEntityState();
        Script_AddVector(angles);
    }
}
