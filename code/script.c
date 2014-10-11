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
/*
	:: Scr_GetFunction/GetMethod ::
	
	CoD2 search for "parameter count exceeds 256" and go upwards
	CoD1 search for "parameter count exceeds 256" or "unknown (builtin) function '%s'" and go upwards
*/

typedef struct {
	char *var;
	int structure_offset; //Some offset in the state for getting generic fields.
	int structure_offset_type; //script type e.g "string", "int"
	void (*set)(int);
	void (*get)(int);
	/*
		Under here some additional stuff the CoD structure looks only like the one above.
	*/
	//scr_memberfield_type type; //scrap this now they work on both ents and players.
} scr_memberfield;

/*
	Works just like builtin functions only difference is the assignment or getting part.
	e.g instead of 
	<obj> setorigin( org );
	<obj>.origin = org;
	IF both cases were linked to the same function they'd do the same thing.
	this applies for
	org = <obj> getorigin();
	org = <obj>.origin;
	
	- Richard
*/

void scr_field_awesome(int a1) {
	int argc = Script_GetNumParam();
	
	if(argc == 0) {
		Script_AddString("Laughing With");
		//works - Richard
	} else {
		char *s = Script_GetString(0);
		printf("char *s = %s\n", s);
		/*
			throw error()
			crash here yet to figure out
		*/
	}
}

static scr_memberfield scr_entity_memberfields[] = {
	//easy to add your own new fields which do not exist yet on entities
	//fieldname, unused, unused, set func ptr, get func ptr
	{"php", 0, 0, scr_field_awesome, scr_field_awesome},
	{"guid", 0, 0, scr_field_awesome, scr_field_awesome},
	{"number", 0, 0, scr_field_awesome, scr_field_awesome},
	{"index", 0, 0, scr_field_awesome, scr_field_awesome},
	{"add_your_own_field", 0, 0, scr_field_awesome, scr_field_awesome},
	{NULL,NULL,NULL,NULL}
};

/*
static scr_memberfield scr_client_memberfields[] = {
	{"name", 0, 0, 0, 0},
	{"sessionteam", 0x0, 0x3, 0x316b0, 0x315ec},
	{"sessionstate", 0x0, 0x3, 0x31580, 0x314dc},
	{"maxhealth", 0x2150, 0x0, 0x31470, 0xd6a41ee8},
	{"handicap", 0x214c, 0x0, 0x316d8, 0xd6a41ee8},
	{"score", 0x20e0, 0x0, 0x313fc, 0xd6a41ee8},
	{"deaths", 0x20e4, 0x0, 0xd6a41ee8, 0xd6a41ee8},
	{"statusicon", 0x0, 0x3, 0x31398, 0x3136c},
	{"headicon", 0x0, 0x3, 0x31314, 0x312d4},
	{"headiconteam", 0x0, 0x3, 0x31264, 0x311ac},
	{"spectatorclient", 0x20d4, 0x0, 0x313d4, 0xd6a41ee8},
	{"archivetime", 0x20dc, 0x1, 0x31130, 0x310ec},
	{"pers", 0x20e8, 0x7, 0x316d8, 0xd6a41ee8},
	
	{"php", 0, 1, scr_field_awesome, scr_field_awesome},
	{NULL,NULL,NULL,NULL}
};
*/

void GScr_SendServerCommand(int a1);
void GScr_killserver(int);
void GScr_typeof(int);

void Scr_fuckPrecacheString(int a1);

void GScr_getSite(int a1) {
	#if 0
	char* url = Script_GetString(0);
	
	std::string result = get_site_result(url);
	Script_AddString((char*)result.c_str());
	#endif
}

long long current_timestamp();

void GScr_seconds(int a1) {
	int s = (int)(current_timestamp()/1000);
	Script_AddInt(s);
}

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
	#ifdef uMYSQL
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
	#endif
	
	/*
		STRING
	*/
	{"strtok", Scr_StrTok, 0},
	{"issubstr", Scr_IsSubStr, 0},
	{"tolower", Scr_ToLower, 0},
	{"toupper", Scr_ToUpper, 0},
	{"ucfirst", Scr_ucfirst, 0},
	{"trim", Scr_trim, 0},
	{"convert_string", Scr_convertToIString, 0},
	{"xprecachestring", Scr_fuckPrecacheString, 0},
	
    {"printconsole", GScr_printconsole, 0},
    {"getsite", GScr_getSite, 0},
    {"seconds", GScr_seconds, 0},
    {"get_animation_index", GScr_xtnded_anim, 0},
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
    {"fopen", GScr_fopen, 0},
    {"fread", GScr_fread, 0},
    {"fclose", GScr_fclose, 0},
    {"fwrite", GScr_fwrite, 0},
    {"fexists", GScr_fexists, 0},
    {"fsize", GScr_fsize, 0},
    {"strpos", GScr_strpos, 0},
    {"salt_password", GScr_salt_password, 0},
	{NULL, NULL, 0}
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
    {"getguid", PlayerCmd_GetGuid, 0},
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
	{"leanrightbuttonpressed", PlayerCmd_leanRightButtonPressed, 0},
	{NULL, NULL, 0}
};

char* g_script_data;
char* script_const;
unsigned char* g_clients;
unsigned char* hudelems;

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

#ifdef _WIN32
Scr_ExecThread_t Scr_ExecThread = (Scr_ExecThread_t)0x481E10;
Scr_ExecEntThread_t Scr_ExecEntThread = (Scr_ExecEntThread_t)0x481EC0;
Scr_FreeThread_t Scr_FreeThread = (Scr_FreeThread_t)0x482070;
ScriptL_ConvertToString_t ScriptL_ConvertToString = (ScriptL_ConvertToString_t)0x474EE0;
#else
Script_ExecThread_t Script_ExecThread;
Script_ExecEntThread_t Script_ExecEntThread;
Script_FreeThread_t Script_FreeThread;
ScriptL_ConvertToString_t ScriptL_ConvertToString;
#endif

_SL_GetString_t _SL_GetString;

/*
	Can't use Scr_GetMethod/Scr_GetFunction because those names already exists and aren't mangled.
*/

Script_GetFunction_t Script_GetFunction;
Script_GetMethod_t Script_GetMethod;

#ifdef _WIN32
Scr_MakeArray_t Scr_MakeArray = (Scr_MakeArray_t)0x483330;
Scr_AddArray_t Scr_AddArray = (Scr_AddArray_t)0x483380;
Scr_Error_t Scr_Error = (Scr_Error_t)0x483470;
#else
Script_MakeArray_t Script_MakeArray;
Script_AddArray_t Script_AddArray;
Script_Error_t Script_Error;
#endif

#ifdef _WIN32
Scr_GetNumParam_t Scr_GetNumParam = (Scr_GetNumParam_t)0x483060;
Scr_GetPointerType_t Scr_GetPointerType = (Scr_GetPointerType_t)0x482FC0;
Scr_GetType_t Scr_GetType = (Scr_GetType_t)0x482F50;
Scr_AddInt_t Scr_AddInt = (Scr_AddInt_t)0x483090;
Scr_AddBool_t Scr_AddBool = (Scr_AddBool_t)0x483070;
Scr_AddString_t Scr_AddString = (Scr_AddString_t)0x483210;
Scr_AddVector_t Scr_AddVector = (Scr_AddVector_t)0x4832E0;
Scr_AddUndefined_t Scr_AddUndefined = (Scr_AddUndefined_t)0x4830F0;
Scr_AddEntity_t Scr_AddEntity = (Scr_AddEntity_t)0x483140;
#else
Script_GetNumParam_t Script_GetNumParam;
Script_GetPointerType_t Script_GetPointerType;
Script_GetType_t Script_GetType;
Script_AddFloat_t Script_AddFloat;
Script_AddInt_t Script_AddInt;
Script_AddBool_t Script_AddBool;
Script_AddString_t Script_AddString;
Script_AddIString_t Script_AddIString;
Script_AddVector_t Script_AddVector;
Script_AddUndefined_t Script_AddUndefined;
Script_AddEntity_t Script_AddEntity;
#endif

Script_SetString_t Script_SetString;
Script_GetConstString_t Script_GetConstString;
oSpawn_t oSpawn;
oInitGentity_t oInitGentity;
oFreeEntity_t oFreeEntity;

oBG_AnimationIndexForString_t oBG_AnimationIndexForString;
CallSpawnEntity_t CallSpawnEntity;

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
        for(unsigned int i = 0; scriptFunctions[i].name; i++) {
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
        for(unsigned int i = 0; scriptMethods[i].name; i++) {
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

int load_callback(const char* file, const char* functionname, bool flag) {
    if(!Script_LoadScript(file) && !flag)
        Com_Error(ERR_DROP, "Could not find script '%s'.", file);

    int v4 = Script_GetFunctionHandle(file, functionname);
    if(!v4 && !flag)
        Com_Error(ERR_DROP, "Could not find label '%s'.", functionname, file);
	if(!flag || v4)
		printf("callback[%s] = %x\n", functionname, v4);
    return v4;
}

void GScr_typeof(int a1) {
	int type = Script_GetType(0);
	Script_AddString(Scr_GetVariableType(type));
}

char* Scr_GetVariableType(int type) {
	switch(type) {
		default:
		case VT_UNDEFINED: {
			return "undefined";
		}
		break;
		case VT_INT: {
			return "integer";
		} 
		break;
		case VT_FLOAT: {
			return "float";
		} 
		break;
		case VT_STRING: {
			return "string";
		} 
		break;
		case VT_VECTOR: {
			return "vector";
		} 
		break;
		case VT_LOCALIZED_STRING: {
			return "localized-string";
		} 
		break;
		case VT_OBJECT: {
			return "object";
		} 
		break;
		case VT_ARRAY: {
			return "array";
		} 
		break;
	}
}

/*
=============
FUNCTIONS
=============
*/

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

void GScr_SendServerCommand(int a1) {
	char* cmd = Script_GetString(0);
	SV_SendServerCommand(NULL, 1, cmd);
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
	FILE* f;
    if((f = fopen(name, "r"))) {
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

void QDECL gscr_loadgametypescript( void ) { //bleh
	char v1[64];
	snprintf(v1, 64, "maps/mp/gametypes/%s", g_gametype->string);
	printf("g_scr_data is located at %x\n", (int)&g_script_data);
	printf("//////////////////////////////////////////////////////////\nLoading gametype %s\n", v1);
	*(int*)&g_script_data[8] = load_callback(v1, "main", 0);
	*(int*)&g_script_data[12] = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_StartGameType", 0);
	*(int*)&g_script_data[16] = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerConnect", 0);
	*(int*)&g_script_data[20] = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerDisconnect", 0);
	*(int*)&g_script_data[24] = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerDamage", 0);
	*(int*)&g_script_data[28] = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerKilled", 0);
	
	/*callbackfunctionmain = load_callback("xtnded", "main");

    int result = Script_ExecThread(callbackfunctionmain, 0);
    Script_FreeThread(result);
	*/
	
	callbackPlayerCommand = load_callback("callback", "CodeCallback_PlayerCommand", 1);
	/*
	callbackEntityDamage = load_callback("callback", "CodeCallback_EntityDamage");
	callbackEntityKilled = load_callback("callback", "CodeCallback_EntityKilled");
	callbackEntityUse = load_callback("callback", "CodeCallback_EntityUse");
	callbackEntityThink = load_callback("callback", "CodeCallback_EntityThink");
	callbackEntityTouch = load_callback("callback", "CodeCallback_EntityTouch");
	*/
	
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

static int x_localized_string_index = 0;

int X_SetLocalizedStringIndex(int index, const char* str) {
	int i = index;
	
	if(!str || !*str)
		return 0;
	
	if(i >= 256)
		i = x_localized_string_index;
	
	if(i >= 256)
		i = x_localized_string_index = 0;
	
	SV_SetConfigstring(i + 1244, str);
	return i;
}

static int x_model_index = 1;

int X_ModelIndex(int index, const char* name) {
	int i = index;
	if(!name || !*name)
		return 0;
	
	if(i >= 256)
		i = x_model_index;
	
	if(i >= 256)
		i = x_model_index = 0;
		
	SV_SetConfigstring(i + 268, name);
	
	return i;
}
/*
int X_LocalizedStringIndex(const char* str) {
	int i;
	char s[MAX_STRING_CHARS];
	
	if(x_localized_string_index >= 256)
		x_localized_string_index = 0;
	
	if(!str || !*str)
		return 0;
	
	#if 0
	int start = 1244;
	
	for(i = 1; i < 256; i++) {
		SV_GetConfigstring( start + i, s, sizeof( s ) );
		if(!*s)
			break;
		if ( !strcmp( s, str ) )
			return i;
	}
	
	if(i == 256) { //idea to add ignoring strings, maybe first bit. or use half/half 128 for tmp storage
		i = 2047;
	}
	#endif
	
	SV_SetConfigstring(x_localized_string_index + 1244, str); //would require to call the precacheString(..) each time you use that string so let's just make a new function
	x_localized_string_index++;
	return i;
}*/

/*
	//TODO
		* reserve player's name up to <sv_maxclients> configstrings which can only be overwritten by connecting players.
		* reserve "real precached" strings up to e.g 128~?
		* reserve some more
		
		- Richard
*/

void Scr_fuckPrecacheString(int a1) {
	int argc = Script_GetNumParam();
	char* str;
	int index = 256;
	
	if(argc == 1) {
		str = Script_GetString(0);
	} else {
		index = Script_GetInt(0);
		str = Script_GetString(1);
	}
	
	int idx = X_SetLocalizedStringIndex(index, str);
	
	Script_AddInt(idx);
}

/*
//----- (00065E30) --------------------------------------------------------
signed int __cdecl G_LocalizedStringIndex(const char *s2)
{
  int v1; // edi@2
  signed int i; // ebx@3
  const char *v3; // eax@7
  int v4; // eax@10

  if ( !*s2 || (v1 = *(_DWORD *)&level[28], !s2) )
    return 0;
  for ( i = 1; i < 256; ++i )
  {
    v3 = (const char *)trap_GetConfigstringConst(i + 1244);
    if ( !*v3 )
      break;
    if ( !strcasecmp(v3, s2) )
      return i;
  }
  if ( !v1 )
  {
    if ( "localized string" )
    {
      v4 = (int)va("%s \"%s\" not precached", "localized string", s2);
      Scr_Error(v4);
    }
    return 0;
  }
  if ( i == 256 )
    G_Error("G_FindConfigstringIndex: overflow");
  trap_SetConfigstring(i + 1244, (int)s2);
  return i;
}
*/

void _GScr_AddFieldsForEntity() {
	void (*o)( void );
	*(int*)&o = GAME("GScr_AddFieldsForEntity");
	o();
	
	int class = *(int*)&g_script_data[420];
	int field_len = 0x21;
	
	void (*add)(int16_t,const char*,int16_t);
	*(int*)&add=GAME("Scr_AddClassField");
	
	scr_memberfield *it = &scr_entity_memberfields[0];
	
	for(; it->var != NULL; it++)
		add(class, it->var, ++field_len);
}

void _Scr_GetObjectField(int a1, int entNum, int a3) {
	/*
		//TODO
			* maybe add default offsets which can be looked up in a structure e.g x_clients
	*/
	scr_memberfield *field;
	if(!a1) { //check for hudelem fields
		if((uint16_t)(a3 & 0xC000) != 0xC000) { //add here for player fields
			if(a3 > 0x21) {
				field = &scr_entity_memberfields[a3 - 0x22];
				
				field->get( entNum /* maybe additional parameters? like flag for set/get */ );
			}
		}
	}
	
	int (*o)(int,int,int);
	*(int*)&o = 0x809A090;
	
	o(a1,entNum,a3);
}

void _Scr_SetObjectField(int a1, int entNum, int a3) {
	/*
		//TODO
			* maybe add default offsets which can be looked up in a structure e.g x_clients
	*/
	scr_memberfield *field;
	if(!a1) { //check for hudelem fields
		if((uint16_t)(a3 & 0xC000) != 0xC000) { //add here for player fields
			if(a3 > 0x21) {
				field = &scr_entity_memberfields[a3 - 0x22];
				
				field->set( entNum /* maybe additional parameters? like flag for set/get */ );
			}
		}
	}
	
	int (*o)(int,int,int);
	*(int*)&o = 0x809A070;
	
	o(a1,entNum,a3);
}

gitem_t *_bg_itemlist;

void scriptInitializing() {
	__jmp((int)GAME("GScr_LoadGameTypeScript"), (int)gscr_loadgametypescript);
	
	__call(GAME("GScr_LoadScripts")+0xab, _GScr_AddFieldsForEntity);
	
	//080AA35A                 call    sub_809A090 (Scr_GetObjectField)
	__call(0x80AA35A, _Scr_GetObjectField);
	//.text:080AA2E8                 call    sub_809A070
	__call(0x80AA2E8, _Scr_SetObjectField);
	
	//__jmp(GAME("GScr_AddFieldsForClient"), _GScr_AddFieldsForClient);
	
	
	//__jmp( GAME("Scr_SetClientField"), _Scr_SetClientField);
	
	/*
	int cfields_off = *(int*)( GAME("Scr_GetClientField")+0x1f );
	scr_client_memberfield *fields = (scr_client_memberfield*)cfields_off;
	
	printf("Printing client memberfields.\n");
	
	for(scr_client_memberfield *t = fields;t->var!=NULL;t++)
		printf("{\"%s\", 0x%x, 0x%x, 0x%x, 0x%x},\n", t->var, t->a, t->b, t->set, t->get);
	*/
	
	/*
	*(int*)(GAME("GScr_AddFieldsForClient")+0x30) = &scr_client_memberfields[0];
	*(int*)(GAME("GScr_AddFieldsForClient")+0x12) = &scr_client_memberfields[0];
	*(int*)(GAME("GScr_AddFieldsForClient")+0xd) = &scr_client_memberfields[0];
	*/
	
	g_script_data = (char*)dlsym(gamelib, "g_scr_data");
	
	script_const = (char*)GAME("scr_const");
	
	g_clients = (unsigned char*)GAME("g_clients");
	hudelems = (unsigned char*)GAME("g_hudelems");
	//cracking_hook_function(GAME("G_LocalizedStringIndex"), (int)X_LocalizedStringIndex);
	
	_bg_itemlist = (gitem_t*)GAME("bg_itemlist");
	
	CallSpawnEntity = (CallSpawnEntity_t)GAME("G_CallSpawnEntity");
	
	_SL_GetString = (_SL_GetString_t)GAME("SL_GetString");
	
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
	Script_AddIString = (Script_AddIString_t)dlsym(gamelib, "Scr_AddIString");
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
