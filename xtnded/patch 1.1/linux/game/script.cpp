#include "script.hpp"
/*
	:: Scr_GetFunction/GetMethod ::
	
	CoD2 search for "parameter count exceeds 256" and go upwards
	CoD1 search for "parameter count exceeds 256" or "unknown (builtin) function '%s'" and go upwards
*/

void pass_unk(int);
void GScr_bodyQueIndex(int a1);
void GScr_GetEntity(int a1);
void GScr_SendServerCommand(int a1);
void GScr_killserver(int);
void GScr_typeof(int);

void GScr_xtnded_anim(int a1) {
	/*int index = 0;
	int b = *(int*)0x82F5948;
	int v1 = *(int *)(b - 8 * index);*/
	
	//Script_GetAnim((int)&v1, 0, 0);
	const char* str_anim = Script_GetString(0);
	int anim_index = oBG_AnimationIndexForString(str_anim);
	//printf("anim = %d\n", anim_index);
	if(anim_index==-1)
		Script_AddInt(0);
	else
		Script_AddInt(anim_index);
}

SCRIPTFUNCTION scriptFunctions[] = {
	//name, function, developer
	
	//MYSQL
    {"mysql_init", GScr_mysql_init, 0},
    {"mysql_close", GScr_mysql_close, 0},
    {"mysql_affected_rows", GScr_mysql_affected_rows, 0},
    {"mysql_errno", GScr_mysql_errno, 0},
    {"mysql_error", GScr_mysql_error, 0},
    {"mysql_fetch_field", GScr_mysql_fetch_field, 0},
    {"mysql_fetch_row", GScr_mysql_fetch_row, 0},
    {"mysql_field_seek", GScr_mysql_field_seek, 0},
    {"mysql_free_result", GScr_mysql_free_result, 0},
    {"mysql_num_fields", GScr_mysql_num_fields, 0},
    {"mysql_num_rows", GScr_mysql_num_rows, 0},
    {"mysql_query", GScr_mysql_query, 0},
    {"mysql_real_connect", GScr_mysql_real_connect, 0},
    {"mysql_real_escape_string", GScr_mysql_real_escape_string, 0},
    {"mysql_store_result", GScr_mysql_store_result, 0},
    {"mysql_get_connection", GScr_mysql_get_connection, 0},
	//MYSQL END
	
    {"printconsole", GScr_printconsole, 0},
    {"get_animation_index", GScr_xtnded_anim, 0},
    {"killserver", GScr_killserver, 0},
    {"err_fatal", GScr_killserver, 0},
    {"typeof", GScr_typeof, 0},
    {"sendservercommand", GScr_SendServerCommand, 0},
    {"trace", GScr_Trace, 0},
    {"md5", GScr_md5, 0},
    {"creturn", GScr_return, 0},
    {"cmd_argc", GScr_Cmd_Argc, 0},
    {"cmd_argv", GScr_Cmd_Argv, 0},
    {"cmd_argvbuffer", GScr_trap_Argv, 0},
    {"concatargs", GScr_ConcatArgs, 0},
    {"getchat", GScr_getChat, 0},
    {"getanimindex", GScr_GetAnimIndex, 0},
    {"spawnaniment", GScr_SpawnAnimEnt, 0},
    {"bodyqueindex", GScr_bodyQueIndex, 0},
    {"g_spawn", GScr_callspawn, 0},
    {"getentity", GScr_GetEntity, 0},
    {"pass_unk", pass_unk, 0},
    {"fopen", GScr_fopen, 0},
    {"fread", GScr_fread, 0},
    {"fclose", GScr_fclose, 0},
    {"fwrite", GScr_fwrite, 0},
    {"fexists", GScr_fexists, 0},
    {"fsize", GScr_fsize, 0},
    {"strpos", GScr_strpos, 0},
    {"salt_password", GScr_salt_password, 0},
    {"and", GScr_and, 0},
    {"xor", GScr_xor, 0},
    {"or", GScr_or, 0},
    {"not", GScr_not, 0},
    {"rshift", GScr_rshift, 0},
    {"lshift", GScr_lshift, 0},
    {"sin", MScr_sin, 0},
    {"cos", MScr_cos, 0},
    {"sqrt", MScr_sqrt, 0}
};

SCRIPTFUNCTION scriptMethods[] = {
	//name, function, developer
	
	/*
	======
	ENTITY
	======
	*/
	
	{"setbounds", EntCmd_setBounds, 0},
	{"setboundcorners", EntCmd_setBoundCorners, 0},
	{"setsize", EntCmd_setBoundCorners, 0},
	{"settakedamage", EntCmd_setTakeDamage, 0},
	{"callback", EntCmd_callback, 0},
	{"nextthink", EntCmd_nextthink, 0},
	
	/*
	======
	PLAYER
	======
	*/
    {"usebuttonpressedx", PlayerCmd_useButtonPressedX, 0},
	{"setvelocity", PlayerCmd_setVelocity, 0},
	{"getvelocity", PlayerCmd_getVelocity, 0},
	{"getplayerangles", PlayerCmd_getPlayerAngles, 0},
	{"getip", PlayerCmd_getip, 0},
	{"ispure", PlayerCmd_ispure, 0},
	{"sendservercommand", PlayerCmd_SendServerCommand, 0},
	{"get_ip", PlayerCmd_getip, 0},
	{"getint", PlayerCmd_getInt, 0},
	{"setint", PlayerCmd_setInt, 0},
	{"getbyte", PlayerCmd_getbyte, 0},
	{"getfloat", PlayerCmd_getfloat, 0},
	{"setfloat", PlayerCmd_setfloat, 0},
	{"setbyte", PlayerCmd_setbyte, 0},
	{"getuserinfokey", PlayerCmd_GetUserInfoKey, 0},
	{"dropclient", PlayerCmd_DropClient, 0},
	{"kickbot", PlayerCmd_kickbot, 0},
	{"renamebot", PlayerCmd_renamebot, 0},
	{"isbot", PlayerCmd_isbot, 0},
	{"backbuttonpressed", PlayerCmd_backButtonPressed, 0},
	{"forwardbuttonpressed", PlayerCmd_forwardButtonPressed, 0},
	{"leftbuttonpressed", PlayerCmd_leftButtonPressed, 0},
	{"rightbuttonpressed", PlayerCmd_rightButtonPressed, 0},
	{"moveupbuttonpressed", PlayerCmd_moveupButtonPressed, 0},
	{"movedownbuttonpressed", PlayerCmd_movedownButtonPressed, 0},
	{"aimbuttonpressed", PlayerCmd_aimButtonPressed, 0},
	{"reloadbuttonpressed", PlayerCmd_reloadButtonPressed, 0},
	{"leanleftbuttonpressed", PlayerCmd_leanLeftButtonPressed, 0},
	{"leanrightbuttonpressed", PlayerCmd_leanRightButtonPressed, 0}
};

char* g_script_data;
char* script_const;

int callbackTest;
int callbackPlayerCommand;
int callbackEntityDamage;
int callbackEntityThink;
int callbackEntityUse;
int callbackEntityKilled;
int callbackEntityTouch;
int callbackfunctionmain;

bool scr_return = 0;

Script_LoadScript_t Script_LoadScript;
Script_GetFunctionHandle_t Script_GetFunctionHandle;
Script_ExecThread_t Script_ExecThread;
Script_ExecEntThread_t Script_ExecEntThread;
Script_FreeThread_t Script_FreeThread;

ScriptL_ConvertToString_t ScriptL_ConvertToString;

/*
	Can't use Scr_GetMethod/Scr_GetFunction because those names already exists and aren't mangled.
*/

Script_GetFunction_t Script_GetFunction;
Script_GetMethod_t Script_GetMethod;

Script_MakeArray_t Script_MakeArray;
Script_AddArray_t Script_AddArray;
Script_Error_t Script_Error;

Script_GetNumParam_t Script_GetNumParam;
Script_GetPointerType_t Script_GetPointerType;
Script_GetType_t Script_GetType;
Script_AddFloat_t Script_AddFloat;
Script_AddInt_t Script_AddInt;
Script_AddBool_t Script_AddBool;
Script_AddString_t Script_AddString;
Script_AddVector_t Script_AddVector;
Script_AddUndefined_t Script_AddUndefined;
Script_AddEntity_t Script_AddEntity;

Script_SetString_t Script_SetString;
Script_GetConstString_t Script_GetConstString;
oSpawn_t oSpawn;
oInitGentity_t oInitGentity;
oFreeEntity_t oFreeEntity;

oBG_AnimationIndexForString_t oBG_AnimationIndexForString;

Script_GetBool_t Script_GetBool;
Script_GetInt_t Script_GetInt;
Script_GetAnim_t Script_GetAnim = (Script_GetAnim_t)0x80A7FAC;
Script_GetAnimsIndex_t Script_GetAnimsIndex;
Script_GetFloat_t Script_GetFloat;
Script_GetVector_t Script_GetVector;
Script_GetString_t Script_GetString;
Script_GetFunc_t Script_GetFunc;

SCRIPTFUNCTIONCALL Script_GetCustomFunction(const char** fname, int* fdev) {
    SCRIPTFUNCTIONCALL m = Script_GetFunction(fname, fdev);
    void (*fc)(int);
    *(int*)&fc = (int)m;
    if(!m) {
        for(unsigned int i = 0; i < (sizeof(scriptFunctions)/sizeof(SCRIPTFUNCTION)); i++) {
            if(!strcmp(*fname, scriptFunctions[i].name)) {
                SCRIPTFUNCTION func = scriptFunctions[i];
                *fname = func.name;
                *fdev = func.developer;
                return func.call;
            }
        }
    }
	return fc;
}

SCRIPTFUNCTIONCALL Script_GetCustomMethod(const char** fname, int* fdev) {
    SCRIPTFUNCTIONCALL m = Script_GetMethod(fname, fdev);
    void (*fc)(int);
    *(int*)&fc = (int)m;
    if(!m) {
        for(unsigned int i = 0; i < (sizeof(scriptMethods)/sizeof(SCRIPTFUNCTION)); i++) {
            if(!strcmp(*fname, scriptMethods[i].name)) {
                SCRIPTFUNCTION func = scriptMethods[i];
                *fname = func.name;
                *fdev = func.developer;
                return func.call;
            }
        }
    }
	return fc;
}

int load_callback(const char* file, const char* functionname) {
    if(!Script_LoadScript(file))
        game_error("Could not find script '%s'\n", file);

    int v4 = Script_GetFunctionHandle(file, functionname);
    if(!v4)
        game_error("Could not find label '%s' in script '%s'\n", functionname, file);
    printf("callback[%s] = %x\n", functionname, v4);
    return v4;
}

void GScr_killserver(int a1) {
	char* reason = Script_GetString(0);
	void (*call)();
	*((int*)(&call)) = 0x8084D3C;
	call();
	printf("%s\n", reason);
}

void GScr_typeof(int a1) {
	int type = Script_GetType(0);
	switch(type) {
		default:
		case VT_UNDEFINED: {
			Script_AddString("undefined");
		}
		break;
		case VT_INT: {
			Script_AddString("integer");
		} 
		break;
		case VT_FLOAT: {
			Script_AddString("float");
		} 
		break;
		case VT_STRING: {
			Script_AddString("string");
		} 
		break;
		case VT_VECTOR: {
			Script_AddString("vector");
		} 
		break;
		case VT_LOCALIZED_STRING: {
			Script_AddString("localized-string");
		} 
		break;
		case VT_OBJECT: {
			Script_AddString("object");
		} 
		break;
		case VT_ARRAY: {
			Script_AddString("array");
		} 
		break;
	}
}

void pass_unk(int a1) {
	int type = Script_GetType(0);
	int type2 = Script_GetPointerType(0);
	printf("passed an %d and %d\n", type, type2);
}

/*
=============
FUNCTIONS
=============
*/

void GScr_and(int a1) {
	byte a = Script_GetInt(0);
	byte b = Script_GetInt(1);
	Script_AddInt(a&b);
}

void GScr_or(int a1) {
	byte a = Script_GetInt(0);
	byte b = Script_GetInt(1);
	Script_AddInt(a|b);
}

void GScr_xor(int a1) {
	byte a = Script_GetInt(0);
	byte b = Script_GetInt(1);
	Script_AddInt(a^b);
}

void GScr_lshift(int a1) {
	byte a = Script_GetInt(0);
	byte b = Script_GetInt(1);
	Script_AddInt(a<<b);
}

void GScr_rshift(int a1) {
	byte a = Script_GetInt(0);
	byte b = Script_GetInt(1);
	Script_AddInt(a>>b);
}

void GScr_not(int a1) {
	byte a = Script_GetInt(0);
	Script_AddInt(~a);
}

void GScr_printconsole(int entityIndex) { //if this was a method the index would be the entity's number
	const char* txt = Script_GetString(0);
	printf(txt);
}

void GScr_salt_password(int a1) {
	char* password = Script_GetString(0);
	char* salt = Script_GetString(1);
	Script_AddString(get_pass_hash(password, salt));
}

void GScr_strpos(int a1) {
	char* haystack = Script_GetString(0);
	char* needle = Script_GetString(1);
	char* p = strstr(haystack, needle);
	if(p)
		Script_AddInt(((int)p-(int)haystack));
	else
		Script_AddInt(-1);
}

void GScr_GetAnimIndex(int a1) {
	char* animstr = Script_GetString(0);
	printf("animstr = %s\n", animstr);
	int index = oBG_AnimationIndexForString("pb_climbdown");
	printf("anim index = %d\n", index);
	Script_AddInt(index);
}

void GScr_SendServerCommand(int a1) {
	char* cmd = Script_GetString(0);
	SV_SendServerCommand(NULL, 1, cmd);
}

#define MAX_BODYQUE 32

int bodyqueindex = 0;

void GScr_bodyQueIndex(int a1) {
	bodyqueindex++;
	if(bodyqueindex >= 32)
	bodyqueindex=0;
}

void GScr_Trace(int a1) {
    trace_t trace;
    vec3_t start, endl, mins, maxs;
    int ignore = Script_GetInt(4);
    int contentmask = Script_GetInt(5);
    Script_GetVector(0, start);
    Script_GetVector(1, endl);
    Script_GetVector(2, mins);
    Script_GetVector(3, maxs);
    //printf("trap_Trace called; start: (%f %f %f) end: (%f %f %f)\n", start[0], start[1], start[2], endl[0], endl[1], endl[2]);
    SV_Trace(&trace, start, mins, maxs, endl, ignore, contentmask);
    //printf("finished trap_trace\n");
    //printf("::output %f %f %f\n", trace.endpos[0], trace.endpos[1], trace.endpos[2]);
    Script_MakeArray();
    /*
        0 = endpos
        1 = surfaceflags
        2 = entityNum
        3 = fraction
        4 = startsolid
        5 = allsolid
        6 = normal
        7 = contents
    */
    Script_AddVector(trace.endpos); Script_AddArray();
    Script_AddInt(trace.surfaceFlags); Script_AddArray();
    Script_AddInt(trace.entityNum); Script_AddArray();
    Script_AddFloat(trace.fraction); Script_AddArray();
    Script_AddBool(trace.startsolid); Script_AddArray();
    Script_AddBool(trace.allsolid); Script_AddArray();
    Script_AddVector(trace.plane.normal); Script_AddArray();
    Script_AddInt(trace.contents); Script_AddArray();
}


void GScr_Cmd_Argc(int a1) {
    Script_AddInt(Cmd_Argc());
}

void GScr_Cmd_Argv(int a1) {
    unsigned int idx = Script_GetInt(0);
    if(idx > Cmd_Argc()) {
        Script_AddString("");
        return;
    }
    Script_AddString(Cmd_Argv(idx));
}

void GScr_trap_Argv(int a1) {
    unsigned int idx = Script_GetInt(0);
    if(idx > Cmd_Argc()) {
        Script_AddString("");
        return;
    }
    char buf[MAX_STRING_CHARS];
    Cmd_ArgvBuffer(idx, buf, sizeof(buf));
    Script_AddString(buf);
}

void GScr_ConcatArgs(int a1) {
    unsigned int idx = Script_GetInt(0);
    if(idx > Cmd_Argc()) {
        Script_AddString("");
        return;
    }
    Script_AddString(ConcatArgs(idx));
}

void GScr_md5(int a1) {
	char* str = Script_GetString(0);
	Script_AddString(get_md5(str));
}

void GScr_getChat(int a1) {
    unsigned int idx = Script_GetInt(0);
    if(idx > Cmd_Argc()) {
        Script_AddString("");
        return;
    }
	char* chat = ConcatArgs(idx);
	if(strlen(chat) == 0 || (chat[0] == 0x15 && strlen(chat)==1)) {
		Script_AddString("");
		return;
	}
	if(chat[0] == 0x15)
		Script_AddString(&chat[1]);
	else
		Script_AddString(chat);
}

void GScr_return(int a1) {
    scr_return = 1;
}

bool Scr_Continue() {
    bool ret = scr_return;
    scr_return = 0;
    return !ret;
}

/*
=============
MATH
=============
*/

void MScr_cos(int entityIndex) {
	float f = Script_GetFloat(0);
	Script_AddFloat(cos(f));
}

void MScr_sin(int entityIndex) {
	float f = Script_GetFloat(0);
	Script_AddFloat(sin(f));
}

void MScr_sqrt(int entityIndex) {
	float f = Script_GetFloat(0);
	Script_AddFloat(sqrt(f));
}

/*
=============
PLAYER METHODS
=============
*/

void PlayerCmd_useButtonPressedX(int entityIndex) {
	if(entityIndex > 1023) {
		Script_Error(va("%i is not a valid entity number", entityIndex));
		return;
	}
	
	ENTITY* ent = game->getEntity(entityIndex);
	if(ent) {
		if(!ent->isPlayer()) {
			Script_Error(va("entity %i is not a player", entityIndex));
			return;
		}
		ent->toPlayerState();
		int press;
		ent->get(8688, &press, sizeof(press));
		if(press & 0x40)
			Script_AddInt(1);
		else
			Script_AddInt(0);
		ent->toEntityState();
	}
	
}

/*
=============
FILE FUNCTIONS
=============
*/

void GScr_fopen(int entityIndex) {
    char* name = Script_GetString(0);
    char* mode = Script_GetString(1);
    FILE* f = fopen(name, mode);
    if(f)
        Script_AddInt((int)(f));
    else
        Script_AddInt(-1);
}

void GScr_fexists(int entityIndex) {
    char* name = Script_GetString(0);
    if(FILE *f = fopen(name, "r")) {
        fclose(f);
        Script_AddBool(true);
    } else {
        Script_AddBool(false);
    }
}

void GScr_fread(int entityIndex) {
    int len = Script_GetInt(0);
    FILE *f = (FILE*)(Script_GetInt(1));
    if(f == NULL) {
        Script_AddInt(-1);
    } else {
        char txt[len+1];
        size_t txt_c = 0;
        int c;
        while((c = fgetc(f)) != EOF)
            txt[txt_c++] = c;
        Script_AddString(txt);
    }
}

void GScr_fwrite(int entityIndex) {
    char* text = Script_GetString(0);
    FILE *f = (FILE*)(Script_GetInt(1));
    if(f == NULL) {
        Script_AddBool(false);
    } else {
        Script_AddBool(true);
        fprintf(f, text);
    }
}

void GScr_fsize(int entityIndex) {
    char* name = Script_GetString(0);
    FILE *f = fopen(name, "r");
    size_t len;
    if(f) {
        fseek(f, 0, SEEK_END);
        len = ftell(f);
        rewind(f);
        Script_AddInt((int)len);
    } else {
        Script_AddInt(-1);
    }
}

void GScr_fclose(int entityIndex) {
    FILE *f = (FILE*)(Script_GetInt(0));
    if(f) {
        Script_AddBool(true);
        fclose(f);
    } else {
        Script_AddBool(false);
    }
}

void GScr_SpawnAnimEnt(int a1) {
  vec3_t org;
  Script_GetVector(0, org);
  int reference = Script_GetInt(1);
  int spawnflags = 0;
  if(Script_GetNumParam() > 2)
	spawnflags = Script_GetInt(2);
  gentity_t* ent = oSpawn();
  int* v1 = (int*)(*(int*)(ent+374));
  //Script_SetString(v1, *(int*)&script_const[44]); //entity
  
  *(float *)(ent + 308) = org[0];
  *(float *)(ent + 312) = org[1];
  *(float *)(ent + 316) = org[2];
  *(int*)(ent + 376) = spawnflags;
  *(int*)(ent + 332) = reference;
  *(int*)(ent + 384) = 0;
  *(int*)(ent + 388) = 0;
  *(byte*)(ent + 352) = 1;
  Script_AddEntity(ent);
}

void GScr_callspawn(int a1) {
	int index = Script_GetConstString(0);
	gentity_t* ent = oSpawn();
	
	int* v1 = (int*)(*(int*)(ent+374));
	Script_SetString(v1, index);
	Script_AddEntity(ent);
}

void GScr_GetEntity(int a1) {
	int index = Script_GetInt(0);
	gentity_t* ent = (gentity_t*)(gentities + GENTITY_SIZE * index);
	oInitGentity(ent);
	Script_AddEntity(ent);
}

gentity_t* mySpawnPlayerClone() {
  gentity_t* ent = oSpawn();
  *(byte*)(ent+352) = 1;
  //int* v1 = (int*)(*(int*)(ent+374));
  //Script_SetString(v1, *(int*)&script_const[86]);
  *(int*)(ent+332)=1023;
  *(int*)(ent+384)=0;
  *(int*)(ent+388)=0;
  *(int*)(ent+8)=0;
  return ent;
}

void gscr_loadgametypescript() { //bleh
	char v1[64];
	char* g_gametype = Cvar_VariableString("g_gametype");
	snprintf(v1, 64, "maps/mp/gametypes/%s", g_gametype);
	printf("g_scr_data is located at %x\n", (int)&g_script_data);
	printf("//////////////////////////////////////////////////////////\nLoading gametype %s\n", v1);
	*(int*)&g_script_data[8] = load_callback(v1, "main");
	*(int*)&g_script_data[12] = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_StartGameType");
	*(int*)&g_script_data[16] = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerConnect");
	*(int*)&g_script_data[20] = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerDisconnect");
	*(int*)&g_script_data[24] = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerDamage");
	*(int*)&g_script_data[28] = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerKilled");
	
	/*callbackfunctionmain = load_callback("xtnded", "main");

    int result = Script_ExecThread(callbackfunctionmain, 0);
    Script_FreeThread(result);
	*/
	
    callbackPlayerCommand = load_callback("callback", "CodeCallback_PlayerCommand");
    callbackEntityDamage = load_callback("callback", "CodeCallback_EntityDamage");
    callbackEntityKilled = load_callback("callback", "CodeCallback_EntityKilled");
    callbackEntityUse = load_callback("callback", "CodeCallback_EntityUse");
    callbackEntityThink = load_callback("callback", "CodeCallback_EntityThink");
    callbackEntityTouch = load_callback("callback", "CodeCallback_EntityTouch");
    /*callbackTest = load_callback("callback", "test");
    Com_Printf("callbackTest: %d\n", callbackTest);
    Cmd_AddCommand("loadscript", loadScript);
    Cmd_AddCommand("callscript", callScript);*/

    for(size_t i = 0; i < MAX_ENTITY_SIZE; i++) {
        ENTITY *ent = game->getEntity(i);
        if(ent) {
            ent->nextthink = 0;
            ent->pain = 0;
            ent->die = 0;
            ent->think = 0;
            ent->use = 0;
            ent->touch = 0;
        }
    }
}


void scriptInitializing() {
	cracking_hook_function((int)GAME("GScr_LoadGameTypeScript"), (int)gscr_loadgametypescript);
	g_script_data = (char*)dlsym(gamelib, "g_scr_data");
	script_const = (char*)GAME("scr_const");
	
	Script_GetFunction = (Script_GetFunction_t)GAME("Scr_GetFunction");
	Script_GetMethod = (Script_GetMethod_t)GAME("Scr_GetMethod");
	Script_GetString = (Script_GetString_t)dlsym(gamelib, "Scr_GetString");
	Script_GetConstString = (Script_GetConstString_t)GAME("Scr_GetConstString");
	Script_SetString = (Script_SetString_t)GAME("Scr_SetString");
	oSpawn = (oSpawn_t)GAME("G_Spawn");
	oInitGentity = (oInitGentity_t)GAME("G_InitGentity");
	oFreeEntity = (oFreeEntity_t)GAME("G_FreeEntity");
	level = (level_locals_t)GAME("level");
	oBG_AnimationIndexForString = (oBG_AnimationIndexForString_t)GAME("BG_AnimationIndexForString");
	
	/*printf("gamelib: %.2x\n", (int)gamelib);
	printf("base: %.2x\n", (int)base);
	printf("Scr_GetString in memory: %.2x\n", (int)Script_GetString);
	printf("Scr_GetString without base: %.2x\n", ((int)Script_GetString - (int)gamelib));*/
	Script_GetInt = (Script_GetInt_t)dlsym(gamelib, "Scr_GetInt");
	//Script_GetAnim = (Script_GetAnim_t)dlsym(gamelib, "Scr_GetAnim");
	Script_GetAnimsIndex = (Script_GetAnimsIndex_t)dlsym(gamelib, "Scr_GetAnimsIndex");
	Script_GetFloat = (Script_GetFloat_t)dlsym(gamelib, "Scr_GetFloat");
	Script_GetVector = (Script_GetVector_t)dlsym(gamelib, "Scr_GetVector");
	Script_GetNumParam = (Script_GetNumParam_t)dlsym(gamelib, "Scr_GetNumParam");
	Script_GetBool = (Script_GetBool_t)dlsym(gamelib, "Scr_GetBool");
	Script_GetFunc = (Script_GetFunc_t)dlsym(gamelib, "Scr_GetFunc");
	Script_GetType = (Script_GetType_t)dlsym(gamelib, "Scr_GetType");
	Script_GetPointerType = (Script_GetPointerType_t)dlsym(gamelib, "Scr_GetPointerType");
	
	Script_AddInt = (Script_AddInt_t)dlsym(gamelib, "Scr_AddInt");
	Script_AddFloat = (Script_AddFloat_t)dlsym(gamelib, "Scr_AddFloat");
	Script_AddVector = (Script_AddVector_t)dlsym(gamelib, "Scr_AddVector");
	Script_AddString = (Script_AddString_t)dlsym(gamelib, "Scr_AddString");
	Script_AddEntity = (Script_AddEntity_t)dlsym(gamelib, "Scr_AddEntity");
	Script_AddUndefined = (Script_AddUndefined_t)dlsym(gamelib, "Scr_AddUndefined");
	Script_AddBool = (Script_AddBool_t)dlsym(gamelib, "Scr_AddBool");
	
	Script_MakeArray = (Script_MakeArray_t)dlsym(gamelib, "Scr_MakeArray");
	Script_AddArray = (Script_AddArray_t)dlsym(gamelib, "Scr_AddArray");
	Script_Error = (Script_Error_t)dlsym(gamelib, "Scr_Error");
	
	Script_ExecThread = (Script_ExecThread_t)GAME("Scr_ExecThread");
	Script_ExecEntThread = (Script_ExecEntThread_t)GAME("Scr_ExecEntThreadNum");
	Script_FreeThread = (Script_FreeThread_t)GAME("Scr_FreeThread");
	ScriptL_ConvertToString = (ScriptL_ConvertToString_t)GAME("SL_ConvertToString");
	Script_GetFunctionHandle = (Script_GetFunctionHandle_t)GAME("Scr_GetFunctionHandle");
	Script_LoadScript = (Script_LoadScript_t)GAME("Scr_LoadScript");
}
