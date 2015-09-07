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

#include "js.h"
#include "server.h"

#ifdef BUILD_ECMASCRIPT

duk_context *js_context = NULL;

static inline unsigned js_this_num(duk_context *c) {
	duk_push_this(c);
	duk_get_prop_string(c, -1, "\xff""num");
	unsigned n = duk_to_int(c, -1);
	duk_pop_2(c);
	return n;
}

int js_setbyte(duk_context* c) {
    byte* off = (byte*)duk_require_pointer(c, -1);
    byte val = (byte)duk_require_int(c, -2);
    *off = val;
    return 0;
}

int js_getbyte(duk_context* c) {
    byte* off = (byte*)duk_require_pointer(c, -1);
    duk_push_int(c, *off);
    return 1;
}

int js_setint(duk_context* c) {
    int* ptr = (int*)duk_require_pointer(c, -1);
    int off = duk_require_int(c, -2);
    int value = duk_require_int(c, -3);
    ptr+=off;
	*ptr = value;
    return 0;
}

int js_getint(duk_context* c) {
    int* ptr = (int*)duk_require_pointer(c, -1);
	int off = duk_require_int(c,-2);
	ptr+=off;
    duk_push_int(c,*ptr);
    return 1;
}

int js_getliboffset(duk_context* c) {
    const char* str = duk_require_string(c, -1);
	void* p = (void*)dlsym(gamelib, str);
	duk_push_pointer(c, p);
    return 1;
}

int js_getentint(duk_context* c) {
	int index = duk_require_number(c,-1);
	int offset = duk_require_number(c,-2);
	gentity_t *ent = &g_entities[index];
	if(ent) {
		int value;
		ENT_GET(ent, offset, &value, sizeof(value));
		duk_push_int(c,value);
	} else {
		duk_push_int(c,0);
	}
	return 1;
}

int js_setentint(duk_context* c) {
	int index = duk_require_int(c,-1);
	int offset = duk_require_int(c,-2);
	int value = duk_require_int(c,-3);
	gentity_t *ent = &g_entities[index];
	if(ent) {
		ENT_SET(ent, offset, &value, sizeof(value));
	}
	return 0;
}

int js_getplayer(duk_context* c) {
	int index = duk_require_int(c, -1);
	int off = duk_require_int(c, -2);
	const char* str = duk_require_string(c, -3);
	printf("-1 = %d\n", index);
	printf("-2 = %d\n", off);
	printf("-3 = %s\n", str);
	gentity_t *ent = &g_entities[index];
	if(ent) {
		int v;
		ENT_GET(ent, off, &v, sizeof(v));
		duk_push_int(c,v);
	} else {
		duk_push_int(c,-1);
	}
	/*if(!strcmp(str, "int")) {
		printf("off = %d\n", *(int*)off);
		duk_push_int(c, *(int*)off);
	} else if(!strcmp(str, "float")) {
		duk_push_number(c, *(float*)off);
	} else if(!strcmp(str, "string")) {
		duk_push_string(c, (const char*)off);
	} else if(!strcmp(str, "byte")) {
		duk_push_int(c, *(byte*)off);
	}*/
	return 1;
}

int js_setplayer(duk_context* c) {
	int player = duk_require_int(c, -1);
	int duk_off = duk_require_int(c, -2);
	int off = (g_entities + GENTITY_SIZE * player) + duk_off;
	const char* str = duk_require_string(c, -3);
	if(!strcmp(str, "int")) {
		*(int*)off = duk_require_int(c,-4);
	} else if(!strcmp(str, "float")) {
		*(float*)off = duk_require_number(c,-4);
	} else if(!strcmp(str, "byte")) {
		*(byte*)off=duk_require_int(c,-4);
	}
	return 0;
}

void js_error(const char* str, int fatal) {
	if(str!=NULL)
		printf("SCRIPTING ENGINE > Error: %s", str);
	if(fatal)
		COD_Destructor();
}

int js_Cmd_ArgvBuffer(duk_context *c) {
	if(!duk_has_prop(c,-1)) {
		duk_push_string(c,"");
		return;
	}
	int index = duk_require_int(c, 0);
	if(index > Cmd_Argc() || index < 0) {
		duk_push_string(c, "");
		return 0;
	}
	
    char cmd[MAX_STRING_CHARS];
    Cmd_ArgvBuffer(index, cmd, sizeof(cmd));
	
	duk_push_string(c,cmd);
	return 1;
}

int js_player_getname(duk_context *c) {
	int player = js_this_num(c);
	if(player < 0 || player > 63)
		return 0;
	
	client_t* cl = getclient(player);
	duk_push_string(c,cl->name);
	return 1;
}

int js_player_getscore(duk_context *c) {
	int player = js_this_num(0);
	if(player<0||player>63)
		return 0;
	client_t* cl = getclient(player);
	if(!cl->state)
		return 0;
		
	duk_push_int(c,SV_GetClientScore(cl));
	return 1;
}

int js_getconfigstring(duk_context *c) {
	int i = duk_require_int(c,0);
	char cs[MAX_INFO_STRING];
	
	if(i<0||i>2047)
		return 0;
	SV_GetConfigstring(i, cs, sizeof(cs));
	duk_push_string(c,cs);
	return 1;
}

int js_setconfigstring(duk_context *c) {
	int i = duk_require_int(c,0);
	char* str = duk_require_string(c,1);
	
	if(i<0||i>2047)
		return 0;
	SV_SetConfigstring(i, str);
	return 0;
}

int js_player_getping(duk_context *c) {
	int player = js_this_num(c);
	if(player<0||player>63)
		return 0;
	client_t* cl = getclient(player);
	if(!cl->state)
		return 0;
		
	duk_push_int(c,cl->ping);
	return 1;
}

int js_player_getip(duk_context *c) {
	int player = js_this_num(c);
	if(player<0||player>63)
		return 0;
	client_t* cl = getclient(player);
	if(!cl->state)
		return 0;
	char *ip = NET_AdrToString(cl->netchan.remoteAddress);
	duk_push_string(c,ip);
	return 1;
}

int js_player_kick(duk_context *c) {
	int player = js_this_num(c);
	char* reason = duk_require_string(c,0);
	if(player<0||player>63)
		return 0;
	client_t* cl = getclient(player);
	if(!cl->state)
		return 0;
	
	bool isBot = cl->netchan.remoteAddress.type == NA_BOT;
	SV_DropClient(cl, reason);
	
	if(isBot)
		cl->state = CS_FREE;
	
	return 0;
}
extern unsigned char *hudelems;
int js_newhudelem(duk_context *c) {
	unsigned char* cur = hudelems;
	unsigned int i;
	
	for(i = 0; i < 1024; i++) {
		if(!*(int*)cur) {
			*(int *)(cur + 12) = 1065353216;
			*(int *)(cur + 108) = 0;
			*(int *)(cur + 100) = 0;
			*(int *)cur = 1;
			*(int *)(cur + 4) = 0;
			*(int *)(cur + 8) = 0;
			*(int *)(cur + 16) = 0;
			*(int *)(cur + 20) = 0;
			*(int *)(cur + 24) = 0;
			*(int *)(cur + 28) = -1;
			*(int *)(cur + 32) = 0;
			*(int *)(cur + 36) = 0;
			*(int *)(cur + 40) = 0;
			*(int *)(cur + 44) = 0;
			*(int *)(cur + 120) = 1;
			*(int *)(cur + 48) = 0;
			*(int *)(cur + 52) = 0;
			*(int *)(cur + 56) = 0;
			*(int *)(cur + 60) = 0;
			*(int *)(cur + 64) = 0;
			*(int *)(cur + 68) = 0;
			*(int *)(cur + 72) = 0;
			*(int *)(cur + 92) = 0;
			*(int *)(cur + 96) = 0;
			*(int *)(cur + 104) = 0;
			*(int *)(cur + 112) = 1023;
			*(int *)(cur + 116) = 0;
			break;
		}
		cur+=124;
	}
	
	if(i==1024)
		return 0; //max hud elems
	
	duk_push_int(c,i);
	return 1;
}

int js_SendServerCommand(duk_context *c) {
	char* msg = duk_require_string(c, 0);
	if(!*msg)
		return 0;
	
	SV_SendServerCommand(NULL, 0, msg);
	return 0;
}

int js_PlayerSendServerCommand(duk_context *c) {
	char* msg;
	int playerNumber = duk_require_int(c,0);
	msg = duk_require_string(c,1);
	if(!*msg)
		return 0;
	
	if(playerNumber < 0 || playerNumber > 63)
		return 0;
	
	SV_SendServerCommand( getclient( playerNumber ), 0, msg);
	return 0;
}

extern gitem_t *bg_itemlist;

void (*X_SetOrigin)(gentity_t*, vec3_t);
void (*TRAP_LinkEntity)(gentity_t*);

int js_ent_setorigin(duk_context *c) {
	vec3_t org;
	int ent_idx = duk_require_int(c,0);
	
	float x,y,z;
	duk_get_prop_string(c, 1, "x");
	x = (float)duk_to_number(c,-1);
	duk_pop(c);
	duk_get_prop_string(c, 1, "y");
	y = (float)duk_to_number(c,-1);
	duk_pop(c);
	duk_get_prop_string(c, 1, "z");
	z = (float)duk_to_number(c,-1);
	duk_pop(c);
	
	org[0]=x;
	org[1]=y;
	org[2]=z;
	
	
	
	gentity_t *ent = &g_entities[ent_idx];
	
	if(!ent)
		return 0;
	
	X_SetOrigin(ent, org);
	TRAP_LinkEntity(ent);
	return 0;
}

int js_ent_getorigin(duk_context *c) {
	vec3_t org;

	int ent_idx = duk_require_int(c,0);
	
	gentity_t *ent = &g_entities[ent_idx];
	
	if(!ent)
		return 0;
	
	int ORIGIN_OFF = 308;
	
	ENT_GET(ent, ORIGIN_OFF, org, sizeof(org));
	
	int obj_idx = duk_push_object(c);
	duk_push_number(c, org[0]);
    duk_put_prop_string(c, obj_idx, "x");
	duk_push_number(c, org[1]);
    duk_put_prop_string(c, obj_idx, "y");
	duk_push_number(c, org[2]);
    duk_put_prop_string(c, obj_idx, "z");
	return 1;
}

qboolean X_CallSpawnEntity(gentity_t *ent) {
	void (*spawnitem)(gentity_t*, gitem_t*);
	*(int*)&spawnitem=GAME("G_SpawnItem");
	
	char* classname = ScriptL_ConvertToString( *(short*)( ent + 374 ) );
	
	gitem_t *item;
	gitem_t *start = bg_itemlist;
	start+=12;
	for(item = start; item->classname; item += 12) {
		Com_DPrintf("items = %s\n", item->classname);
		if(!strcmp(item->classname, classname)) {
			spawnitem(ent, item);
			return qtrue;
		}
	}
	return qfalse;
}

/*
signed int __cdecl G_CallSpawnEntity(int a1)
{
  signed int result; // eax@3
  const char **v2; // ebx@5
  int v3; // esi@5
  int v4; // ebx@8
  const char *v5; // [sp-20h] [bp-38h]@12
  int v6; // [sp-1Ch] [bp-34h]@12

  if ( !*(_WORD *)(a1 + 374) )
    goto LABEL_12;
  v3 = SL_ConvertToString(*(_WORD *)(a1 + 374));
  v2 = (const char **)&bg_itemlist[12];
  if ( bg_itemlist[12] )
  {
    while ( strcmp(*v2, (const char *)v3) )
    {
      v2 += 12;
      if ( !*v2 )
        goto LABEL_8;
    }
    G_SpawnItem(a1, (int)v2);
    result = 1;
  }
  else
  {
LABEL_8:
    v4 = (int)spawns;
    if ( !spawns[0] )
    {
LABEL_11:
      v6 = SL_ConvertToString(*(_WORD *)(a1 + 374));
      v5 = "%s doesn't have a spawn function\n";
LABEL_12:
      G_Printf(v5, v6);
      return 0;
    }
    while ( strcmp(*(const char **)v4, (const char *)v3) )
    {
      v4 += 8;
      if ( !*(_DWORD *)v4 )
        goto LABEL_11;
    }
    (*(void (__cdecl **)(int))(v4 + 4))(a1);
    result = 1;
  }
  return result;
}*/

int js_dobjupdate(duk_context *c) {
	int e = duk_require_int(c,0);
	
	void (__cdecl *d)(gentity_t*);
	*(int*)&d = GAME("G_DObjUpdate");
	
	printf("dobjupdate = %x\n", d);
	
	gentity_t *ent = &g_entities[e];
	
	if(!ent)
		return 0;
	d(ent);
}

int js_linkentity(duk_context *c) {
	void (*linkentity)(gentity_t*);
	*(int*)&linkentity = GAME("trap_LinkEntity");
	
	int idx = duk_require_int(c,0);
	
	if(idx < 0 || idx > 0x400)
		return 0;
	
	gentity_t *ent = &g_entities[idx];
	
	if(ent)
		linkentity(ent);
	return 0;
}

int js_ent_setint(duk_context *c) {
	int entity = duk_require_int(c,0);
	int off = duk_require_int(c,1);
	int value = duk_require_int(c,2);
	int type = duk_require_int(c,3);
	
	if(entity<0||entity>=0x400)
		return 0;
		
	gentity_t *ent = &g_entities[entity];
	
	if(!ent)
		return 0;
	
	if(type==8)
		ENT_SET(ent, off, (byte*)&value, sizeof(byte));
	else if(type==16)
		ENT_SET(ent, off, (short*)&value, sizeof(short));
	else if(type==32)
		ENT_SET(ent, off, &value, sizeof(value));
	else if(type==64)
		ENT_SET(ent, off, (long*)&value, sizeof(long));
	return 0;
}

int js_ent_getint(duk_context *c) {
	int entity = duk_require_int(c,0);
	int off = duk_require_int(c,1);
	int type = duk_require_int(c,2);
	
	if(entity<0||entity>=0x400)
		return 0;
		
	gentity_t *ent = &g_entities[entity];
	
	if(!ent)
		return 0;
	
	union {
		int value;
		byte bvalue;
		long lvalue;
		short svalue;
	} u;
	
	ENT_GET(ent, off, &u.lvalue, sizeof(u.lvalue));
	
	if(type==8)
	duk_push_int(c,u.bvalue);
	else if(type==16)
	duk_push_int(c,u.svalue);
	else if(type==32)
	duk_push_int(c,u.value);
	else if(type==64)
	duk_push_int(c,u.lvalue);
	return 1;
}

int js_net_outofbandprint(duk_context *c) {
	int clientNum = duk_require_int(c, 0);
	char *msg = duk_require_string(c, 1);
	
	client_t *cl = getclient(clientNum);
	
	NET_OutOfBandPrint(NS_SERVER,cl->netchan.remoteAddress,msg);
}

duk_ret_t js_setcvar(duk_context *c) {
	char* key = duk_require_string(c,0);
	char* value = duk_require_string(c,1);
	Cvar_Set(key, value);
	return 0;
}

duk_ret_t js_getcvar(duk_context *c) {
	char *key = duk_require_string(c, 0);
	char *value = Cvar_VariableString(key);
	duk_push_string(c, value);
	return 1;
}

int js_spawn(duk_context *c) {
	char* classname = duk_require_string(c,0);
	int spawnflags = 0;
	float x,y,z;
	duk_get_prop_string(c, 1, "x");
	x = (float)duk_to_number(c,-1);
	duk_pop(c);
	duk_get_prop_string(c, 1, "y");
	y = (float)duk_to_number(c,-1);
	duk_pop(c);
	duk_get_prop_string(c, 1, "z");
	z = (float)duk_to_number(c,-1);
	duk_pop(c);
	
	void (__cdecl *_Scr_SetString)(short*,unsigned short);
	*(int*)&_Scr_SetString = GAME("Scr_SetString");
	
	int ent = oSpawn(); //doesn't work with gentity_t* or int*
	//printf("ent = %x\n", ent);
	unsigned short str_index = _SL_GetString(classname, 0);
	_Scr_SetString(ent + 374, str_index);
	*(float *)(ent + 308) = x;
	*(float *)(ent + 312) = y;
	*(float *)(ent + 316) = z;
	*(int *)(ent + 376) = spawnflags;
	
	if(!CallSpawnEntity(ent)) //FUCKING annoying didn't work with gentity_t* or int*
		return 0;
	duk_push_int(c,*(int*)ent);
	return 1;
}

int js_Cmd_Argv(duk_context *c) {
	int index = duk_require_int(c,0);
	duk_push_string(c,Cmd_Argv(index));
	return 1;
}

int js_Cmd_Argc(duk_context *c) {
	int argc = Cmd_Argc();
	duk_push_int(c,argc);
	return 1;
}

duk_ret_t js_playerobj_getplayerangles(duk_context *c) {
	vec3_t org;
	int ent_idx = js_this_num(c);
	
	gentity_t *ent = &g_entities[ent_idx];
	
	if(!ent)
		return 0;
	
	int ORIGIN_OFF = 308;
	
	ENT_GET(ent, ORIGIN_OFF, org, sizeof(org));
	
	int obj_idx = duk_push_object(c);
	duk_push_number(c, org[0]);
    duk_put_prop_string(c, obj_idx, "x");
	duk_push_number(c, org[1]);
    duk_put_prop_string(c, obj_idx, "y");
	duk_push_number(c, org[2]);
    duk_put_prop_string(c, obj_idx, "z");
	return 1;
}

duk_ret_t js_playerobj_sendservercommand(duk_context *c) {
	char *msg;
	msg = duk_require_string(c,0);
	if(!*msg)
		return;
	int self = js_this_num(c);
	
	SV_SendServerCommand( getclient( self ), 0, msg);
	return 0;
}

const duk_function_list_entry player_object_methods[] = {
	{"getPlayerAngles", js_playerobj_getplayerangles, 0},
	{"sendServerCommand", js_playerobj_sendservercommand, 1},
	{"kick", js_player_kick, 1},
    {"getName", js_player_getname, 0},
    {"getScore", js_player_getscore, 0},
    {"getIp", js_player_getip, 0},
    {"getPing", js_player_getping, 0},
	{NULL, NULL, 0}
};

duk_ret_t js_player_get_score(duk_context *c) {
	gentity_t *self = &g_entities[js_this_num(c)];
	
	if(!self->client)
		return 0;
	
	int *score = (int*)((unsigned)self->client + 8416);
	duk_push_int(c, *score);
	return 1;
}

duk_ret_t js_player_get_deaths(duk_context *c) {
	gentity_t *self = &g_entities[js_this_num(c)];
	
	if(!self->client)
		return 0;
	
	int *n = (int*)((unsigned)self->client + 0x20E4);
	duk_push_int(c, *n);
	return 1;
}

duk_ret_t js_player_get_headicon(duk_context *c) {
	gentity_t *self = &g_entities[js_this_num(c)];
	
	if(!self->client)
		return 0;
	
	int *n = (int*)((unsigned)self + 148);
	if(!*n) {
		duk_push_string(c, "");
		return 1;
	}
	char cs[MAX_STRING_CHARS] = {0};
	SV_GetConfigstring(*n + 28, cs, sizeof(cs));
	duk_push_string(c, cs);
	return 1;
}

duk_ret_t js_player_set_headicon(duk_context *c) {
	char *s = duk_require_string(c, 0);
	gentity_t *self = &g_entities[js_this_num(c)];
	
	if(!self->client)
		return 0;
	
	int *n = (int*)((unsigned)self + 148);
	int (*_GScr_GetHeadIconIndex)(char*) = (int(*)(char*))GAME("GScr_GetHeadIconIndex");
	*n = _GScr_GetHeadIconIndex(s);
	return 0;
}

duk_ret_t js_player_get_statusicon(duk_context *c) {
	gentity_t *self = &g_entities[js_this_num(c)];
	
	if(!self->client)
		return 0;
	
	int *statusicon = (int*)((unsigned)self->client + 8408);
	if(!*statusicon) {
		duk_push_string(c, "");
		return 1;
	}
	char cs[MAX_STRING_CHARS] = {0};
	SV_GetConfigstring(20 + *statusicon, cs, sizeof(cs));
	duk_push_string(c, cs);
	return 1;
}

duk_ret_t js_player_set_statusicon(duk_context *c) {
	char *s = duk_require_string(c, 0);
	gentity_t *self = &g_entities[js_this_num(c)];
	
	if(!self->client)
		return 0;
	int *statusicon = (int*)((unsigned)self->client + 8408);
	int (*_GScr_GetStatusIconIndex)(char*) = (int(*)(char*))GAME("GScr_GetStatusIconIndex");
	*statusicon = _GScr_GetStatusIconIndex(s);
	return 0;
}

duk_ret_t js_player_set_deaths(duk_context *c) {
	int n = duk_require_int(c,0);
	gentity_t *self = &g_entities[js_this_num(c)];
	
	if(!self->client)
		return 0;
	
	int *nn = (int*)((unsigned)self->client + 0x20E4);
	
	*nn = n;
	return 0;
}

duk_ret_t js_player_set_score(duk_context *c) {
	int nScore = duk_require_int(c,0);
	gentity_t *self = &g_entities[js_this_num(c)];
	
	if(!self->client)
		return 0;
	
	int *score = (int*)((unsigned)self->client + 8416);
	
	*score = nScore;
	
	void (*_CalculateRanks)() = (void(*)())GAME("CalculateRanks");
	_CalculateRanks();
	return 0;
}

duk_ret_t js_player_set_sessionteam(duk_context *c) {
	char *team = duk_require_string(c, 0);
	gentity_t *self = &g_entities[js_this_num(c)];
	
	if(!self->client)
		return 0;
	
	int *sessionteam = (int*)((unsigned)self->client + 8572);
	
	if(!strcmp(team, "allies"))
		*sessionteam = 2;
	else if(!strcmp(team, "axis"))
		*sessionteam = 1;
	else if(!strcmp(team, "spectator"))
		*sessionteam = 3;
	else if(!strcmp(team, "none"))
		*sessionteam = 0;
	
	return 0;
}

duk_ret_t js_player_set_sessionstate(duk_context *c) {
	char *state = duk_require_string(c, 0);
	gentity_t *self = &g_entities[js_this_num(c)];
	
	if(!self->client)
		return 0;
	
	int *sessionstate = (int*)((unsigned)self->client + 8400);
	if(!strcmp(state, "dead"))
		*sessionstate = 1;
	else if(!strcmp(state, "playing"))
		*sessionstate = 0;
	else if(!strcmp(state, "spectator"))
		*sessionstate = 2;
	else if(!strcmp(state, "intermission"))
		*sessionstate = 3;
	
	return 0;
}
	
duk_ret_t js_player_get_sessionstate(duk_context *c) {
	gentity_t *self = &g_entities[js_this_num(c)];
	
	if(!self->client)
		return 0;
	
	int sessionstate = *(int*)((unsigned)self->client + 8572);
	if(sessionstate == 1)
		duk_push_string(c, "dead");
	else if(sessionstate == 2)
		duk_push_string(c, "spectator");
	else if(sessionstate == 3)
		duk_push_string(c, "intermission");
	else
		duk_push_string(c, "playing");
	return 1;
}

duk_ret_t js_player_get_sessionteam(duk_context *c) {
	gentity_t *self = &g_entities[js_this_num(c)];
	
	if(!self->client)
		return 0;
	
	int sessionteam = *(int*)((unsigned)self->client + 8572);
	if(sessionteam == 1)
		duk_push_string(c, "axis");
	else if(sessionteam == 2)
		duk_push_string(c, "allies");
	else if(sessionteam == 3)
		duk_push_string(c, "spectator");
	else
		duk_push_string(c, "none");
	return 1;
}

duk_ret_t js_playerobj_readonly_set(duk_context *c) {
	printf("Cannot set read-only field!\n");
	return 0;
}

void js_remove_player_object(client_t *cl) {
	
	int clientNum = get_client_number(cl);
	
	duk_push_global_object(js_context);
	duk_get_prop_string(js_context, -1, "players");
	
	if(duk_has_prop_index(js_context, -1, clientNum)) {
		duk_del_prop_index(js_context, -1, clientNum);
	}
	duk_pop_2(js_context);
}

void js_push_player_object(client_t *cl) {
	
	int clientNum = get_client_number(cl);
	
	duk_push_global_object(js_context);
	
	duk_get_prop_string(js_context, -1, "players");
	
	//duk_push_object(js_context);
	duk_eval_string(js_context, "new Player");
	duk_put_prop_index(js_context, -2, clientNum);
	duk_get_prop_index(js_context, -1, clientNum);
	
	duk_push_int(js_context, clientNum);
	duk_put_prop_string(js_context, -2, "\xff""num");
	
	duk_put_function_list(js_context, -1, player_object_methods);
	
	duk_push_int(js_context, clientNum);
	duk_put_prop_string(js_context, -2, "number");
	
	#define ADD_PLAYER_FIELD(key, get, set) \
	duk_push_string(js_context, key); \
	duk_push_c_function(js_context, get, 0); \
	duk_push_c_function(js_context, set, 1); \
	duk_def_prop(js_context, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER)
	
	ADD_PLAYER_FIELD("name", js_player_getname, js_playerobj_readonly_set);
	ADD_PLAYER_FIELD("sessionteam", js_player_get_sessionteam, js_player_set_sessionteam);
	ADD_PLAYER_FIELD("sessionstate", js_player_get_sessionstate, js_player_set_sessionstate);
	ADD_PLAYER_FIELD("score", js_player_get_score, js_player_set_score);
	ADD_PLAYER_FIELD("deaths", js_player_get_deaths, js_player_set_deaths);
	ADD_PLAYER_FIELD("statusicon", js_player_get_statusicon, js_player_set_statusicon);
	ADD_PLAYER_FIELD("headicon", js_player_get_headicon, js_player_set_headicon);
	
	duk_pop(js_context); //players
	duk_pop(js_context); //global
}

void update_player_js_data(duk_context *c) {
	duk_push_global_object(c);
	client_t *cl;
	
	duk_get_prop_string(c, -1, "players");
	for(int i = 0; i < duk_get_length(c, -1); i++) {
		duk_get_prop_index(c, -1, i);
			duk_get_prop_string(c, -1, "name");
			char *str = duk_get_string(c, -1);
			//printf("oldname = %s\n", str);
			duk_pop(c);
			
			duk_push_string(c, "hello");
			duk_put_prop_string(c, -2, "name");
			
		duk_pop(c);
	}
	duk_pop(c); //players
	duk_pop(c); //global
}


void js_destroy() {
	if(js_context!=NULL) {
		duk_destroy_heap(js_context);
		js_context = NULL;
	}
}

int js_com_printf(duk_context *c) {
    const char* str = duk_require_string(c, -1);
    Com_Printf(str);
    return 0;
}

duk_ret_t js_makeCvarServerInfo(duk_context *c) {
	char *var = duk_require_string(c,-1);
	char *value = duk_require_string(c,-2);
	
	return 0;
}

duk_ret_t js_precacheShader(duk_context *c) {
	char *str = duk_require_string(c, -1);
	
	if(!*str)
		return 0;
	unsigned (*_G_ShaderIndex)(char*) = (unsigned(*)(char*))GAME("G_ShaderIndex");
	_G_ShaderIndex(str);
	return 0;
}

duk_ret_t js_precacheMenu(duk_context *c) {
	char *str = duk_require_string(c, -1);
	
	if(!*str)
		return 0;
	
	char cs[MAX_STRING_CHARS] = {0};
	int i;
	
	for(i = 0; i < 32; i++) {
		SV_GetConfigstring(i + 1180, cs, sizeof(cs));
		
		if(!*cs)
			break;
		
		if(!Q_stricmp(cs, str)) {
			printf("Script tried to precache menu '%s' twice.\n", str);
			return 0;
		}
	}
	
	for(i = 0; i < 32; i++) {
		SV_GetConfigstring(i + 1180, cs, sizeof(cs));
		
		if(!*cs) {
			SV_SetConfigstring(i + 1180, str);
			return 0;
		}
	}
	
	if(i == 32) {
		printf("Too many menus precached. Max allowed menus is 32\n");
	}
	
	return 0;
}

duk_ret_t js_precacheString(duk_context *c) {
	char *str = duk_require_string(c, -1);
	
	if(!*str)
		return 0;
	unsigned (*_G_LocalizedStringIndex)(char*) = (unsigned(*)(char*))GAME("G_LocalizedStringIndex");
	_G_LocalizedStringIndex(str);
	return 0;
}

static long BG_StringHashValue( const char *fname );

duk_ret_t js_GSC_LoadCallback(duk_context *c) {
	unsigned short handle = 0;
	
	char *szFileName = duk_require_string(c, -1);
	char *szFunctionName = duk_require_string(c, -2);
	
	unsigned short load_callback(const char* file, const char* functionname, bool flag); /* flag is required for game to load?*/
	
	if(!(handle = load_callback(szFileName, szFunctionName, 1))) {
		printf("GSC_LoadCallback (JS) failed load_callback(%s, %s)\n", szFileName, szFunctionName);
	}
	//printf("handle = %u\n", handle);
		
	duk_push_int(c, handle);
	return 1;	
}

duk_ret_t js_callGSC(duk_context *c) {
	unsigned short handle = (unsigned short)duk_require_int(c, 0);
	//printf("handle = %u\n", handle);
	long numArgs = (long)duk_get_top(c) - 1;
	
	for(int i = numArgs; i > 0; i--) { /* reverse for GSC stack pushing */
		int type = duk_get_type(c, i);
		//printf("type = %d, i = %d\n", type, i);
		switch(type) {
			default:
			case DUK_TYPE_UNDEFINED:
			case DUK_TYPE_NULL:
				Scr_AddUndefined();
			break;
			
			case DUK_TYPE_BOOLEAN:
				Scr_AddBool(duk_require_bool(c, i));
			break;
			
			case DUK_TYPE_NUMBER:
				Scr_AddFloat((float)duk_require_number(c, i));
			break;
			
			case DUK_TYPE_STRING:
				Scr_AddString(duk_require_string(c, i));
			break;
			
		}
	}
	
	if(!handle) {
		printf("invalid GSC handle!\n");
		return 0;
	}
	//printf("calling gsc with argc = %d\n", numArgs);
	unsigned short ret = Scr_ExecThread(handle, numArgs);
	Scr_FreeThread(ret);
	
	return 0;
}

void js_addfunction(const char* name, int (*func)(duk_context*), int argc) {
    duk_push_global_object(js_context);
    duk_push_c_function(js_context, func, argc);
    duk_put_prop_string(js_context, -2, name);
    duk_pop(js_context);
}

void js_debug() {
    printf("debug;\n");
}

#define JS_FILE "cod.js"

void js_execute() {
    if(js_context) {
        /*duk_push_global_object(js_context);
        duk_push_c_function(js_context, duk_com_printf, 1);
        duk_put_prop_string(js_context, -2, "Com_Printf");
        duk_pop(js_context);
        */

        duk_push_global_object(js_context);
        duk_get_prop_string(js_context, -1, "ConsoleCommand");
        int argc = Cmd_Argc();
        int arr_idx = duk_push_array(js_context);
        for(int i = 0; i < argc; i++) {
            duk_push_string(js_context, Cmd_Argv(i));
            duk_put_prop_index(js_context, arr_idx, i);
        }
        if(duk_pcall(js_context, 1) != 0) {
			printf("Script Error (ConsoleCommand): %s\n", duk_to_string(js_context, -1));
		}
        duk_pop(js_context);
    }
}

void js_fatal_handler(duk_context *c, int code) {
	DUK_UNREF(c);
	printf("SCRIPTING ENGINE > Fatal error; code %s\n", code);
	COD_Destructor();
}

int js_gsc_func_args = 0;
int js_gsc_func_parms = 0;
duk_context *js_gsc_duk_context = NULL;

duk_ret_t js_gsc_call_builtin_func(duk_context *c) {
	duk_push_global_object(c);
	duk_get_prop_string(c, -1, "GSC_FuncPtr");
	int fIdx = duk_require_int(c, -1);
	duk_pop(c);
	
	SCRIPTFUNCTION *it = (SCRIPTFUNCTION*)GAME("functions");
	js_gsc_duk_context = c;
	js_gsc_func_parms = (int)duk_get_top(c);
	js_gsc_func_args = (int)duk_get_top(c);
	
	//printf("fIdx = %d\n", fIdx);
	void (*func)() = (void(*)())it[fIdx].call;
	//cprintf(PRINT_GOOD, "calling %s\n", it[fIdx].name);
	func();
	//printf("args = %d\n", js_gsc_func_args);
	js_gsc_duk_context = NULL;
	return (js_gsc_func_args > 0);
}

char *gsc_function_whitelist[] = {
	"exitlevel", "gettime", "getcvarfloat",
	"precachestring", "precacheshader", "precachemenu", "precachestatusicon", "precacheheadicon", "makecvarserverinfo",
	
	"randomint", "randomfloat", "randomintrange", "randomfloatrange",
	
	"println", "iprintln", "iprintlnbold", "print3d",
	
	NULL
};

void js_add_gsc_functions() {
	
	if(js_context == NULL)
		return;
	
	duk_push_global_object(js_context);
	
	bool f;
	
	SCRIPTFUNCTION *it = (SCRIPTFUNCTION*)GAME("functions");
	for(int i = 0; i != 0x69; i++, it++) {
		f = false;
		for(int j = 0; gsc_function_whitelist[j]; j++) {
			if(!strcmp(gsc_function_whitelist[j], it->name)) {
				f = true;
				break;
			}
		}
		if(!f)
			continue;
		//if(!strcmp(it->name, "gettime") || !strcmp(it->name, "getcvarfloat")) {
			char *funcStr = va("var %s = function() { GSC_FuncPtr = %hu; return GSC_CallBuiltin.apply(this, arguments); }", it->name, i);
			//printf("funcStr = %s\n", funcStr);
			duk_eval_string(js_context, funcStr);
			#if 0
			duk_push_c_function(js_context, js_gsc_call_builtin_func, DUK_VARARGS);
			duk_put_prop_string(js_context, -2, it->name);
			#endif
			cprintf(PRINT_GOOD|PRINT_UNDERLINE,"Added GSC function %s:%d to JS\n", it->name, i);
		//}
	}
	duk_pop(js_context);		
}

void js_load() {
	if(js_context != NULL)
		return;
	
    js_context = duk_create_heap(NULL, NULL, NULL, NULL, js_fatal_handler);
	if(!js_context) {
		Com_Printf("Could not create heap for scripting engine.\n");
		COD_Destructor();
		return;
	}
	
	if(0 != duk_peval_string_noresult(js_context, "var GSC_FuncPtr = 0;var Player = function() {};function setTimeout(a,b){var c,d,e,f=EventLoop;if(\"number\"!=typeof b)throw new TypeError(\"delay is not a number\");if(b=Math.max(f.minimumDelay,b),\"string\"==typeof a)c=eval.bind(this,a);else{if(\"function\"!=typeof a)throw new TypeError(\"callback is not a function/string\");arguments.length>2?(d=Array.prototype.slice.call(arguments,2),d.unshift(this),c=a.bind.apply(a,d)):c=a}return e=f.nextTimerId++,f.insertTimer({id:e,oneshot:!0,cb:c,delay:b,target:Date.now()+b}),e}function clearTimeout(a){var b=EventLoop;if(\"number\"!=typeof a)throw new TypeError(\"timer ID is not a number\");b.removeTimerById(a)}function setInterval(a,b){var c,d,e,f=EventLoop;if(\"number\"!=typeof b)throw new TypeError(\"delay is not a number\");if(b=Math.max(f.minimumDelay,b),\"string\"==typeof a)c=eval.bind(this,a);else{if(\"function\"!=typeof a)throw new TypeError(\"callback is not a function/string\");arguments.length>2?(d=Array.prototype.slice.call(arguments,2),d.unshift(this),c=a.bind.apply(a,d)):c=a}return e=f.nextTimerId++,f.insertTimer({id:e,oneshot:!1,cb:c,delay:b,target:Date.now()+b}),e}function clearInterval(a){var b=EventLoop;if(\"number\"!=typeof a)throw new TypeError(\"timer ID is not a number\");b.removeTimerById(a)}EventLoop={timers:[],expiring:null,nextTimerId:1,minimumDelay:1,minimumWait:1,maximumWait:6e4,maxExpirys:10},EventLoop.getEarliestTimer=function(){var a=this.timers;return n=a.length,n>0?a[n-1]:null},EventLoop.getEarliestWait=function(){var a=this.getEarliestTimer();return a?a.target-Date.now():null},EventLoop.insertTimer=function(a){var c,d,e,b=this.timers;for(d=b.length,c=d-1;c>=0&&(e=b[c],!(a.target<=e.target));c--);b.splice(c+1,0,a)},EventLoop.removeTimerById=function(a){var c,d,e,b=this.timers;if(e=this.expiring,e&&e.id===a)return void(e.removed=!0);for(d=b.length,c=0;d>c;c++)if(e=b[c],e.id===a)return e.removed=!0,void this.timers.splice(c,1)},EventLoop.processTimers=function(){for(var d,e,a=Date.now(),b=this.timers,c=this.maxExpirys;c-->0&&(d=b.length,!(0>=d))&&(e=b[d-1],!(a<=e.target));){b.pop(),e.oneshot?e.removed=!0:e.target=a+e.delay,this.expiring=e;try{e.cb()}catch(f){print(\"timer callback failed, ignored: \"+f)}this.expiring=null,e.removed||this.insertTimer(e)}},EventLoop_RunFrame=function(){var a;EventLoop.processTimers(),a=EventLoop.getEarliestWait(),null!==a&&(a=Math.min(EventLoop.maximumWait,Math.max(EventLoop.minimumWait,a)))};"))
		printf("Script Error: %s\n", duk_safe_to_string(js_context, -1));
	
	duk_idx_t arr_idx = duk_push_array(js_context);
	duk_put_global_string(js_context, "players");
	
	js_addfunction("GSC_Call", js_callGSC, DUK_VARARGS);
	js_addfunction("GSC_CallBuiltin", js_gsc_call_builtin_func, DUK_VARARGS);
	js_addfunction("GSC_LoadCallback", js_GSC_LoadCallback, 2);
	js_addfunction("precacheShader", js_precacheShader, 1);
	js_addfunction("precacheString", js_precacheString, 1);
	js_addfunction("precacheMenu", js_precacheMenu, 1);
	js_addfunction("makeCvarServerInfo", js_makeCvarServerInfo, 2);
	js_addfunction("Com_Printf", js_com_printf, 1);
    js_addfunction("sendServerCommand", js_SendServerCommand, 1);
	
    js_addfunction("getCvar", js_getcvar, 1);
    js_addfunction("getDvar", js_getcvar, 1);
    js_addfunction("setCvar", js_setcvar, 2);
    js_addfunction("setDvar", js_setcvar, 2);
	
    js_addfunction("newHudElem", js_newhudelem, 0);
    js_addfunction("spawn", js_spawn, 2);
    js_addfunction("ent_getorigin", js_ent_getorigin, 1);
    js_addfunction("ent_setorigin", js_ent_setorigin, 2);
    js_addfunction("ent_setint", js_ent_setint, 4);
    js_addfunction("ent_getint", js_ent_getint, 3);
    js_addfunction("linkentity", js_linkentity, 1);
    js_addfunction("dobjupdate", js_dobjupdate, 1);
	
    js_addfunction("Cmd_ArgvBuffer", js_Cmd_ArgvBuffer, 1);
    js_addfunction("getConfigString", js_getconfigstring, 1);
    js_addfunction("setConfigString", js_setconfigstring, 2);
    js_addfunction("Cmd_Argv", js_Cmd_Argv, 1);
    js_addfunction("Cmd_Argc", js_Cmd_Argc, 0);
    js_addfunction("print", js_com_printf, 1);
	js_addfunction("setbyte", js_setbyte, 2);
	js_addfunction("getbyte", js_getbyte, 1);
	js_addfunction("setint", js_setint, 3);
	js_addfunction("getint", js_getint, 2);
	js_addfunction("get_lib_offset", js_getliboffset, 1);
	js_addfunction("setentint", js_setentint, 2);
	js_addfunction("getentint", js_getentint, 3);
	js_addfunction("NET_OutOfBandPrint", js_net_outofbandprint, 2);
	
	client_t *cl;
	if(sv_running->integer) {
		for(int i = 0; i < sv_maxclients->integer; i++) {
			cl = getclient(i);
			if(!cl->state)
				continue;
			js_push_player_object(cl);
		}
		js_add_gsc_functions();
	}
	
	if(duk_pcompile_file(js_context, 0, JS_FILE) != 0)
		printf("Script Compile Failed: %s\n", duk_safe_to_string(js_context, -1));
	else
		duk_call(js_context, 0);
	/*
	if(duk_peval_file(js_context, JS_FILE) != 0)
		printf("Script Error: %s\n", duk_safe_to_string(js_context, -1));
	duk_pop(js_context);
	*/
}

void js_reload() {
	js_destroy();
	js_load();
	void (*SV_MapRestart_f)() = (void(*)())0x8083DE4;
	SV_MapRestart_f();
}

void js_init() {
	js_load();
	
    Cmd_AddCommand("js", js_execute);
    Cmd_AddCommand("js_reload", js_reload);
    Cmd_AddCommand("js_debug", js_debug);
}

#endif
