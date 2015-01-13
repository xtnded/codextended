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

void PlayerCmd_getip(int a1) {
	/*char userinfo[MAX_STRING_CHARS];
	getuserinfo(a1, userinfo, sizeof(userinfo));
	
	char* ip = Info_ValueForKey(userinfo, "ip");
	if(NULL == ip) {
		Scr_AddString("");
	} else {
		char* tok = strtok(ip, ":");
		if(tok)
			Scr_AddString(tok);
		else
			Scr_AddString("");
	}*/
	client_t* cl = getclient(a1);
	if(cl) {
		char ip[16];
		get_client_ip(a1, &ip[0]);
		Scr_AddString(ip);
	}
}

void PlayerCmd_SetPerk(int self) {
	xclient_t *xcl = &xclients[self];
	int perk = Scr_GetConstString(0);
	int i;
	for(i = 0; i < MAX_PERKS; i++) {
		if(xscr_const.perks[i] == perk) {
			xcl->perks[i] = 1;
			break;
		}
	}
	
	if(i == MAX_PERKS)
		printf("ERROR: SetPerk() Perk '%s' was not found!\n", SL_ConvertToString(perk));
}

void PlayerCmd_UnsetPerk(int self) {
	xclient_t *xcl = &xclients[self];
	int perk = Scr_GetConstString(0);
	int i;
	for(i = 0; i < MAX_PERKS; i++) {
		if(xscr_const.perks[i] == perk) {
			xcl->perks[i] = 0;
			break;
		}
	}
	
	if(i == MAX_PERKS)
		printf("ERROR: UnsetPerk() '%s' was not found!\n", SL_ConvertToString(perk));
}

void PlayerCmd_HasPerk(int self) {
	xclient_t *xcl = &xclients[self];
	int perk = Scr_GetConstString(0);
	int i;
	for(i = 0; i < MAX_PERKS; i++) {
		if(xscr_const.perks[i] == perk) {
			if(xcl->perks[i])
				Scr_AddBool(true);
			break;
		}
	}
	
	if(i == MAX_PERKS)
		printf("ERROR: HasPerk() '%s' was not found!\n", SL_ConvertToString(perk));
}

extern challenge_t* challenges;

void PlayerCmd_GetGuid(int a1) {
	client_t* cl = getclient(a1);
	challenge_t* challenge;
	int guid = 0;
	if(cl) {
		challenge = &challenges[0];
		for (int j = 0 ; j < MAX_CHALLENGES ; j++, challenge++ ) {
			if (NET_CompareAdr( cl->remoteAddress, challenge->adr ) ) {
				guid = x_challenges[j].guid;
				break;
			}
		}
	}
	Scr_AddInt(guid);
}

void PlayerCmd_renamebot(int a1) {
	char* key = Scr_GetString(0);
	char userinfo[MAX_STRING_CHARS];
	getuserinfo(a1, userinfo, sizeof(userinfo));
	
	/*char* value = Info_ValueForKey(userinfo, key);
	if(value == NULL)
		Scr_AddString("");
	else
		Scr_AddString(value);*/
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
	Scr_AddBool(xtnded_clients[a1].pure);
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
			Scr_AddBool(true);
		else
			Scr_AddBool(false);
	} else {
		Scr_AddBool(false);
	}
}

void PlayerCmd_DropClient(int a1) {
	char* reason = Scr_GetString(0);
	if(Scr_GetNumParam() > 0)
	SV_DropClient(getclient(a1), reason);
	else
	SV_DropClient(getclient(a1), NULL);
}

void PlayerCmd_SendServerCommand(int a1) {
	char* cmd = Scr_GetString(0);
	SV_SendServerCommand(getclient(a1), 1, cmd);
}

void PlayerCmd_SendGamestate(int a1) {
	client_t *cl = getclient(a1);
	void (*donedl)(client_t*) = (void(*)(client_t*))0x80879FC;
	if(cl)
		donedl(cl);
}

void PlayerCmd_GetUserInfoKey(int a1) {
	char* key = Scr_GetString(0);
	char userinfo[MAX_STRING_CHARS];
	getuserinfo(a1, userinfo, sizeof(userinfo));
	
	char* value = Info_ValueForKey(userinfo, key);
	if(value == NULL)
		Scr_AddString("");
	else
		Scr_AddString(value);
}

void PlayerCmd_GetUserInfo(int a1) {
	char userinfo[MAX_STRING_CHARS];
	getuserinfo(a1, userinfo, sizeof(userinfo));
	
	Scr_AddString(userinfo);
}

void PlayerCmd_SetVelocity(int self) {
	gentity_t *e = &g_entities[self];
	
	if(!e->client) {
		Scr_Error("entity is not a player");
		return;
	}
	
	vec3_t vec;
    Scr_GetVector(0, vec);
	
	VectorCopy(vec, (float*)((int)e->client + POFF_VELOCITY));
}

void PlayerCmd_GetVelocity(int self) {
	gentity_t *e = &g_entities[self];
	
	if(!e->client) {
		Scr_Error("entity is not a player");
		return;
	}
	
	vec3_t vec;
	
	VectorCopy((float*)((int)e->client + POFF_VELOCITY), vec);
	Scr_AddVector(vec);
}

void PlayerCmd_GetInt(int self) {
	gentity_t *ent = &g_entities[self];
	int off = Scr_GetInt(0);
	int flag = Scr_GetInt(1);
	int base = (int)ent, value;
	if(flag)
		base = (int)ent->client;
	value = *(int*)(base + off);
	Scr_AddInt(value);
}

void PlayerCmd_GetByte(int self) {
	gentity_t *ent = &g_entities[self];
	int off = Scr_GetInt(0);
	unsigned char value;
	int flag = Scr_GetInt(1);
	int base = (int)ent;
	if(flag)
		base = (int)ent->client;
	value = *(unsigned char*)(base + off);
	Scr_AddInt(value);
}

void PlayerCmd_GetFloat(int self) {
	gentity_t *ent = &g_entities[self];
	int off = Scr_GetInt(0);
	float value;
	int flag = Scr_GetInt(1);
	int base = (int)ent;
	if(flag)
		base = (int)ent->client;
	value = *(float*)(base + off);
	Scr_AddFloat(value);
}

void PlayerCmd_SetInt(int self) {
	gentity_t *ent = &g_entities[self];
	int off = Scr_GetInt(0);
	int value = Scr_GetInt(1);
	int flag = Scr_GetInt(2);
	int base = (int)ent;
	if(flag)
		base = (int)ent->client;
	*(int*)(base + off) = value;
}

void PlayerCmd_SetByte(int self) {
	gentity_t *ent = &g_entities[self];
	int off = Scr_GetInt(0);
	unsigned char value = Scr_GetInt(1) & 0xff;
	int flag = Scr_GetInt(2);
	int base = (int)ent;
	if(flag)
		base = (int)ent->client;
	*(unsigned char*)(base + off) = value;
}

void PlayerCmd_SetFloat(int self) {
	gentity_t *ent = &g_entities[self];
	int off = Scr_GetInt(0);
	float value = Scr_GetFloat(1);
	int flag = Scr_GetInt(2);
	int base = (int)ent;
	if(flag)
		base = (int)ent->client;
	*(float*)(base + off) = value;
}

void PlayerCmd_forwardButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.forwardmove & 0x7f) == 0x7f) {
            Scr_AddBool(true);
            return;
		}
    }
    Scr_AddBool(false);
}

void PlayerCmd_leanLeftButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if(cl->lastUsercmd.wbuttons & 0x10) {
            Scr_AddBool(true);
            return;
		}
    }
    Scr_AddBool(false);
}

void PlayerCmd_leanRightButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if(cl->lastUsercmd.wbuttons & 0x20) {
            Scr_AddBool(true);
            return;
		}
    }
    Scr_AddBool(false);
}

void PlayerCmd_aimButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if(cl->lastUsercmd.buttons & 0x10) {
            Scr_AddBool(true);
            return;
		}
    }
    Scr_AddBool(false);
}


void PlayerCmd_reloadButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if(cl->lastUsercmd.wbuttons & 0x8) {
            Scr_AddBool(true);
            return;
		}
    }
    Scr_AddBool(false);
}


void PlayerCmd_backButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.forwardmove & 0x81) == 0x81) {
            Scr_AddBool(true);
            return;
		}
    }
    Scr_AddBool(false);
}

void PlayerCmd_leftButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.rightmove & 0x81) == 0x81) {
            Scr_AddBool(true);
            return;
		}
    }
    Scr_AddBool(false);
}

void PlayerCmd_rightButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.rightmove & 0x7f) == 0x7f) {
            Scr_AddBool(true);
            return;
		}
    }
    Scr_AddBool(false);
}

void PlayerCmd_moveupButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.upmove & 0x7f) == 0x7f) {
            Scr_AddBool(true);
            return;
		}
    }
    Scr_AddBool(false);
}

void PlayerCmd_movedownButtonPressed(int a1) {
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.upmove & 0x81) == 0x81) {
            Scr_AddBool(true);
            return;
		}
    }
    Scr_AddBool(false);
}

void PlayerCmd_getPlayerAngles(int self) {
	gentity_t *ent = &g_entities[self];
	
	vec3_t vec;
	
	VectorCopy((float*)((int)ent->client + POFF_ANGLES), vec);
	Scr_AddVector(vec);
}
