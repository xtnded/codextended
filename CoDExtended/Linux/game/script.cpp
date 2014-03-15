#include "script.hpp"
/*
	:: Scr_GetFunction/GetMethod ::
	
	CoD2 search for "parameter count exceeds 256" and go upwards
	CoD1 search for "parameter count exceeds 256" or "unknown (builtin) function '%s'" and go upwards
*/

SCRIPTFUNCTION scriptFunctions[] = {
	//name, function, developer
    {"printconsole", GScr_printconsole, 0},
    {"fopen", GScr_fopen, 0},
    {"fread", GScr_fread, 0},
    {"fclose", GScr_fclose, 0},
    {"fwrite", GScr_fwrite, 0},
    {"fexists", GScr_fexists, 0},
    {"sin", MScr_sin, 0},
    {"cos", MScr_cos, 0}
};

SCRIPTFUNCTION scriptMethods[] = {
	//name, function, developer
	
	/*
	======
	ENTITY
	======
	*/
	
	{"setbounds", EntCmd_setBounds, 0},/*
	{"settakedamage", EntCmd_setTakeDamage, 0},
	{"callback", EntCmd_callback, 0},
	{"nextthink", EntCmd_nextthink, 0},*/
	
	/*
	======
	PLAYER
	======
	*/
    {"usebuttonpressedx", PlayerCmd_useButtonPressedX, 0},
	{"setvelocity", PlayerCmd_setVelocity, 0},
	{"getvelocity", PlayerCmd_getVelocity, 0},
	{"getplayerangles", PlayerCmd_getPlayerAngles, 0},
	{"getip", PlayerCmd_getIP, 0},
	{"backbuttonpressed", PlayerCmd_backButtonPressed, 0},
	{"forwardbuttonpressed", PlayerCmd_forwardButtonPressed, 0},
	{"leftbuttonpressed", PlayerCmd_leftButtonPressed, 0},
	{"rightbuttonpressed", PlayerCmd_rightButtonPressed, 0},
	{"moveupbuttonpressed", PlayerCmd_moveupButtonPressed, 0},
	{"movedownbuttonpressed", PlayerCmd_movedownButtonPressed, 0},
	{"aimbuttonpressed", PlayerCmd_aimButtonPressed, 0},
	{"reloadbuttonpressed", PlayerCmd_reloadButtonPressed, 0}
};

/*
	Can't use Scr_GetMethod/Scr_GetFunction because those names already exists and aren't mangled.
*/

Script_GetFunction_t Script_GetFunction = (Script_GetFunction_t)0x80A0B34;
Script_GetMethod_t Script_GetMethod = (Script_GetMethod_t)0x80A0B50;

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

Script_GetBool_t Script_GetBool;
Script_GetInt_t Script_GetInt;
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

/*
=============
FUNCTIONS
=============
*/

void GScr_printconsole(int entityIndex) { //if this was a method the index would be the entity's number
	const char* txt = Script_GetString(0);
	printf(txt);
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
        char txt[len];
        fgets(txt, len, f);
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


void scriptInitializing() {
	Script_GetString = (Script_GetString_t)dlsym(gamelib, "Scr_GetString");
	/*printf("gamelib: %.2x\n", (int)gamelib);
	printf("base: %.2x\n", (int)base);
	printf("Scr_GetString in memory: %.2x\n", (int)Script_GetString);
	printf("Scr_GetString without base: %.2x\n", ((int)Script_GetString - (int)gamelib));*/
	Script_GetInt = (Script_GetInt_t)dlsym(gamelib, "Scr_GetInt");
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
}
