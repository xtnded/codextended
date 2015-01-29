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

duk_context *ctx;

void mf_name(int num, JSMemberfield* field) {
    char userinfo[MAX_STRING_CHARS];
    getuserinfo(num, userinfo, sizeof(userinfo));

    char* name = Info_ValueForKey(userinfo, "name");
    duk_push_string(ctx, name);
}

void mf_num(int num, JSMemberfield* field) {
    duk_push_int(ctx, num);
}

JSMemberfield playerfields[] = {
    {"name", FT_STRING, mf_name},
    {"num", FT_INT, mf_num},
    {"team", FT_STRING, mf_name},
    {"state", FT_STRING, mf_name},
    {"maxhealth", FT_INT, mf_name},
    {"maxspeed", FT_FLOAT, mf_name},
    {"score", FT_INT, mf_name},
    {"deaths", FT_INT, mf_name},
    {NULL, NULL}
};

void js_destroy() {
	if(ctx!=NULL)
		duk_destroy_heap(ctx);
}

int js_com_printf(duk_context *ctx) {
    const char* str = duk_require_string(ctx, -1);
    Com_Printf(str);
    return 0;
}

void js_addfunction(const char* name, int (*func)(duk_context*), int argc) {
    duk_push_global_object(ctx);
    duk_push_c_function(ctx, func, argc);
    duk_put_prop_string(ctx, -2, name);
    duk_pop(ctx);
}

void js_debug() {
    printf("debug;\n");
}

void js_execute() {
    if(ctx) {
        /*duk_push_global_object(ctx);
        duk_push_c_function(ctx, duk_com_printf, 1);
        duk_put_prop_string(ctx, -2, "Com_Printf");
        duk_pop(ctx);
        */

        duk_push_global_object(ctx);
        duk_get_prop_string(ctx, -1, "consolecommand");
        int argc = Cmd_Argc()-1;
        int arr_idx = duk_push_array(ctx);
        for(int i = 0; i < argc; i++) {
            duk_push_string(ctx, Cmd_Argv(i));
            duk_put_prop_index(ctx, arr_idx, i);
        }
        duk_call(ctx, 1);
        duk_pop(ctx);
    }
}

void js_load() {
	FILE *fp = fopen("cod.js","rb");
	
	if(!fp)
		return;
	fclose(fp);
    duk_eval_file(ctx, "cod.js");
    duk_pop(ctx);
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
    const char* str = duk_require_string(ctx, -1);
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
		duk_push_int(ctx, *(int*)off);
	} else if(!strcmp(str, "float")) {
		duk_push_number(ctx, *(float*)off);
	} else if(!strcmp(str, "string")) {
		duk_push_string(ctx, (const char*)off);
	} else if(!strcmp(str, "byte")) {
		duk_push_int(ctx, *(byte*)off);
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
		duk_push_string(ctx,"");
		return;
	}
	int index = duk_require_int(c, 0);
	if(index > Cmd_Argc() || index < 0) {
		duk_push_string(ctx, "");
		return 0;
	}
	
    char cmd[MAX_STRING_CHARS];
    Cmd_ArgvBuffer(index, cmd, sizeof(cmd));
	
	duk_push_string(ctx,cmd);
	return 1;
}

int js_player_getname(duk_context *c) {
	int player = duk_require_int(c,0);
	if(player < 0 || player > 63)
		return 0;
	
	client_t* cl = getclient(player);
	if(!cl->state)
		return 0;
	duk_push_string(c,cl->name);
	return 1;
}

int js_player_getscore(duk_context *c) {
	int player = duk_require_int(c,0);
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
	int player = duk_require_int(c,0);
	if(player<0||player>63)
		return 0;
	client_t* cl = getclient(player);
	if(!cl->state)
		return 0;
		
	duk_push_int(c,cl->ping);
	return 1;
}

int js_player_getip(duk_context *c) {
	int player = duk_require_int(c,0);
	if(player<0||player>63)
		return 0;
	client_t* cl = getclient(player);
	if(!cl->state)
		return 0;
	char *ip = NET_AdrToString(cl->remoteAddress);
	duk_push_string(c,ip);
	return 1;
}

int js_player_kick(duk_context *c) {
	int player = duk_require_int(c,0);
	char* reason = duk_require_string(c,1);
	if(player<0||player>63)
		return 0;
	client_t* cl = getclient(player);
	if(!cl->state)
		return 0;
	SV_DropClient(cl, reason);
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

extern gitem_t *_bg_itemlist;

void (*X_SetOrigin)(gentity_t*, vec3_t);
void (*TRAP_LinkEntity)(gentity_t*);

int js_ent_setorigin(duk_context *c) {
	vec3_t org;
	int ent_idx = duk_require_int(ctx,0);
	
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

	int ent_idx = duk_require_int(ctx,0);
	
	gentity_t *ent = &g_entities[ent_idx];
	
	if(!ent)
		return 0;
	
	int ORIGIN_OFF = 308;
	
	ENT_GET(ent, ORIGIN_OFF, org, sizeof(org));
	
	int obj_idx = duk_push_object(ctx);
	duk_push_number(ctx, org[0]);
    duk_put_prop_string(ctx, obj_idx, "x");
	duk_push_number(ctx, org[1]);
    duk_put_prop_string(ctx, obj_idx, "y");
	duk_push_number(ctx, org[2]);
    duk_put_prop_string(ctx, obj_idx, "z");
	return 1;
}

qboolean X_CallSpawnEntity(gentity_t *ent) {
	void (*spawnitem)(gentity_t*, gitem_t*);
	*(int*)&spawnitem=GAME("G_SpawnItem");
	
	char* classname = ScriptL_ConvertToString( *(short*)( ent + 374 ) );
	
	gitem_t *item;
	gitem_t *start = _bg_itemlist;
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
	
	NET_OutOfBandPrint(NS_SERVER,cl->remoteAddress,msg);
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
	duk_push_string(ctx,Cmd_Argv(index));
	return 1;
}

int js_Cmd_Argc(duk_context *c) {
	int argc = Cmd_Argc();
	duk_push_int(ctx,argc);
	return 1;
}

void js_fatal_handler(duk_context *c, int code) {
	DUK_UNREF(c);
	printf("SCRIPTING ENGINE > Fatal error; code %s\n", code);
	COD_Destructor();
}

void js_init() {
    ctx = duk_create_heap(NULL, NULL, NULL, NULL, js_fatal_handler);
	if(!ctx) {
		Com_Printf("Could not create heap for scripting engine.\n");
		COD_Destructor();
		return;
	}
    Cmd_AddCommand("js", js_execute);
    Cmd_AddCommand("js_reload", js_load);
    Cmd_AddCommand("js_debug", js_debug);
    js_addfunction("Com_Printf", js_com_printf, 1);
    js_addfunction("sendcommand", js_SendServerCommand, 1);
	
    js_addfunction("player_getname", js_player_getname, 1);
    js_addfunction("player_getscore", js_player_getscore, 1);
    js_addfunction("player_getip", js_player_getip, 1);
    js_addfunction("player_getping", js_player_getping, 1);
    js_addfunction("player_kick", js_player_kick, 2);
    js_addfunction("playersendcommand", js_PlayerSendServerCommand, 2);
	
    js_addfunction("newhudelem", js_newhudelem, 0);
    js_addfunction("spawn", js_spawn, 2);
    js_addfunction("ent_getorigin", js_ent_getorigin, 1);
    js_addfunction("ent_setorigin", js_ent_setorigin, 2);
    js_addfunction("ent_setint", js_ent_setint, 4);
    js_addfunction("ent_getint", js_ent_getint, 3);
    js_addfunction("linkentity", js_linkentity, 1);
    js_addfunction("dobjupdate", js_dobjupdate, 1);
	
    js_addfunction("Cmd_ArgvBuffer", js_Cmd_ArgvBuffer, 1);
    js_addfunction("getconfigstring", js_getconfigstring, 1);
    js_addfunction("setconfigstring", js_setconfigstring, 2);
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
    js_load();
}

#endif
