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
#include "steamwrapper.h"

/*
	:: Scr_GetFunction/GetMethod ::
	
	CoD2 search for "parameter count exceeds 256" and go upwards
	CoD1 search for "parameter count exceeds 256" or "unknown (builtin) function '%s'" and go upwards
*/

void scr_field_awesome(int a1) {
	int argc = Scr_GetNumParam();
	
	if(argc == 0) {
		Scr_AddString("Laughing With");
		//works - Richard
	} else {
		char *s = Scr_GetString(0);
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
void PlayerCmd_SendConnectionlessPacket(int a1);
void GScr_killserver(int);
void GScr_typeof(int);

void Scr_fuckPrecacheString(int a1);

void GScr_getSite(int a1) {
	#if 0
	char* url = Scr_GetString(0);
	
	std::string result = get_site_result(url);
	Scr_AddString((char*)result.c_str());
	#endif
}

long long current_timestamp();

void GScr_seconds(int a1) {
	int s = (int)(current_timestamp()/1000);
	Scr_AddInt(s);
}

#ifdef xDEBUG
void MakeTrail(int self) {
	
	vec3_t start, end;
	Scr_GetVector(0, start);
	Scr_GetVector(1, end);
	gentity_t *tent = G_TempEntity(start, EV_RAILTRAIL);
	VectorCopy(end, tent->s.origin2);
	tent->s.otherEntityNum2 = 0;
}
#endif

#if 0

gentity_t *FindEntity( const char *key, const char *value ) {
	gentity_t *ent = NULL;
	for(int i = 0; i < level.num_entities; i++) {
		ent = &g_entities[i];
		if(!ent->inuse) {
			//dostuff
		}
	}
}

void ___test_debug_dont_really_blah(int a) {
	short classname = Scr_GetOffset(*(short *)&g_scr_data[420], "classname");
	
	if(classname >= 0) {
		
	}
}
#endif

void GScr_xtnded_anim(int a1) {
	/*int index = 0;
	int b = *(int*)0x82F5948;
	int v1 = *(int *)(b - 8 * index);*/
	
	//Scr_GetAnim((int)&v1, 0, 0);
	const char* str_anim = Scr_GetString(0);
	int anim_index = oBG_AnimationIndexForString(str_anim);
	//printf("anim = %d\n", anim_index);
	if(anim_index==-1)
		Scr_AddInt(0);
	else
		Scr_AddInt(anim_index);
}

#ifdef xDEBUG
void scr_newzombie(int);
void scr_spawnbrushmodel( int a1 ) ;
#endif

void Math_Scr_sqrt(int a) {
	float f = Scr_GetFloat(0);
	Scr_AddFloat( sqrt(f) );
}

#ifdef xDEBUG
void trace_For_me(int);
#endif

void gscr_malloc(int a) {
	int size = Scr_GetInt(0);
	void *p = malloc(size);
	if(!p) {
		Scr_AddUndefined();
		return;
	}
	Scr_AddInt((int)p);
}

void gscr_free(int a) {
	void *p = (void*)Scr_GetInt(0);
	if(!p)
		return;
	free(p);
}

void gscr_memset(int a) {
	void *s = (void*)Scr_GetInt(0);
	int c = Scr_GetInt(1);
	int n = Scr_GetInt(2);
	memset(s, c, n);
}

void gscr_memcpy(int a) {
	void *s1 = (void*)Scr_GetInt(0);
	void *s2 = (void*)Scr_GetInt(1);
	int n = Scr_GetInt(2);
	memcpy(s1, s2, n);
}

void Scr_GetArrayKeys(int a);
void Scr_PassArray(int);

void GScr_SetTempVec(unsigned n);

SCRIPTFUNCTION scriptFunctions[] = {
	//name, function, developer
	
	{"settempvec", GScr_SetTempVec, 0},
	
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
		MATH
	*/
	
	{"sqrt", Math_Scr_sqrt, 0},
	
	/*
		STRING
	*/
	{"strtok", Scr_StrTok, 0},
	{"issubstr", Scr_IsSubStr, 0},
	{"tolower", Scr_ToLower, 0},
	{"toupper", Scr_ToUpper, 0},
	{"ucfirst", Scr_ucfirst, 0},
	{"trim", Scr_trim, 0},
	#ifdef xDEBUG
	{"maketrail", MakeTrail, 0},
	#endif
	{"convert_string", Scr_convertToIString, 0},
	{"xprecachestring", Scr_fuckPrecacheString, 0},
	#ifdef xDEBUG
    {"spawn_zombie", scr_newzombie, 0},
    {"spawn_brushmodel", scr_spawnbrushmodel, 0},
    #endif
	{"printconsole", GScr_printconsole, 0},
    {"getsite", GScr_getSite, 0},
    {"seconds", GScr_seconds, 0},
    {"get_animation_index", GScr_xtnded_anim, 0},
    {"typeof", GScr_typeof, 0},
    {"sendservercommand", GScr_SendServerCommand, 0},
    {"trace", GScr_Trace, 0},
	#ifdef xDEBUG
    {"debugtrace", trace_For_me, 0},
	#endif
    {"md5", GScr_md5, 0},
    {"getarraykeys", Scr_GetArrayKeys, 0},
	
	{"passarray", Scr_PassArray, 0},
	
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

void PlayerCmd_IsUsingClient(int a1) {
	//Scr_AddInt(xtnded_clients[a1].clientusage);
	client_t *cl = getclient(a1);
	
	int clientbuild = atoi( Info_ValueForKey(cl->userinfo, "xtndedbuild") );
	
	if(clientbuild != clientversion) {
		Scr_AddBool(false);
		return;
	}
	Scr_AddBool(true);
}

void ScriptEnt_GetPosition(int);

SCRIPTFUNCTION scriptMethods[] = {
	//name, function, developer
	
	/*
	======
	ENTITY
	======
	*/
	{"getposition", ScriptEnt_GetPosition, 0},
	{"setbounds", ScriptEnt_SetBounds, 0},
	{"setsize", ScriptEnt_SetBounds, 0},
	{"settakedamage", ScriptEnt_SetTakeDamage, 0},
	{"setmaxs", ScriptEnt_SetMaxs, 0},
	{"setmins", ScriptEnt_SetMins, 0},
	{"setabsmins", ScriptEnt_SetAbsMin, 0},
	{"setabsmaxs", ScriptEnt_SetAbsMax, 0},
	{"setlight", ScriptEnt_SetLight, 0},
	{"showtoplayer", Ent_ShowToPlayer, 0},
		
	
	/*
	======
	PLAYER
	======
	*/
	{"getmuid", PlayerCmd_GetMUID, 0},
	{"setvelocity", PlayerCmd_SetVelocity, 0},
	{"getvelocity", PlayerCmd_GetVelocity, 0},
	{"getplayerangles", PlayerCmd_getPlayerAngles, 0},
	{"getip", PlayerCmd_getip, 0},
	{"ispure", PlayerCmd_ispure, 0},
	{"sendconnectionlesspacket", PlayerCmd_SendConnectionlessPacket, 0},
	{"sendservercommand", PlayerCmd_SendServerCommand, 0},
	{"sendgamestate", PlayerCmd_SendGamestate, 0},
	{"hasclient", PlayerCmd_IsUsingClient, 0},
	{"get_ip", PlayerCmd_getip, 0},
	{"getint", PlayerCmd_GetInt, 0},
	{"setint", PlayerCmd_SetInt, 0},
	{"getbyte", PlayerCmd_GetByte, 0},
	{"getfloat", PlayerCmd_GetFloat, 0},
	{"setfloat", PlayerCmd_SetFloat, 0},
	{"setbyte", PlayerCmd_SetByte, 0},
	{"getuserinfokey", PlayerCmd_GetUserInfoKey, 0},
	{"getuserinfo", PlayerCmd_GetUserInfo, 0},
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
	{"hasperk", PlayerCmd_HasPerk, 0},
	{"setperk", PlayerCmd_SetPerk, 0},
	{"unsetperk", PlayerCmd_UnsetPerk, 0},
	{"getping", PlayerCmd_GetPing, 0},
	{"setmaxspeed", PlayerCmd_SetMaxSpeed, 0},
	{"setmovespeedscale", PlayerCmd_SetMoveSpeedScale, 0},
	{NULL, NULL, 0}
};

game_script_data* g_scr_data;
scr_const_t* scr_const;
xscr_const_t xscr_const;

unsigned char* g_clients;
unsigned char* hudelems;

int callbackTest;
int callbackPlayerCommand;

bool scr_return = 0;

Scr_LoadScr_t Scr_LoadScript;
Scr_GetFunctionHandle_t Scr_GetFunctionHandle;

#ifdef _WIN32
Scr_ExecThread_t Scr_ExecThread = (Scr_ExecThread_t)0x481E10;
Scr_ExecEntThread_t Scr_ExecEntThread = (Scr_ExecEntThread_t)0x481EC0;
Scr_FreeThread_t Scr_FreeThread = (Scr_FreeThread_t)0x482070;
SL_ConvertToString_t SL_ConvertToString = (SL_ConvertToString_t)0x474EE0;
#else
Scr_ExecThread_t Scr_ExecThread = (Scr_ExecThread_t)0x80A95EC;
Scr_ExecEntThread_t Scr_ExecEntThread = (Scr_ExecEntThread_t)0x80a9674; //actually is Scr_ExecEntThreadNum
Scr_FreeThread_t Scr_FreeThread = (Scr_FreeThread_t)0x80A97D4; //RemoveRefToObject__FUs
SL_ConvertToString_t SL_ConvertToString;
#endif

SL_GetString_t SL_GetString;

/*
	Can't use Scr_GetMethod/Scr_GetFunction because those names already exists and aren't mangled.
*/

Scr_GetFunction_t Scr_GetFunction;
Scr_GetMethod_t Scr_GetMethod;

#ifdef _WIN32
Scr_MakeArray_t Scr_MakeArray = (Scr_MakeArray_t)0x483330;
Scr_AddArray_t Scr_AddArray = (Scr_AddArray_t)0x483380;
Scr_Error_t Scr_Error = (Scr_Error_t)0x483470;
#else
Scr_MakeArray_t Scr_MakeArray;
Scr_AddArray_t Scr_AddArray;
Scr_Error_t Scr_Error;
#endif

Scr_AddArrayStringIndexed_t Scr_AddArrayStringIndexed;

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
Scr_GetNumParam_t Scr_GetNumParam;
Scr_GetPointerType_t Scr_GetPointerType;
Scr_GetEntity_t Scr_GetEntity;
Scr_GetType_t Scr_GetType;
Scr_AddFloat_t Scr_AddFloat;
Scr_AddInt_t Scr_AddInt;
Scr_AddBool_t Scr_AddBool;
Scr_AddString_t Scr_AddString;
Scr_AddIString_t Scr_AddIString;
Scr_AddVector_t Scr_AddVector;
Scr_AddUndefined_t Scr_AddUndefined;
Scr_AddEntity_t Scr_AddEntity;
#endif

Scr_AllocString_t Scr_AllocString;
Scr_SetString_t Scr_SetString;
Scr_GetConstString_t Scr_GetConstString;

oBG_AnimationIndexForString_t oBG_AnimationIndexForString;
CallSpawnEntity_t CallSpawnEntity;

Scr_GetBool_t Scr_GetBool;
Scr_GetInt_t Scr_GetInt;
Scr_GetAnim_t Scr_GetAnim = (Scr_GetAnim_t)0x80A7FAC;
Scr_GetAnimsIndex_t Scr_GetAnimsIndex;
Scr_GetFloat_t Scr_GetFloat;
Scr_GetVector_t Scr_GetVector;
Scr_GetString_t Scr_GetString;
Scr_GetFunc_t Scr_GetFunc;
Scr_GetOffset_t Scr_GetOffset;

SCRIPTFUNCTIONCALL Scr_GetCustomFunction(const char** fname, int* fdev) {
    SCRIPTFUNCTIONCALL m = Scr_GetFunction(fname, fdev);
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

SCRIPTFUNCTIONCALL Scr_GetCustomMethod(const char** fname, int* fdev) {
    SCRIPTFUNCTIONCALL m = Scr_GetMethod(fname, fdev);
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

unsigned short load_callback(const char* file, const char* functionname, bool flag) {
    if(!Scr_LoadScript(file) && !flag)
        Com_Error(ERR_DROP, "Could not find script '%s'.", file);

    unsigned short v4 = Scr_GetFunctionHandle(file, functionname);
    if(!v4 && !flag)
        Com_Error(ERR_DROP, "Could not find label '%s'.", functionname, file);
	if(!flag || v4)
		cprintf(PRINT_GOOD|PRINT_UNDERLINE,"callback[%s] = %x\n", functionname, v4);
    return v4;
}

void GScr_typeof(int a1) {
	int type = Scr_GetType(0);
	Scr_AddString(Scr_GetVariableType(type));
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
	const char* txt = Scr_GetString(0);
	printf(txt);
}

void GScr_salt_password(int a1) {
	char* password = Scr_GetString(0);
	char* salt = Scr_GetString(1);
	Scr_AddString(get_pass_hash(password, salt));
}

void GScr_strpos(int a1) {
	char* haystack = Scr_GetString(0);
	char* needle = Scr_GetString(1);
	char* p = strstr(haystack, needle);
	if(p)
		Scr_AddInt(((int)p-(int)haystack));
	else
		Scr_AddInt(-1);
}

void GScr_SendServerCommand(int a1) {
	char* cmd = Scr_GetString(0);
	SV_SendServerCommand(NULL, 1, cmd);
}

void PlayerCmd_SendConnectionlessPacket(int a1) {
	char *msg = Scr_GetString(0);
	for(char *i = msg; *i != '\0'; *i++)
		if(*i == '\'')
			*i = '"';
			
	client_t *cl = getclient(a1);
	if(cl)
	NET_OutOfBandPrint(NS_SERVER, cl->netchan.remoteAddress, "%s", msg);
}

void GScr_Trace(int a1) {
    trace_t tr;
    vec3_t start, end, mins, maxs;
	Scr_GetVector(0, start);
	Scr_GetVector(1, mins);
	Scr_GetVector(2, maxs);
	Scr_GetVector(3, end);
	int ignore = Scr_GetInt(4);
	int mask = Scr_GetInt(5);
	int locational = 0;
	int staticmodels = 0;
	if(Scr_GetNumParam() > 6)
		locational = Scr_GetInt(6);
	if(Scr_GetNumParam() > 7)
		staticmodels = Scr_GetInt(7);
    
	void (*trace)(void*,float*,float*,float*,float*,int ignore,int contentmask,int locational,char *priorityMap,int staticmodels);
	*(int*)&trace = 0x80916F4;
	
	trace(&tr,start,mins,maxs,end,-1,mask,locational,NULL,staticmodels);
    Scr_MakeArray();
	
    Scr_AddVector(tr.endpos); Scr_AddArrayStringIndexed(scr_const->position);
    if((tr.entityNum - 1022) > 1)
	Scr_AddInt(tr.entityNum);//Scr_AddEntity(&g_entities[tr.entityNum]); //scr_addentity crashed sometime cba
	else
	Scr_AddUndefined();
	Scr_AddArrayStringIndexed(scr_const->entity);
	
    Scr_AddFloat(tr.fraction); Scr_AddArrayStringIndexed(scr_const->fraction);
    Scr_AddVector(tr.normal); Scr_AddArrayStringIndexed(scr_const->normal);
    Scr_AddInt(tr.contents); Scr_AddArrayStringIndexed(xscr_const.contents);
    Scr_AddInt(tr.surfaceFlags); Scr_AddArrayStringIndexed(scr_const->surfacetype);
    Scr_AddInt(tr.textureName); Scr_AddArrayStringIndexed(xscr_const.texturename);
}


void GScr_Cmd_Argc(int a1) {
    Scr_AddInt(Cmd_Argc());
}

void GScr_Cmd_Argv(int a1) {
    unsigned int idx = Scr_GetInt(0);
    if(idx > Cmd_Argc()) {
        Scr_AddString("");
        return;
    }
    Scr_AddString(Cmd_Argv(idx));
}

void GScr_trap_Argv(int a1) {
    unsigned int idx = Scr_GetInt(0);
    if(idx > Cmd_Argc()) {
        Scr_AddString("");
        return;
    }
    char buf[MAX_STRING_CHARS];
    Cmd_ArgvBuffer(idx, buf, sizeof(buf));
    Scr_AddString(buf);
}

void GScr_ConcatArgs(int a1) {
    unsigned int idx = Scr_GetInt(0);
    if(idx > Cmd_Argc()) {
        Scr_AddString("");
        return;
    }
    Scr_AddString(ConcatArgs(idx));
}

void GScr_md5(int a1) {
	char* str = Scr_GetString(0);
	Scr_AddString(get_md5(str));
}

void GScr_getChat(int a1) {
    unsigned int idx = Scr_GetInt(0);
    if(idx > Cmd_Argc()) {
        Scr_AddString("");
        return;
    }
	char* chat = ConcatArgs(idx);
	if(strlen(chat) == 0 || (chat[0] == 0x15 && strlen(chat)==1)) {
		Scr_AddString("");
		return;
	}
	if(chat[0] == 0x15)
		Scr_AddString(&chat[1]);
	else
		Scr_AddString(chat);
}

void GScr_return(int a1) {
    scr_return = 1;
}

bool Scr_Continue() {
    bool ret = scr_return;
    scr_return = 0;
    return(!ret);
}

/*
=============
FILE FUNCTIONS
=============
*/

void GScr_fopen(int entityIndex) {
    char* name = Scr_GetString(0);
    char* mode = Scr_GetString(1);
    FILE* f = fopen(name, mode);
    if(f)
        Scr_AddInt((int)(f));
    else
        Scr_AddInt(-1);
}

void GScr_fexists(int entityIndex) {
    char* name = Scr_GetString(0);
	FILE* f;
    if((f = fopen(name, "r"))) {
        fclose(f);
        Scr_AddBool(true);
    } else {
        Scr_AddBool(false);
    }
}

void GScr_fread(int entityIndex) {
	char *buf = NULL;
	size_t size = 0;
	
    int len = Scr_GetInt(0);
    FILE *f = (FILE*)(Scr_GetInt(1));
    
	if(f == NULL) {
        Scr_AddUndefined();
    } else {
		fseek(f, 0, SEEK_END);
        size = ftell(f);
		rewind(f);
        buf = (char *) malloc(size);
		fread(buf, size, 1, f);
        Scr_AddString(buf);
		free(buf);
    }
}

void GScr_fwrite(int entityIndex) {
    char* text = Scr_GetString(0);
    FILE *f = (FILE*)(Scr_GetInt(1));
    if(f == NULL) {
        Scr_AddBool(false);
    } else {
        Scr_AddBool(true);
        fprintf(f, text);
    }
}

void GScr_fsize(int entityIndex) {
    char* name = Scr_GetString(0);
    FILE *f = fopen(name, "r");
    size_t len;
    if(f) {
        fseek(f, 0, SEEK_END);
        len = ftell(f);
        rewind(f);
        Scr_AddInt((int)len);
    } else {
        Scr_AddInt(-1);
    }
}

void GScr_fclose(int entityIndex) {
    FILE *f = (FILE*)(Scr_GetInt(0));
    if(f) {
        Scr_AddBool(true);
        fclose(f);
    } else {
        Scr_AddBool(false);
    }
}

void Scr_LoadConsts() {
	xscr_const.texturename = Scr_AllocString("texturename", 1);
	xscr_const.contents = Scr_AllocString("contents", 1);
	xscr_const.perks[PERK_QUICK_RELOAD] = Scr_AllocString("sleight_of_hand", 1);
}

void _Scr_StartupGameType() {
	unsigned short ret = Scr_ExecThread(g_scr_data->startgametype, 0);
	Scr_FreeThread(ret);
	
	#ifdef BUILD_ECMASCRIPT
		duk_push_global_object(js_context);
		if(duk_has_prop_string(js_context, -1, "StartGameType")) {
			duk_get_prop_string(js_context, -1, "StartGameType");
			if(duk_pcall(js_context, 0) != 0)
				printf("Script Error (StartGameType): %s\n", duk_to_string(js_context, -1));
		}
		duk_pop(js_context);
	#endif
}

void _Scr_LoadGameType() {
	unsigned short ret = Scr_ExecThread(g_scr_data->gametype_main, 0);
	Scr_FreeThread(ret);
	
	#ifdef BUILD_ECMASCRIPT
		duk_push_global_object(js_context);
		if(duk_has_prop_string(js_context, -1, "LoadGameType")) {
			duk_get_prop_string(js_context, -1, "LoadGameType");
			if(duk_pcall(js_context, 0) != 0)
				printf("Script Error (LoadGameType): %s\n", duk_to_string(js_context, -1));
		}
		duk_pop(js_context);
	#endif
}

char onplayerconnect_result[64];

void _Scr_PlayerConnect(gentity_t *self) {
    unsigned short ret = Scr_ExecEntThread(self->s.number, 0, g_scr_data->playerconnect, 0);
    Scr_FreeThread(ret);
	#ifdef BUILD_ECMASCRIPT
	duk_push_global_object(js_context);
	
	duk_get_prop_string(js_context, -1, "players");
	duk_get_prop_index(js_context, -1, self->s.number);
	if(duk_has_prop_string(js_context, -3, "OnPlayerConnect")) {
		duk_get_prop_string(js_context, -3, "OnPlayerConnect");
		
		duk_dup(js_context, -2); //copy of player[idx] obj
		if(duk_pcall(js_context, 1) != 0)
			printf("Script Error (OnPlayerConnect): %s\n", duk_to_string(js_context, -1));
		if(duk_get_type(js_context, -1) == DUK_TYPE_UNDEFINED)
			*onplayerconnect_result = 0;
		else
			strncpy(onplayerconnect_result, duk_to_string(js_context, -1), sizeof(onplayerconnect_result));
		duk_pop(js_context);
	}
	duk_pop(js_context); //players
	duk_pop(js_context);
	#endif
}

void _Scr_PlayerDisconnect(self) 
	gentity_t *self;
{	
	#ifdef BUILD_ECMASCRIPT
	duk_push_global_object(js_context);
	
	duk_get_prop_string(js_context, -1, "players");
	duk_get_prop_index(js_context, -1, self->s.number);
	if(duk_has_prop_string(js_context, -3, "OnPlayerDisconnect")) {
		duk_get_prop_string(js_context, -3, "OnPlayerDisconnect");
		
		duk_dup(js_context, -2); //copy of player[idx] obj
		if(duk_pcall(js_context, 1) != 0)
			printf("Script Error (OnPlayerDisconnect): %s\n", duk_to_string(js_context, -1));
		duk_pop(js_context);
	}
	duk_pop(js_context); //players
	duk_pop(js_context);
	#endif
	
    unsigned short ret = Scr_ExecEntThread(self->s.number, 0, g_scr_data->playerdisconnect, 0);
    Scr_FreeThread(ret);
}

void _Scr_PlayerDamage(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int dmgflags, unsigned mod, int weapon, vec3_t point, vec3_t dir, int hitloc) {
    unsigned short (*_G_GetHitLocationString)(int) = (unsigned short(*)(int))GAME("G_GetHitLocationString");
	void (*Scr_AddConstString)(unsigned short) = (void(*)(unsigned short))GAME("Scr_AddConstString");
	unsigned short usHitLoc = _G_GetHitLocationString(hitloc);
	Scr_AddConstString(usHitLoc);
	int (*getinfo)(int);
	*(int*)&getinfo = GAME("BG_GetInfoForWeapon");
	unsigned info = (unsigned)getinfo(weapon);
	char **_modNames = (char**)GAME("modNames");
	
	if(dir)
		Scr_AddVector(dir);
	else
		Scr_AddUndefined();
	if(point)
		Scr_AddVector(point);
	else
		Scr_AddUndefined();
	Scr_AddString((char*)*(int*)(info + 4));
	if(mod < 0x18)
		Scr_AddString(_modNames[mod]);
	else
		Scr_AddString("badMOD");
	Scr_AddInt(dmgflags);
	Scr_AddInt(damage);
	if(attacker) Scr_AddEntity(attacker); else Scr_AddUndefined();
	if(inflictor) Scr_AddEntity(inflictor); else Scr_AddUndefined();
	unsigned short ret = Scr_ExecEntThread(self->s.number, 0, g_scr_data->playerdamage, 9);
    Scr_FreeThread(ret);
	#ifdef BUILD_ECMASCRIPT
	duk_push_global_object(js_context);
	
	duk_get_prop_string(js_context, -1, "players");
	duk_get_prop_index(js_context, -1, self->s.number);
	if(duk_has_prop_string(js_context, -3, "OnPlayerDamage")) {
		duk_get_prop_string(js_context, -3, "OnPlayerDamage");
		
		//printf("top after pop: %ld\n", (long) duk_get_top(js_context));
		duk_dup(js_context, -2); //copy of player[idx] obj
		if(inflictor)
			duk_get_prop_index(js_context, -4, inflictor->s.number);
		else
			duk_push_undefined(js_context);
		if(attacker)
			duk_get_prop_index(js_context, -5, attacker->s.number);
		else
			duk_push_undefined(js_context);
		duk_push_int(js_context, damage);
		duk_push_int(js_context, dmgflags);
		if(mod < 0x18)
			duk_push_string(js_context, _modNames[mod]);
		else
			duk_push_string(js_context, "badMOD");
		duk_push_string(js_context, (char*)*(int*)(info + 4));
		
		duk_idx_t arrIdx;
		
		#define JS_ADD_ARRAY_VECTOR(v) \
			arrIdx = duk_push_array(js_context); \
			duk_push_number(js_context, v[0]); \
			duk_put_prop_index(js_context, arrIdx, 0); \
			duk_push_number(js_context, v[1]); \
			duk_put_prop_index(js_context, arrIdx, 1); \
			duk_push_number(js_context, v[2]); \
			duk_put_prop_index(js_context, arrIdx, 2);
			
		if(point) {
			JS_ADD_ARRAY_VECTOR(point);
		} else {
			duk_push_undefined(js_context);
		}
		
		if(dir) {
			JS_ADD_ARRAY_VECTOR(dir);
		} else {
			duk_push_undefined(js_context);
		}
		
		char *csHitLoc = SL_ConvertToString(usHitLoc);
		
		duk_push_string(js_context, csHitLoc);
		
		//printf("top after pop: %ld\n", (long) duk_get_top(js_context));
		if(duk_pcall(js_context, 10) != 0)
			printf("Script Error (OnPlayerDamage): %s\n", duk_to_string(js_context, -1));
		//printf("top after pop: %ld\n", (long) duk_get_top(js_context));
		duk_pop(js_context);
	}
	duk_pop(js_context); //players
	duk_pop(js_context);
	#endif
}

void _Cmd_MenuResponse_f(gentity_t *self) {
	int svId = atoi(Cmd_Argv(1));
	
	cvar_t *svid = Cvar_Get("sv_serverId", "0", 72);
	
	if(svid->integer != svId)
		return;
	
	char cs[MAX_STRING_CHARS] = {0};
		
	unsigned menuIndex = atoi(Cmd_Argv(2));

	SV_GetConfigstring( 1180 + menuIndex, cs, sizeof( cs ) );
	
	#ifdef BUILD_ECMASCRIPT
		duk_push_global_object(js_context);
		
		duk_get_prop_string(js_context, -1, "players");
		duk_get_prop_index(js_context, -1, self->s.number);
		if(duk_has_prop_string(js_context, -3, "OnMenuResponse")) {
			duk_get_prop_string(js_context, -3, "OnMenuResponse");
			
			duk_dup(js_context, -2); //copy of player[idx] obj
			duk_push_string(js_context, cs);
			duk_push_string(js_context, Cmd_Argv(3));
			if(duk_pcall(js_context, 3) != 0)
				printf("Script Error (OnMenuResponse): %s\n", duk_to_string(js_context, -1));
			duk_pop(js_context);
		}
		duk_pop(js_context); //players
		duk_pop(js_context);
	#endif
	
	void (*o)(gentity_t*) = (void(*)(gentity_t*))GAME("Cmd_MenuResponse_f");
	o(self);
}

void GScr_LoadGametypeScript( void ) {
	extern int cpy_once;
	cpy_once = 0;
	
	char v1[64];
	snprintf(v1, 64, "maps/mp/gametypes/%s", g_gametype->string);
	g_scr_data->gametype_main = load_callback(v1, "main", 0);
	g_scr_data->startgametype = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_StartGameType", 0);
	g_scr_data->playerconnect = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerConnect", 0);
	g_scr_data->playerdisconnect = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerDisconnect", 0);
	g_scr_data->playerdamage = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerDamage", 0);
	g_scr_data->playerkilled = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerKilled", 0);
	
	callbackPlayerCommand = load_callback("callback", "CodeCallback_PlayerCommand", 1);
	
	extern int callbackEntityDamage, callbackEntityKilled;
	callbackEntityDamage = load_callback("callback", "EntityDamage", 1);
	callbackEntityKilled = load_callback("callback", "EntityDeath", 1);
	#ifdef BUILD_ECMASCRIPT
		duk_push_global_object(js_context);
		if(duk_has_prop_string(js_context, -1, "GSC_LoadGametypeScript")) {
			duk_get_prop_string(js_context, -1, "GSC_LoadGametypeScript");
			if(duk_pcall(js_context, 0) != 0)
				printf("Script Error (GSC_LoadGametypeScript): %s\n", duk_to_string(js_context, -1));
		}
		duk_pop(js_context);
	#endif

	Scr_LoadConsts();
}

static int x_localized_string_index = 128;

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


int X_LocalizedStringIndex(const char* str) {
	int i;
	char s[MAX_STRING_CHARS];
	
	if(x_localized_string_index >= 256)
		x_localized_string_index = 128;
	
	if(!str || !*str)
		return 0;
	
	int start = 1244;
	
	for(i = 1; i < 256; i++) {
		SV_GetConfigstring( start + i, s, sizeof( s ) );
		if(!*s)
			break;
		if ( !strcmp( s, str ) )
			return i;
	}
	
	if(i == 256) { //idea to add ignoring strings, maybe first bit. or use half/half 128 for tmp storage
		//overflow
		//i = 2047;
		i = x_localized_string_index;
	}
	
	SV_SetConfigstring(i + 1244, str); //would require to call the precacheString(..) each time you use that string so let's just make a new function
	++x_localized_string_index;
	return i;
}

/*
	//TODO
		* reserve player's name up to <sv_maxclients> configstrings which can only be overwritten by connecting players.
		* reserve "real precached" strings up to e.g 128~?
		* reserve some more
		
		- Richard
*/

void Scr_fuckPrecacheString(int a1) {
	int argc = Scr_GetNumParam();
	char* str;
	int index = 256;
	
	if(argc == 1) {
		str = Scr_GetString(0);
	} else {
		index = Scr_GetInt(0);
		str = Scr_GetString(1);
	}
	
	int idx = X_SetLocalizedStringIndex(index, str);
	
	Scr_AddInt(idx);
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
#if 0

void _GScr_AddFieldsForEntity() {
	void (*o)( void );
	*(int*)&o = GAME("GScr_AddFieldsForEntity");
	o();
	
	int class = *(int*)&g_scr_data[420];
	int field_len = 0x21;
	
	void (*add)(int16_t,const char*,int16_t);
	*(int*)&add=GAME("Scr_AddClassField");
	
	scr_memberfield *it = &scr_entity_memberfields[0];
	
	for(; it->var != NULL; it++)
		add(class, it->var, ++field_len);
}
#endif

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
				
				field->set( entNum  ); /* maybe additional parameters? like flag for set/get */
			}
		}
	}
	
	int (*o)(int,int,int);
	*(int*)&o = 0x809A070;
	
	o(a1,entNum,a3);
}

static long BG_StringHashValue( const char *fname ) {
	int i;
	long hash;
	char letter;

	hash = 0;
	i = 0;
	while ( fname[i] != '\0' ) {
		letter = tolower( fname[i] );
		hash += (long)( letter ) * ( i + 119 );
		i++;
	}
	if ( hash == -1 ) {
		hash = 0;   // never return -1
	}
	return hash;
}


#if 1

gentity_t *_fire_grenade(gentity_t *self, vec3_t start, vec3_t dir, int grenadeWPID) {
	gentity_t *bolt;
	gclient_t *gclient = self->client;
	
	bolt = G_Spawn();
	
	#if 0
	if(self->client && *(int*)((unsigned)gclient + 52))
		*(int*)((unsigned)bolt + 508) = LEVELTIME + *(int*)((unsigned)gclient + 52);
	else
		*(int*)((unsigned)bolt + 508) = LEVELTIME + 2500;
	#endif
	
	if(self->client)
		*(int*)((unsigned)gclient + 52) = 0; //reset grenadetimer
	
	*(int*)((unsigned)bolt + 508) = LEVELTIME + 35000;
	*(int*)((unsigned)bolt + 512) = GAME("G_ExplodeMissile");
	bolt->s.eType = 4;
	//bolt->svFlags = 136;
	bolt->s.weapon = grenadeWPID;
	bolt->ownerNum = self->s.number;
	bolt->parent = self;
	
	int (*getinfo)(int);
	*(int*)&getinfo = GAME("BG_GetInfoForWeapon");
	
	int info = getinfo(self->s.weapon);
	
	bolt->classname = scr_const->grenade;
	*(int *)((int)bolt + 568) = *(int *)(info + 448); //damage?
	*(int *)((int)bolt + 572) = *(int *)(info + 780); //radius?
	*(int *)((int)bolt + 576) = *(int *)(info + 784);
	*(int *)((int)bolt + 580) = *(int *)(info + 776);
	*(int *)((int)bolt + 584) = 3;
	*(int *)((int)bolt + 588) = 4;
	//bolt->s.eFlags = 50331648;
	bolt->clipmask = 41951377;
	bolt->s.pos.trType = 5;
	bolt->s.pos.trTime = LEVELTIME;
	VectorCopy(start, bolt->s.pos.trBase);
	cvar_t *speed = Cvar_Get("rocket_speed", "1", 0);
	VectorScale(dir, speed->value, bolt->s.pos.trDelta);
	
	bolt->s.apos.trType = 2;
	bolt->s.apos.trTime = LEVELTIME;
	_vectoangles(dir, bolt->s.apos.trBase);
	
	VectorCopy(start, bolt->currentOrigin);
	VectorCopy(bolt->s.apos.trBase, bolt->currentAngles);
	
	return bolt;
}

#endif


int _fire_rocket(gentity_t *self, vec3_t start, vec3_t dir) {
	int (*getinfo)(int);
	*(int*)&getinfo = GAME("BG_GetInfoForWeapon");
	
	int info = getinfo(self->s.weapon);
	gentity_t *ent = G_Spawn();
	
	_VectorNormalize(dir);
	ent->classname = scr_const->rocket;
	
	*(int *)((int)ent + 508) = LEVELTIME + 30000;
	*(int *)((int)ent + 512) = GAME("G_ExplodeMissile");
	ent->s.eType = 4;
	ent->s.eFlags |= 1u;
	ent->svFlags = 136;
	ent->s.weapon = self->s.weapon;
	ent->ownerNum = self->s.number;
	ent->parent = self;
	//undere here probably death info etc sMeansOfDeath
	*(int *)((int)ent + 568) = *(int *)(info + 448); //damage?
	*(int *)((int)ent + 572) = *(int *)(info + 780); //radius?
	*(int *)((int)ent + 576) = *(int *)(info + 784);
	*(int *)((int)ent + 580) = *(int *)(info + 776);
	*(int *)((int)ent + 584) = 5;
	*(int *)((int)ent + 588) = 6;
	ent->clipmask = 41951377;
	ent->s.pos.trType = 5; //default 2
	ent->s.pos.trTime = LEVELTIME - 50;
	
	VectorCopy(start, ent->s.pos.trBase);
	
	cvar_t *speed = Cvar_Get("rocket_speed", "1", 0);
	
	VectorScale(dir, speed->value, ent->s.pos.trDelta);
	
	VectorCopy(start, ent->currentOrigin);
	
	_vectoangles((int)ent + 36, (int)ent + 320);
	
	G_SetAngle(ent, ent->currentAngles);
	
	return ent;
}

gitem_t *bg_itemlist;

#ifdef xDEBUG
void __db() {
	if(Cmd_Argc()<3)
	return;
	
	int state = atoi(Cmd_Argv(1));
	int anim = atoi(Cmd_Argv(2));
	gentity_t *ent = (gentity_t*)( (int)g_entities );
	gclient_t *gclient = ent->client;
	
	*(int*)((int)gclient + 180) = state;
	*(int*)((int)gclient + 980) = anim;
	
	/*
	#define PR_OFF(x) printf(#x " offset is %d\n", ( (int)x - (int)ent ));
	PR_OFF(&ent->number)
	PR_OFF(&ent->eType)
	PR_OFF(&ent->eFlags)
	PR_OFF(&ent->pos)
	PR_OFF(&ent->apos)
	PR_OFF(&ent->unk[0])
	PR_OFF(&ent->groundEntityNum)
	PR_OFF(&ent->constantLight)
	PR_OFF(&ent->loopSound)
	PR_OFF(&ent->surfaceFlags)
	PR_OFF(&ent->modelindex)
	PR_OFF(&ent->clientNum)
	PR_OFF(&ent->weapon)
	PR_OFF(&ent->legsAnim)
	PR_OFF(&ent->torsoAnim)
	PR_OFF(&ent->leanf)
	PR_OFF(&ent->loopfxid)
	PR_OFF(&ent->hintstring)
	PR_OFF(&ent->animMovetype)
	PR_OFF(&ent->linked)
	PR_OFF(&ent->svFlags)
	PR_OFF(&ent->mins)
	PR_OFF(&ent->maxs)
	PR_OFF(&ent->contents)
	PR_OFF(&ent->absmin)
	PR_OFF(&ent->absmax)
	PR_OFF(&ent->currentOrigin)
	PR_OFF(&ent->currentAngles)
	PR_OFF(&ent->ownerNum)
	PR_OFF(&ent->r_eventTime)
	PR_OFF(&ent->client)
	PR_OFF(&ent->inuse)
	PR_OFF(&ent->physicsObject)
	PR_OFF(&ent->takedamage)
	PR_OFF(&ent->active)
	PR_OFF(&ent->moverState)
	PR_OFF(&ent->modelindex2)
	PR_OFF(&ent->classname)
	PR_OFF(&ent->spawnflags)
	PR_OFF(&ent->flags)
	PR_OFF(&ent->eventTime)
	PR_OFF(&ent->freeAfterEvent)
	PR_OFF(&ent->unlinkAfterEvent)
	PR_OFF(&ent->physicsBounce)
	PR_OFF(&ent->clipmask)
	PR_OFF(&ent->framenum)
	PR_OFF(&ent->parent)
	PR_OFF(&ent->nextTrain)
	printf("sizeof trajectory_t = %d\n", sizeof(trajectory_t));
	*/
	
	/*
	int a = *(int*)(GAME("BG_AnimationIndexForString")+0xa);

	void dumpstuff(int *base, int end, const char *fn);
	dumpstuff(a, 92*500, "ngt");
	*/
	
	
	//void *q = *(void**)( GAME("GScr_LoadScripts")+0xcd );
	
	/*
	animation_t *a = (animation_t*)GAME("bgs");
	int c = *(int*)( GAME("bgs") + 47104 );
	
	FILE *fp = fopen("/home/ext/tmp/anims.txt", "w");
	if(!fp)
	return;
	for(int i = 0; i < c; i++) {
		fprintf(fp, "%d : {\"%s\", %d, %d, %d, %d, %d, %d, %d}\n", i, a[i].name, a[i].a, a[i].b, a[i].c, a[i].d, a[i].e, a[i].f, a[i].g);
	}
	fclose(fp);
	*/
}

char* xanim_getname(int a) {
	printf("a = %d [%x]\n", a, a);
	return "pb_stand_alert";
}

void Prop_Check_Ground( gentity_t *self ) {
	vec3_t mins, maxs;
	vec3_t start, end;
	trace_t tr;

	VectorCopy( self->currentOrigin, start );
	VectorCopy( self->currentOrigin, end );

	end[2] -= 4;

	VectorCopy( self->mins, mins );
	VectorCopy( self->maxs, maxs );

	SV_Trace( &tr, start, mins, maxs, end, self->s.number, -1 );
	
	if ( tr.fraction == 1 ) {
		self->s.groundEntityNum = ENTITYNUM_NONE;
	} else {
		self->s.groundEntityNum = tr.entityNum;
	}

}

void zombie_think(gentity_t *self);

void prop_thr(gentity_t *self) {
	if(self->s.groundEntityNum == ENTITYNUM_NONE) {
		
		return;
	}
	
	*(int*)((int)self + EOFF_THINK) = zombie_think;
}

void zombie_think(gentity_t *self) {
	trace_t tr;
	
	void (*linkentity)(gentity_t*);
	*(int*)&linkentity = GAME("trap_LinkEntity");
	void (*unlinkentity)(gentity_t*);
	*(int*)&unlinkentity = GAME("trap_UnlinkEntity");
	
	unlinkentity(self);
	
	_BG_EvaluateTrajectory(&self->s.pos, LEVELTIME, self->s.pos.trBase);
	
	if ( LEVELTIME > self->s.pos.trDuration ) {
		VectorClear( self->s.pos.trDelta );
		self->s.pos.trDuration = 0;
		self->s.pos.trType = TR_STATIONARY;
	} else
	{
		vec3_t mins, maxs;

		VectorCopy( self->mins, mins );
		VectorCopy( self->maxs, maxs );

		mins[2] += 1;

		SV_Trace( &tr, self->currentOrigin, mins, maxs, self->s.pos.trBase, self->s.number, 1);

		if ( tr.fraction == 1 ) {
			VectorCopy( self->s.pos.trBase, self->currentOrigin );
		} else
		{
			VectorCopy( self->currentOrigin, self->s.pos.trBase );
			VectorClear( self->s.pos.trDelta );
			self->s.pos.trDuration = 0;
			self->s.pos.trType = TR_STATIONARY;
		}

	}
	
	if(self->s.groundEntityNum == ENTITYNUM_NONE) {
		self->physicsObject = qtrue;
		self->physicsBounce = 0.5;
		
		self->s.pos.trDelta[2] -= 200;
		self->s.pos.trType = TR_GRAVITY;
		self->s.pos.trTime = LEVELTIME;
		*(int*)((int)self + EOFF_THINK) = prop_thr;
		
	}
	
	Prop_Check_Ground(self);
	
	*(int*)((int)self + EOFF_NEXTTHINK) = LEVELTIME + 50;
	
	linkentity(self);
}

void moveit( gentity_t *ent, float yaw, float dist ) {
	void (*linkentity)(gentity_t*);
	*(int*)&linkentity = GAME("trap_LinkEntity");
	void (*unlinkentity)(gentity_t*);
	*(int*)&unlinkentity = GAME("trap_UnlinkEntity");
	vec3_t move;
	vec3_t origin;
	trace_t tr;
	vec3_t mins, maxs;

	yaw = yaw * M_PI * 2 / 360;

	move[0] = cos( yaw ) * dist;
	move[1] = sin( yaw ) * dist;
	move[2] = 0;

	VectorAdd( ent->currentOrigin, move, origin );

	mins[0] = ent->mins[0];
	mins[1] = ent->mins[1];
	mins[2] = ent->mins[2] + .01;

	maxs[0] = ent->maxs[0];
	maxs[1] = ent->maxs[1];
	maxs[2] = ent->maxs[2] - .01;

	SV_Trace( &tr, ent->currentOrigin, mins, maxs, origin, ent->s.number, -1 );

	if ( ( tr.endpos[0] != origin[0] ) || ( tr.endpos[1] != origin[1] ) ) {
		mins[0] = ent->mins[0] - 2.0;
		mins[1] = ent->mins[1] - 2.0;
		maxs[0] = ent->maxs[0] + 2.0;
		maxs[1] = ent->maxs[1] + 2.0;

		SV_Trace( &tr, ent->currentOrigin, mins, maxs, origin, ent->s.number, -1 );
	}

	VectorCopy( tr.endpos, ent->currentOrigin );

	VectorCopy( ent->currentOrigin, ent->s.pos.trBase );

	linkentity( ent );

	//DropToFloor( ent );
}

void prop_touch( gentity_t *self, gentity_t *other ) {
	void (*linkentity)(gentity_t*);
	*(int*)&linkentity = GAME("trap_LinkEntity");
	void (*unlinkentity)(gentity_t*);
	*(int*)&unlinkentity = GAME("trap_UnlinkEntity");
	
	if(self->physicsObject)
		return;
	
	unlinkentity(self);
	
	float ratio;
	vec3_t v;

	if ( other->currentOrigin[2] > ( self->currentOrigin[2] + 10 + 15 ) ) {
		return;
	}

	ratio = 8;
	VectorSubtract( self->currentOrigin, other->currentOrigin, v );
	moveit( self, _vectoyaw( v ), ( 20 * ratio * 50 ) * .001 );
	
	//*(int*)((int)self + EOFF_NEXTTHINK) = LEVELTIME + 1500;
	T_LinkEntity(self);
}

void scr_newzombie(int a1) {
	vec3_t org;
	Scr_GetVector(0, org);
	int model = Scr_GetInt(1);
	void (*linkentity)(gentity_t*);
	*(int*)&linkentity = GAME("trap_LinkEntity");
	void (*dobju)(gentity_t*);
	*(int*)&dobju = GAME("G_DObjUpdate");
	void (*inite)(gentity_t*);
	*(int*)&inite = GAME("SP_script_brushmodel");
	
	gentity_t *ent = (gentity_t*)oSpawn();
	xentity_t *xent = &xentities[ent->s.number];
	
	ent->classname = scr_const->rocket;
	
	ent->spawnflags = 0;
	VectorCopy( org, ent->currentOrigin );
	
	ent->s.modelindex = model;
	
	inite(ent);
	
	/*
	*(int*)((int)ent + 512) = (int)zombie_think;
	*(int*)((int)ent + 508) = *(int *)&level[488] + 50;
	*(int*)((int)ent + 524) = prop_touch;
	
	ent->clipmask = -1;
	ent->contents = 1081868552;
	
	VectorSet( ent->mins, -12, -12, 0 );
	VectorSet( ent->maxs, 12, 12, 48 );
	*/
	
	Scr_AddEntity(ent);
}

void _SP_script_brushmodel( gentity_t *self ) {
	printf("self->modelindex = %d, self->modelindex2 = %d\n", self->s.modelindex, self->modelindex2);
	
	T_SetBrushModel(self);
	void (*initscriptmover)(gentity_t*);
	*(int*)&initscriptmover=GAME("InitScriptMover");
	
	initscriptmover(self);
	
	self->contents = 1;
	
	T_LinkEntity(self);
}

void scr_spawnbrushmodel( int a1 ) {
	gentity_t *ent;
	vec3_t org;
	int model;
	Scr_GetVector(0, &org);
	model = Scr_GetInt(1);
	
	ent = _G_Spawn();
	
	ent->classname = scr_const->script_brushmodel;
	
	ent->s.modelindex = model;
	_G_SetOrigin( ent, org );
	
	_SP_script_brushmodel( ent );
	
	ent->contents = CONTENTS_BODY;
	
	T_LinkEntity( ent );
}

typedef struct {
	char    *name;
	void ( *spawn )( gentity_t *ent );
} spawn_t;
#endif

void test_debug() {
}

#ifdef xDEBUG

static unsigned char trace_buf[0x30];
#include <signal.h>
void trace_For_me(int a) {
	vec3_t start, end, mins, maxs;
	Scr_GetVector(0,start);
	Scr_GetVector(3,end);
	Scr_GetVector(1,mins);
	Scr_GetVector(2,maxs);
	
	int contentmask = Scr_GetInt(4);
	
	void (*trace)(void*,float*,float*,float*,float*,int ignore,int contentmask,int locational,char *priorityMap,int staticmodels);
	*(int*)&trace = 0x80916F4;
	
	trace(&trace_buf[0],start,mins,maxs,end,-1,contentmask,0,NULL,1);
	
	printf("trace_buf = %x\n", &trace_buf[0]);
	
	raise(SIGINT);
}
#endif

void _Scr_GetGenericField(int a1, int a2, int a3) {
//	printf("Scr_GetGenericField: a1 = %x, a2 = %x, a3= %x\n", a1, a2, a3);
	char *s = NULL;//gcc error init stuff etc
	switch(a2) {
		case 0:
			Scr_AddInt(*(int*)(a3 + a1));
		break;
		
		case 1:
			Scr_AddFloat(*(float*)(a3 + a1));
		break;
		
		case 2:
			s = (char*)(a3 + a1);
			printf("AddString: %s\n", s);
			if(a3 == 0x21B4) { //name offset in structure
				//do stuff
			}
			//check for NULL e.g?
			if(s == NULL)
				Scr_AddString("");
			else
				Scr_AddString(s);
		break;
		
		case 3: {
			void (*Scr_AddConstString)(short) = (void(*)(short))GAME("Scr_AddConstString");
			short index = *(short*)(a3 + a1);
			if(index) {
				Scr_AddConstString(index);
				//printf("AddConstString: %s\n", SL_ConvertToString(index));
			}
		}
		break;
		
		case 4: {
			float *vec = (float*)(a3 + a1);
			Scr_AddVector(vec);
		//	printf("vector: x: %f, y: %f, z: %f\n", vec[0], vec[1], vec[2]);
		}
		break;
		
		case 5: {
			gentity_t *ent = *(gentity_t**)(a3 + a1);
			if(ent)
				Scr_AddEntity(ent);
		}
		break;
		
		case 6: {
			float org[3] = {0, *(float*)(a3 + a1), 0};
			Scr_AddVector(org);
		}
		break;
		
		case 7: {
			void (*Scr_AddObject)(short) = (void(*)(short))GAME("Scr_AddObject");
			short index = *(short*)(a3 + a1);
			if(index)
				Scr_AddObject(index);
		}
		break;
		
		case 8: {
			unsigned char modelindex = *(unsigned char*)(a3 + a1);
			const char* (*get_model_name_from_index)(unsigned char) = (const char*(*)(unsigned char))GAME("G_ModelName");
			const char *modelname = get_model_name_from_index(modelindex);
			Scr_AddString(modelname);
			//printf("ModelName: %s\n", modelname);
		}		
		break;
	}
}


unsigned short (*GetVariableName)(unsigned short) = (unsigned short(*)(unsigned short))0x80A3060;
unsigned short (*GetNextVariable)(unsigned short) = (unsigned short(*)(unsigned short))0x80A3028; //idk original funcname
unsigned short (*get_var_by_idx)(unsigned short) = (unsigned short(*)(unsigned short))0x80A3210;

unsigned short Scr_GetArray(int index) {
	int *params = (int*)0x82F5944;
	int stack = *(int*)0x82F5948;
	int base = (stack - 8 * index);
	int vartype = *(int*)(base + 4);
	int *params2 = (int*)0x82F5888;
	//printf("params=%d,params2=%d\n",*params,*params2);


	//printf("stack type = %s\n", Scr_GetVariableType(*(int*)(base + 4)));

	if(*params > index) {
		//printf("stack type = %s(%d)\n", Scr_GetVariableType(vartype), vartype);
		//printf("base = 0x%x\n", base);
		//raise(SIGINT);
		if(vartype == VT_OBJECT)
			return *(unsigned short*)base;
	}
	//error param doesn't exist.
	//Scr_Error("error parameter does not exist!\n");
	printf("scr_error\n");
	return 0;
}

unsigned GetVariableXD(unsigned int idx) {
	int *variables = (int*)0x81F17C0;
	
	return ((unsigned)variables + 12 * idx);
}

void Scr_PassArray(int n) {
	
	
	
	unsigned short i, var, a, arr;
	for(a = 0; a < Scr_GetNumParam(); a++) {
		arr = Scr_GetArray(a);
		for(i = GetNextVariable(arr), var = get_var_by_idx(i); i != 0;) {
			printf("%d: %s = %s (var=%x)\n", i, SL_ConvertToString(GetVariableName(i)), SL_ConvertToString(var), GetVariableXD(var));		
			
			i = GetNextVariable(i);
			var = get_var_by_idx(i);
		}
	}
}


void Scr_GetArrayKeys(int a) {
	unsigned short arrIndex = Scr_GetArray(0);
	
	Scr_MakeArray();

	unsigned short i, var;
	for(i = GetNextVariable(arrIndex); i != 0;) {
		//printf("%d: %s = %s\n", i, SL_ConvertToString(GetVariableName(i)), SL_ConvertToString(var));
		
		Scr_AddString(SL_ConvertToString(GetVariableName(i)));
		Scr_AddArray();

		i = GetNextVariable(i);
		var = get_var_by_idx(i);
	}
}

#if 0
void Scr_GetArrayKeys2(int a) { //old messy for reference
	unsigned short (*get_istring_index)(int);
	*(int*)&get_istring_index = 0x80A846C;


	int (*arraysize)(unsigned short) = (int(*)(unsigned short))0x80A3010;

	unsigned short str = get_istring_index(1);

	printf("str = %d (%x), %s\n", str, str, SL_ConvertToString(str));

	int array_index = GetArrayKeys(0); //0x3cff
	unsigned short (*GetVariableName)(unsigned short) = (unsigned short(*)(unsigned short))0x80A3060;
	unsigned short (*get_idx)(unsigned short) = (unsigned short(*)(unsigned short))0x80A3028;

	unsigned short (*get_var_by_idx)(unsigned short) = (unsigned short(*)(unsigned short))0x80A3210;


	unsigned short i, var;
	for(i = get_idx(array_index), var = get_var_by_idx(i); i != 0;) {
		//printf("i (%d) = %s\n", i, SL_ConvertToString(i));
		
		printf("%d: %s > %s\n", i, SL_ConvertToString(var), SL_ConvertToString(GetVariableName(i)));
		i = get_idx(i);
		var = get_var_by_idx(i);
	}
	printf("array name = %s\n", SL_ConvertToString(get_variable_name(array_index)));
	printf("arraysize = %d(%x)\n",arraysize(array_index),arraysize(array_index));

	int v9 = array_index * 0xc;
	printf("v9 = %x\n", v9);
	unsigned char *ast = (unsigned char*)(0x81F17C4 + v9);

	if( (*ast & 0x1f) != 0xf) {

		printf("not an array!\n");
		Scr_AddUndefined();
		return;
	}

	VariableValue *arr = (VariableValue*)(0x81F17C0 + v9);
	printf("arr =  %x\n", arr);
	raise(SIGINT);
	Scr_MakeArray();
	Scr_AddString("hoi");
	Scr_AddArray();
}
#endif

char rct_bytes[5];

void _Scr_RunCurrentThreads() {
	
	memcpy((void*)GAME("Scr_RunCurrentThreads"), (void*)&rct_bytes[0], 5);
	
	void (*o)() = (void(*)())GAME("Scr_RunCurrentThreads");
	
	o();
	__jmp(GAME("Scr_RunCurrentThreads"), (unsigned)_Scr_RunCurrentThreads);
	
	#ifdef BUILD_ECMASCRIPT
	if(js_context != NULL) {
		duk_push_global_object(js_context);
		if(duk_has_prop_string(js_context, -1, "RunFrame")) {
			duk_get_prop_string(js_context, -1, "RunFrame");
			if(duk_pcall(js_context, 0) != 0)
				printf("Script Error (RunFrame): %s\n", duk_to_string(js_context, -1));
		}
		if(duk_has_prop_string(js_context, -1, "EventLoop_RunFrame")) {
			duk_get_prop_string(js_context, -1, "EventLoop_RunFrame");
			
			if(duk_pcall(js_context, 0) != 0)
				printf("Script Error (EventLoop_RunFrame): %s\n", duk_to_string(js_context, -1));
		}
		
		duk_pop(js_context);
	}
	#endif
	
}

#ifdef BUILD_ECMASCRIPT

static void __call_ret(unsigned int off, unsigned int loc) {
	mprotect((void *)off, 6, PROT_READ | PROT_WRITE | PROT_EXEC);
	int foffset = loc - (off + 5);
	*(unsigned char*)off = 0xe8; //call
	memcpy((void *)(off + 1), &foffset, 4);
	*(unsigned char*)(off + 5) = 0xc3; //retn
}

typedef struct {
	char *name;
	int type;
} scr_func_helper_pair;

/* UNDEFINED means no args and VT_INT may be a short */
const static scr_func_helper_pair scr_func_helper_names[] = {
	{"GetBool",               VT_INT},
	{"GetInt",                VT_INT},
	{"GetAnim",               VT_END_OF_LIST},
	{"GetAnimTree",           VT_END_OF_LIST},
	{"GetFloat",              VT_FLOAT},
	{"GetString",             VT_STRING},
	{"GetConstString",        VT_LOCALIZED_STRING},
	{"GetDebugString",        VT_STRING}, /* not sure */
	{"GetIString",            VT_STRING},
	{"GetConstIString",       VT_LOCALIZED_STRING},
	{"GetVector",             VT_VECTOR},
	{"GetFunc",               VT_FUNCTION},
	{"GetEntityNum",          VT_END_OF_LIST},
	{"GetType",               VT_END_OF_LIST},
	{"GetPointerType",        VT_END_OF_LIST},
	{"GetNumParam",           VT_END_OF_LIST},
	{"AddBool",               VT_INT},
	{"AddInt",                VT_INT},
	{"AddFloat",              VT_FLOAT},
	{"AddAnim",               VT_ANIMATION},
	{"AddUndefined",          VT_UNDEFINED},
	{"AddEntityNum",          VT_INT},
	{"AddStruct",             VT_OBJECT},
	{"AddString",             VT_STRING},
	{"AddIString",            VT_STRING},
	{"AddConstString",        VT_LOCALIZED_STRING},
	{"AddVector",             VT_VECTOR},
	{"AddObject",             VT_OBJECT},
	{"AddArray",              VT_UNDEFINED},
	{"AddArrayStringIndexed", VT_INT},
	//{"MakeArray", VT_ARRAY},
	{NULL, 0}
};

typedef struct {
	//Generated by macro, some could be shorts instead of int and most that you won't need/don't use are wrong.
	bool(*GetBool)(int);
	int(*GetInt)(int);
	int(*GetAnim)(int);
	int(*GetAnimTree)(int);
	float(*GetFloat)(int);
	char* (*GetString)(int);
	int(*GetConstString)(int);
	int(*GetDebugString)(int);
	char* (*GetIString)(int);
	int(*GetConstIString)(int);
	float* (*GetVector)(int);
	int(*GetFunc)(int);
	int(*GetEntityNum)(int);
	int(*GetType)(int);
	int(*GetPointerType)(int);
	int(*GetNumParam)(int);
	void(*AddBool)(bool);
	void(*AddInt)(int);
	void(*AddFloat)(float);
	void(*AddAnim)(int);
	void(*AddUndefined)();
	void(*AddEntityNum)(int);
	void(*AddStruct)(int);
	void(*AddString)(char*);
	void(*AddIString)(char*);
	void(*AddConstString)(int);
	void(*AddVector)(float*);
	void(*AddObject)(int);
	void(*AddArray)();
	void(*AddArrayStringIndexed)(int);
	int(*MakeArray)(/*noidea*/); //this sneaky is actually placed before addarray but its not in memory
	int(*Error)(/*noidea*/);
	int(*ErrorWithDialogMessage)(/*noidea*/);
	int(*ParamError)(/*noidea*/);
	int(*ObjectError)(/*noidea*/);
	int(*SetDynamicEntityField)(/*noidea*/);
	int(*FreeEntityNum)(/*noidea*/);
	int(*GetEntityId)(int);
	int(*SetClassMap)(/*noidea*/);
	int(*RemoveClassMap)(/*noidea*/);
	void(*AddClassField)(int);
	void(*AddFields)(int);
	int(*FindField)(/*noidea*/);
	int(*GetOffset)(int);
	int(*CopyEntityNum)(/*noidea*/);
	int(*Init)(/*noidea*/);
	int(*Shutdown)(/*noidea*/);
	int(*Abort)(/*noidea*/);
	int(*SetLoading)(/*noidea*/);
	int(*AllocGameVariable)(/*noidea*/);
	int(*InitSystem)(/*noidea*/);
	int(*GetChecksum)(int);
	int(*HasSourceFiles)(/*noidea*/);
	int(*SaveSource)(/*noidea*/);
	int(*LoadSource)(/*noidea*/);
	int(*SkipSource)(/*noidea*/);
	int(*SavePre)(/*noidea*/);
	int(*SavePost)(/*noidea*/);
	int(*SaveShutdown)(/*noidea*/);
	int(*LoadPre)(/*noidea*/);
	int(*LoadShutdown)(/*noidea*/);
	int(*LoadScript)(/*noidea*/);
	int(*FindAnimTree)(/*noidea*/);
	int(*FindAnim)(/*noidea*/);
	int(*GetFunctionHandle)(int);
	int(*BeginLoadScripts)(/*noidea*/);
	int(*BeginLoadAnimTrees)(/*noidea*/);
	int(*EndLoadScripts)(/*noidea*/);
	int(*EndLoadAnimTrees)(/*noidea*/);
	int(*PrecacheAnimTrees)(/*noidea*/);
	int(*FreeScripts)(/*noidea*/);
	int(*FreeGameVariable)(/*noidea*/);
	int(*ShutdownSystem)(/*noidea*/);
	int(*IsSystemActive)(/*noidea*/);
	int(*ExecThread)(/*noidea*/);
	int(*ExecEntThreadNum)(/*noidea*/);
	void(*AddExecThread)(int);
	void(*AddExecEntThreadNum)(int);
	int(*IsThreadAlive)(/*noidea*/);
	int(*FreeThread)(/*noidea*/);
	int(*ConvertThreadToSave)(/*noidea*/);
	int(*ConvertThreadFromLoad)(/*noidea*/);
	int(*SetString)(/*noidea*/);
	int(*AllocString)(/*noidea*/);
	int(*SetTime)(/*noidea*/);
	int(*RunCurrentThreads)(/*noidea*/);
	int(*ResetTimeout)(/*noidea*/);
	int(*NotifyNum)(/*noidea*/);
	int(*SL_ConvertToString)(/*noidea*/);
	int(*SL_GetString)(/*noidea*/);
	int(*SL_GetLowercaseString)(/*noidea*/);
	int(*SL_FindLowercaseString)(/*noidea*/);
	int(*NotifyId)(/*noidea*/);
	int(*CreateCanonicalFilename)(/*noidea*/);
	int(*GetAnimsIndex)(int);
	int(*GetAnims)(int);
} SCR_FUNCS;

SCR_FUNCS Scr;

int gsc_gettype(int i) {
	return *(int*)(*(unsigned*)0x82F5948 - 8 * i + 4);
}

unsigned gsc_hook_caller;

typedef union {
	int i;
	char *s;
	float f;
	double d;
	float *fp;
	void *p;
} UnionValue;

static vec3_t UnionVector;
extern duk_context *js_gsc_duk_context;
extern int js_gsc_func_args;
			
void gsc_hook_to_js_cb(int a1, int a2, UnionValue value /*float acted weird on smth else*/) {
	//printf("addr = %02X\n", a1);
	
	int *scr_funcs = (int*)0x8161200;
	/* for getting the func from the cod_lnxded instead of game.so */
	for(int i = 0; scr_func_helper_names[i].name; i++) {
		char *szFuncName = va("Scr_%s", scr_func_helper_names[i].name);
		unsigned faddr = GAME( szFuncName );
		//printf("faddr = %02X, addr = %02X\n", faddr, addr);
		
		if(faddr == gsc_hook_caller) {
			/*
			if(strstr(szFuncName, "String")) {
				printf("found %s\n", szFuncName);
				//printf("a1=%02X,a2=%02X,a3=%02X,a4=%02X,a5=%02X\n", a1,a2,a3,a4,a5);
				printf("a3=%s\n", (char*)a3);
			}
			*/
			
			//return scr_funcs[i];
			
			//int type = gsc_gettype(0);
			int type = scr_func_helper_names[i].type; /* we should be able to get the type without having to put it on stack */
			
			if(strstr(szFuncName, "Add")) {
				switch(type) {
					
					case VT_STRING: {
						//printf("%s(%s)\n", szFuncName, (char*)value.s);
						if(js_gsc_duk_context)
							duk_push_string(js_gsc_duk_context, value.s);
					} break;
					
					case VT_INT: {
						if(js_gsc_duk_context) {
							//printf("%s(%d)\n", szFuncName, (int)value.i);
						
							duk_push_int(js_gsc_duk_context, value.i);
						}
					} break;
					
					case VT_FLOAT: {
						//printf("%s(%f)\n", szFuncName, value.f);
						if(js_gsc_duk_context)
							duk_push_number(js_gsc_duk_context, (double)value.f);
					} break;
					
					case VT_UNDEFINED: {
						//printf("%s()\n");
						if(js_gsc_duk_context)
							duk_push_undefined(js_gsc_duk_context);
					} break;
					
					case VT_VECTOR: {
						duk_idx_t arrIdx;
						
						#define JS_ADD_ARRAY_VECTOR(v) \
							arrIdx = duk_push_array(js_gsc_duk_context); \
							duk_push_number(js_gsc_duk_context, v[0]); \
							duk_put_prop_index(js_gsc_duk_context, arrIdx, 0); \
							duk_push_number(js_gsc_duk_context, v[1]); \
							duk_put_prop_index(js_gsc_duk_context, arrIdx, 1); \
							duk_push_number(js_gsc_duk_context, v[2]); \
							duk_put_prop_index(js_gsc_duk_context, arrIdx, 2);
						float *vec = (float*)value.fp;
						//printf("%s(%f,%f,%f)\n", vec[0],vec[1],vec[2]);
						if(js_gsc_duk_context) {
							JS_ADD_ARRAY_VECTOR(value.fp);
						}
					} break;
					
				}
				if(js_gsc_duk_context) {
					++js_gsc_func_args;
					//printf("js_gsc_func_args = %d\n", js_gsc_func_args);
					return;
				}
				UnionValue (*o)() = (UnionValue(*))scr_funcs[i];
				return o();
			} else {
				//printf("%s(%i) (%s) => ", szFuncName, value.i, js_gsc_duk_context != NULL ? "JavaScript" : "GSC");
				//printf("js_gsc_duk_context = %02X\n", js_gsc_duk_context);
	
				UnionValue uv;
				switch(type) {
					
					case VT_STRING: {
						if(js_gsc_duk_context) {
							//printf("uv.s = duk_require_string(js_gsc_duk_context, %d);", value.i);
							uv.s = duk_require_string(js_gsc_duk_context, value.i);
							//printf("%s \n", uv.s);
						}
					} break;
					
					case VT_INT: {
						if(js_gsc_duk_context) {
							uv.i = duk_require_int(js_gsc_duk_context, value.i);
							//printf("%i ", uv.i);
						}
					} break;
					
					case VT_FLOAT: {
						if(js_gsc_duk_context) {
							uv.f = (float)duk_require_number(js_gsc_duk_context, value.i);
							//printf("%f ", uv.f);
						}
					} break;
					
					default: {
						if(js_gsc_duk_context)
							printf("UNSUPPORTED TYPE %s\n", Scr_GetVariableType(type));
					} break;
					
					case VT_VECTOR: {
						if(js_gsc_duk_context) {
							
							duk_get_prop_string(js_gsc_duk_context, value.i, "x");
							UnionVector[0] = (float)duk_to_number(js_gsc_duk_context,-1);
							duk_pop(js_gsc_duk_context);
							duk_get_prop_string(js_gsc_duk_context, value.i, "y");
							UnionVector[1] = (float)duk_to_number(js_gsc_duk_context,-1);
							duk_pop(js_gsc_duk_context);
							duk_get_prop_string(js_gsc_duk_context, value.i, "z");
							UnionVector[2] = (float)duk_to_number(js_gsc_duk_context,-1);
							duk_pop(js_gsc_duk_context);

							uv.fp = UnionVector;
							//printf("(%f, %f, %f) ", uv.fp[0],uv.fp[1],uv.fp[2]);
						}
					} break;
					
				}
				void *result ;
				if(js_gsc_duk_context) {
					js_gsc_func_args--;
					gsc_hook_caller = (unsigned)uv.p;
					//printf("| done\n");
					return;
				}
				void *(*o)(int) = (void*(*)(int))scr_funcs[i];
				result = o(value.i);
				//printf("| done\n");
				#if 0
				if(strstr(szFuncName, "GetString"))
					printf("String = %s\n", result);
				#endif
				gsc_hook_caller = (unsigned)result;
				return;
			}
		}
	}
	
	printf("should never happen\n");
	return 0;
	
	#if 0 /* works aswell but it reuses the game func */
	char cOrigBytes[6] = {0x55,0x89,0xe5,0x83,0xec};
	
	memcpy((void*)addr,&cOrigBytes[0],6);
	void (*o)() = (void(*))addr;
	o();
	__call_ret(addr, (unsigned)GetAddStuffAddr());
	int *numArgs = (int*)0x82F5944;
	*numArgs++;
	printf("str = %s, type = %s\n", Scr_GetString(0), Scr_GetVariableType(gsc_gettype(0)));
	*numArgs--;
	#endif
}

int _Scr_GetType(int a1) {
	if(js_gsc_duk_context)
		return duk_get_type(js_gsc_duk_context, a1);
	int (*o)(int) = (int(*)(int))0x80A9928;
	return o(a1);
}

int get_gsc_type_for_duk(int type) {
	switch(type) {
		case DUK_TYPE_NUMBER:
			return VT_FLOAT;
		case DUK_TYPE_STRING:
			return VT_STRING;
		case DUK_TYPE_UNDEFINED:
			return VT_UNDEFINED;
		default:
			return VT_INT;
	}
}

int _Scr_GetPointerType(int a1) {
	if(js_gsc_duk_context)
		return get_gsc_type_for_duk(duk_get_type(js_gsc_duk_context, a1));
	int (*o)(int) = (int(*)(int))0x80A8C1C;
	return o(a1);
}

int _Scr_GetNumParam() {
	extern int js_gsc_func_parms;
	if(js_gsc_duk_context)
		return js_gsc_func_parms;
	
	int (*o)() = (int(*)())0x80A99B8;
	return o();
}

void gsc_hook_to_js() {
	/* hook all scr_ value funcs */
	
	int *scr_funcs = (int*)0x8161200;
	for (int i = 0; i < (sizeof(SCR_FUNCS) / 4); i++)
		*(int*)((int)&Scr.GetBool + i * 4) = scr_funcs[i];
	bool f = false;
	
	for(int i = 0; scr_func_helper_names[i].name; i++) {
		if(scr_func_helper_names[i].type == VT_END_OF_LIST)
			continue;
		#if 0
		if(!f) {
			if(!strstr(scr_func_helper_names[i].name, "Add"))
				continue;
			f = true;
		}
		#endif
		__call_ret(GAME(va("Scr_%s", scr_func_helper_names[i].name)), (unsigned)GetAddStuffAddr());
	}
	
	/* 3 manual isnt that bad hehe */
	#if 0
	{"GetType",               VT_END_OF_LIST},
	{"GetPointerType",        VT_END_OF_LIST},
	{"GetNumParam",           VT_END_OF_LIST},
	#endif
	
	__jmp(GAME("Scr_GetType"), _Scr_GetType);
	__jmp(GAME("Scr_GetPointerType"), _Scr_GetPointerType);
	__jmp(GAME("Scr_GetNumParam"), _Scr_GetNumParam);
}
#endif
#include <netinet/in.h>

void scriptInitializing() {
	#ifdef BUILD_ECMASCRIPT
	void js_add_gsc_functions();
	js_add_gsc_functions();
	#endif
	
	memcpy((void*)&rct_bytes[0], (void*)GAME("Scr_RunCurrentThreads"), 5);
	__jmp(GAME("Scr_RunCurrentThreads"), (unsigned)_Scr_RunCurrentThreads);
	
	if(CSteamServer_Init(INADDR_ANY, 8766, 28960, 25011, "1.0.0.0"));
	/*
		printf("CSteamServer_Init worked\n");
	else
		printf("CSteamServer_Init failed!\n");
	*/
	
//	__jmp(GAME("Scr_GetGenericField"), _Scr_GetGenericField);
	
	SCRIPTFUNCTION *it = (SCRIPTFUNCTION*)GAME("functions");
	//printf("Patched developer functions:\n");
	//FILE *log_file = fopen("cod1_1_functiondump.txt", "w");
	for(int i = 0; i != 0x69; i++, it++) {
		//fprintf(log_file, "%s, ", it->name);
		
		if(!it->developer)
			continue;
		it->developer = 0;
	}
	//fclose(log_file);
	//printf("\n");
	
	#ifdef xDEBUG2
		__call(GAME("PmoveSingle") + 0x455, _PM_Weapon);
		__call(GAME("PmoveSingle") + 0x535, _PM_Weapon);
		
		spawn_t *sp = (spawn_t*)GAME("spawns");
		
		for(;sp->name!=NULL;sp++) {
			if(!strcmp(sp->name, "script_brushmodel")) {
				sp->spawn = _SP_script_brushmodel;
				printf("Patched script_brushmodel...\n");
			}
		}
		
		//__jmp(GAME("trap_XAnimGetAnimName"), xanim_getname);
		
		//__nop( GAME("BG_FinalizePlayerAnims")+0xe1, 13);
		//*(unsigned char*)( GAME("BG_FinalizePlayerAnims")+0xD8 ) = 0xeb;
		Cmd_AddCommand("db", __db);
		/*animation_t *anim ;
		int tmp ;
		for(int i = 0; i < 255; i++) {
			tmp = ( GAME("bgs") + 92 * i);
			anim = (animation_t*)&tmp;
			printf("{%s, %d, %d, %d, %d, %d, %d, %d}\n", anim->name, anim->unknown, anim->firstFrame,
			anim->unknown2, anim->unknown3, anim->unknown4, anim->flags, anim->unknown5);
		}*/
		
	#endif
	#if 0
	__jmp(GAME("fire_rocket"), _fire_rocket); //worked neatly but not sure after I changed everything to gentity_t offset structures due to naming.
	
	__jmp(GAME("fire_grenade"), _fire_grenade);
	#endif
	
	__jmp(GAME("Scr_StartupGameType"), (unsigned)_Scr_StartupGameType);
	__jmp(GAME("Scr_LoadGameType"), (unsigned)_Scr_LoadGameType);
	__jmp(GAME("Scr_PlayerConnect"), (unsigned)_Scr_PlayerConnect);
	__jmp(GAME("Scr_PlayerDisconnect"), (unsigned)_Scr_PlayerDisconnect);
	__jmp(GAME("Scr_PlayerDamage"), (unsigned)_Scr_PlayerDamage);
	__call(GAME("ClientCommand")+0x140, (unsigned)_Cmd_MenuResponse_f);
	
	__jmp((int)GAME("GScr_LoadGameTypeScript"), (int)GScr_LoadGametypeScript);
	
	#if 0
		__call(GAME("GScr_LoadScripts")+0xab, _GScr_AddFieldsForEntity);
		
		//080AA35A                 call    sub_809A090 (Scr_GetObjectField)
		__call(0x80AA35A, _Scr_GetObjectField);
		//.text:080AA2E8                 call    sub_809A070
		__call(0x80AA2E8, _Scr_SetObjectField);
	#endif
	
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
	
	g_scr_data = (game_script_data*)dlsym(gamelib, "g_scr_data");
	
	scr_const = (scr_const_t*)GAME("scr_const");
	
	g_clients = (unsigned char*)GAME("g_clients");
	hudelems = (unsigned char*)GAME("g_hudelems");
	__jmp(GAME("G_LocalizedStringIndex"), (int)X_LocalizedStringIndex);
	
	bg_itemlist = (gitem_t*)GAME("bg_itemlist");
	
	CallSpawnEntity = (CallSpawnEntity_t)GAME("G_CallSpawnEntity");
	
	SL_GetString = (SL_GetString_t)GAME("SL_GetString");
	
	Scr_GetFunction = (Scr_GetFunction_t)GAME("Scr_GetFunction");
	Scr_GetMethod = (Scr_GetMethod_t)GAME("Scr_GetMethod");
	Scr_GetString = (Scr_GetString_t)dlsym(gamelib, "Scr_GetString");
	Scr_GetConstString = (Scr_GetConstString_t)GAME("Scr_GetConstString");
	Scr_SetString = (Scr_SetString_t)GAME("Scr_SetString");
	Scr_AllocString = (Scr_AllocString_t)GAME("Scr_AllocString");
	
	oBG_AnimationIndexForString = (oBG_AnimationIndexForString_t)GAME("BG_AnimationIndexForString");
	
	/*printf("gamelib: %.2x\n", (int)gamelib);
	printf("base: %.2x\n", (int)base);
	printf("Scr_GetString in memory: %.2x\n", (int)Scr_GetString);
	printf("Scr_GetString without base: %.2x\n", ((int)Scr_GetString - (int)gamelib));*/
	Scr_GetInt = (Scr_GetInt_t)dlsym(gamelib, "Scr_GetInt");
	//Scr_GetAnim = (Scr_GetAnim_t)dlsym(gamelib, "Scr_GetAnim");
	Scr_GetAnimsIndex = (Scr_GetAnimsIndex_t)dlsym(gamelib, "Scr_GetAnimsIndex");
	Scr_GetFloat = (Scr_GetFloat_t)dlsym(gamelib, "Scr_GetFloat");
	Scr_GetVector = (Scr_GetVector_t)dlsym(gamelib, "Scr_GetVector");
	Scr_GetNumParam = (Scr_GetNumParam_t)dlsym(gamelib, "Scr_GetNumParam");
	Scr_GetBool = (Scr_GetBool_t)dlsym(gamelib, "Scr_GetBool");
	Scr_GetFunc = (Scr_GetFunc_t)dlsym(gamelib, "Scr_GetFunc");
	Scr_GetOffset = (Scr_GetOffset_t)GAME("Scr_GetOffset");
	Scr_GetType = (Scr_GetType_t)dlsym(gamelib, "Scr_GetType");
	Scr_GetPointerType = (Scr_GetPointerType_t)dlsym(gamelib, "Scr_GetPointerType");
	Scr_GetEntity = (Scr_GetEntity_t)GAME("Scr_GetEntity");
	#ifdef BUILD_ECMASCRIPT
	gsc_hook_to_js();
	#endif
	
	Scr_AddInt = (Scr_AddInt_t)dlsym(gamelib, "Scr_AddInt");
	Scr_AddFloat = (Scr_AddFloat_t)dlsym(gamelib, "Scr_AddFloat");
	Scr_AddVector = (Scr_AddVector_t)dlsym(gamelib, "Scr_AddVector");
	Scr_AddString = (Scr_AddString_t)dlsym(gamelib, "Scr_AddString");
	Scr_AddIString = (Scr_AddIString_t)dlsym(gamelib, "Scr_AddIString");
	Scr_AddEntity = (Scr_AddEntity_t)dlsym(gamelib, "Scr_AddEntity");
	Scr_AddUndefined = (Scr_AddUndefined_t)dlsym(gamelib, "Scr_AddUndefined");
	Scr_AddBool = (Scr_AddBool_t)dlsym(gamelib, "Scr_AddBool");
	
	Scr_MakeArray = (Scr_MakeArray_t)dlsym(gamelib, "Scr_MakeArray");
	Scr_AddArray = (Scr_AddArray_t)dlsym(gamelib, "Scr_AddArray");
	Scr_AddArrayStringIndexed = (Scr_AddArrayStringIndexed_t)dlsym(gamelib, "Scr_AddArrayStringIndexed");
	Scr_Error = (Scr_Error_t)dlsym(gamelib, "Scr_Error");
	
	SL_ConvertToString = (SL_ConvertToString_t)GAME("SL_ConvertToString");
	Scr_GetFunctionHandle = (Scr_GetFunctionHandle_t)GAME("Scr_GetFunctionHandle");
	Scr_LoadScript = (Scr_LoadScr_t)GAME("Scr_LoadScript");
}
