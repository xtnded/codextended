/*
===========================================================================
    Copyright (C) riicchhaarrd
    Copyright (C) 1999-2005 Id Software, Inc.

    This file is part of CoDExtended source code.

    CoDExtended source code is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    CoDExtended source code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
===========================================================================
*/

#include "script.hpp"

int callbackTest;
int callbackPlayerCommand;
int callbackEntityDamage;
int callbackEntityThink;
int callbackEntityUse;
int callbackEntityKilled;
int callbackEntityTouch;
int callbackfunctionmain;

BOOL scr_return = false;

loadCallback_t loadCallback = (loadCallback_t)0x472AB0;
dword_20031760_t dword_20031760 = (dword_20031760_t)0x20031760;
Scr_ExecThread_t Scr_ExecThread = (Scr_ExecThread_t)0x481E10;
Scr_ExecEntThread_t Scr_ExecEntThread = (Scr_ExecEntThread_t)0x481EC0;
Scr_FreeThread_t Scr_FreeThread = (Scr_FreeThread_t)0x482070;

SL_ConvertToString_t SL_ConvertToString = (SL_ConvertToString_t)0x474EE0;

Scr_MakeArray_t Scr_MakeArray = (Scr_MakeArray_t)0x483330;
Scr_AddArray_t Scr_AddArray = (Scr_AddArray_t)0x483380;
Scr_Error_t Scr_Error = (Scr_Error_t)0x483470;

Scr_GetNumParam_t Scr_GetNumParam = (Scr_GetNumParam_t)0x483060;
Scr_GetPointerType_t Scr_GetPointerType = (Scr_GetPointerType_t)0x482FC0;
Scr_GetType_t Scr_GetType = (Scr_GetType_t)0x482F50;
Scr_AddInt_t Scr_AddInt = (Scr_AddInt_t)0x483090;
Scr_AddFloat_t Scr_AddFloat = (Scr_AddFloat_t)0x4830B0;
Scr_AddBool_t Scr_AddBool = (Scr_AddBool_t)0x483070;
Scr_AddString_t Scr_AddString = (Scr_AddString_t)0x483210;
Scr_AddVector_t Scr_AddVector = (Scr_AddVector_t)0x4832E0;
Scr_AddUndefined_t Scr_AddUndefined = (Scr_AddUndefined_t)0x4830F0;
Scr_AddEntity_t Scr_AddEntity = (Scr_AddEntity_t)0x483140;

Scr_GetBool_t Scr_GetBool = (Scr_AddBool_t)0x482580;
Scr_GetInt_t Scr_GetInt = (Scr_GetInt_t)0x482630;
Scr_GetFloat_t Scr_GetFloat = (Scr_GetFloat_t)0x482920;
Scr_GetVector_t Scr_GetVector = (Scr_GetVector_t)0x482CC0;
Scr_GetString_t Scr_GetString = (Scr_GetString_t)0x482A60;
Scr_GetFunc_t Scr_GetFunc = (Scr_GetFunc_t)0x482D90;
Scr_GetFunction_t Scr_GetFunction = (Scr_GetFunction_t)0x20037A00;
Scr_GetMethod_t Scr_GetMethod = (Scr_GetMethod_t)0x20037B00;


Scr_FunctionCall Scr_GetCustomMethod(const char** fname, int* fdev) {
    Scr_FunctionCall m = Scr_GetMethod(fname, fdev);
    void (*fc)(int);
    *(int*)&fc = (int)m;
    if(m == NULL) {
        if(scriptMethods.find(std::string(*fname)) != scriptMethods.end()){
            Scr_Function func = scriptMethods[*fname];
            *fname = func.name;
            *fdev = func.developer;
            return func.call;
        }
    }
    return fc;
}

Scr_FunctionCall Scr_GetCustomFunction(const char** fname, int* fdev) {
    /*int* base = (int*)0x2006D770;
    int found = 0;
    void (*fc)(int);

    for(int i = 0; i < 110; i++) {
        int* b = (int*)base + i * 3;
        char** name = (char**)b;
        int** func = (int**)b+1;
        int** dev = (int**)b+2;
        if(strcmp(*name, *fname) == 0) {
            found = 1;
            //Com_Printf("ye: %s, %.2x\n", *name, (int)(*func));
            *(int*)&fc = (int)(*func);
        }
    }*/
    Scr_FunctionCall m = Scr_GetFunction(fname, fdev);
    void (*fc)(int);
    *(int*)&fc = (int)m;
    if(m == NULL) {
        if(scriptFunctions.find(std::string(*fname)) != scriptFunctions.end()){
            Scr_Function func = scriptFunctions[*fname];
            *fname = func.name;
            *fdev = func.developer;
            return func.call;
        }
    }
	return fc;
}


void Scr_AddFunction(const char* n, Scr_FunctionCall func, int dev, BOOL method) {
	Scr_Function f;
	f.name = n;
	f.call = func;
	f.developer = dev;
	if(!method)
	scriptFunctions[n] = f;
	else
	scriptMethods[n] = f;
}

void dumpScriptMethods() {
    /*
        HudElem and ScriptEnt are not included, but they should be a easy find.
    */

    if(Cmd_Argc() < 2) {
        Com_Printf("Usage: dumpmethods <output file location (C:/dump.txt)>\n");
        return;
    }

    const char* out = Cmd_Argv(0);

    FILE *f = fopen(out, "w");
    if(f != 0) {
        for(int i = 0; i < 55; i++) {
            char** name = (char**)(0x20058A98 + 8 * i);
            int** func = (int**)(0x20058A9C + 4 * i);
            fprintf(f, "%s, %.2x\n", *name, *func);
        }
        fprintf(f, "\n//end of player methods\n\n");
        //entity methods
        for(int i = 0; i < 45; i++) {
            char** name = (char**)(0x2006DC98+8*i);
            int** func = (int**)(0x2006DC98+4*i);
            fprintf(f, "%s, %.2x\n", *name, *func);
        }
        fclose(f);
    }
}

void dumpScriptFunctions() {
    if(Cmd_Argc() < 2) {
        Com_Printf("Usage: dumpfunctions <output file location (C:/dump.txt)>\n");
        return;
    }

    const char* out = Cmd_Argv(0);

    int* base = (int*)0x2006D770;
    FILE *f = fopen(out, "w");
    if(f != 0) {
        for(int i = 0; i < 110; i++) {
            int* b = (int*)base + i * 3;
            char** name = (char**)b;
            int** func = (int**)b+1;
            int** dev = (int**)b+2;
            fprintf(f, "%s, %.2x, %d\n", *name, *func, *dev);
        }
        fclose(f);
    }
}

int Scr_LoadFunction(const char* file, const char* functionname) {
    if(!call<int, const char*>(0x472DC0, file))
        Com_Printf("Could not find script '%s'\n", file);

    int v4 = call<int, const char*, const char*>(0x472AB0, file, functionname);
    if(!v4)
        Com_Printf("Could not find label '%s' in script '%s'\n", functionname, file);
    printf("callback[%s] = %x\n", functionname, v4);
    return v4;
}

void loadScript() {
    if(Cmd_Argc() < 3) {
        Com_Printf("Usage: loadscript <file> <function>\n");
        return;
    }

    const char* file = Cmd_Argv(0);
    const char* func = Cmd_Argv(1);

    int result = Scr_LoadFunction(file, func);
    Com_Printf("result: %d\n", result);
}

void callScript() {
    if(Cmd_Argc() < 2) {
        Com_Printf("Usage: callscript <handle>\n");
        return;
    }

    int handle = atoi(Cmd_Argv(0));
    int result = Scr_ExecThread(handle, 0);
    Scr_FreeThread(result);
}

void GameDamageMidHook() {
    int damage;
    int target;
    asm("movl %%ecx, %0" : "=m" (target));
    Com_Printf("target: %d\n", target);
    asm("movl %0, %%eax" :: "r" (0x20022B48));
    asm("jmp %eax");
}

int GScr_LoadGameTypeScript() {
    int g = dword_20031760();
    /*char path[256];
    char* gametype = (char*)0x20241330;
    sprintf(path, "maps/mp/gametypes/%s", gametype);
    *(DWORD*)0x202D89C8 = Scr_LoadFunction(path, "main");
    *(DWORD*)0x202D89CC = Scr_LoadFunction("maps/mp/gametypes/_callbacksetup", "CodeCallback_StartGameType");
    *(DWORD*)0x202D89D0 = Scr_LoadFunction("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerConnect");
    *(DWORD*)0x202D89D4 = Scr_LoadFunction("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerDisconnect");
    *(DWORD*)0x202D89D8 = Scr_LoadFunction("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerDamage");
    *(DWORD*)0x202D89DC = Scr_LoadFunction("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerKilled");
    */
    callbackfunctionmain = Scr_LoadFunction("callback", "main");

    //int result = Scr_ExecThread(callbackfunctionmain, 0);
    //Scr_FreeThread(result);

    callbackPlayerCommand = Scr_LoadFunction("callback", "CodeCallback_PlayerCommand");
    callbackEntityDamage = Scr_LoadFunction("callback", "CodeCallback_EntityDamage");
    callbackEntityKilled = Scr_LoadFunction("callback", "CodeCallback_EntityKilled");
    callbackEntityUse = Scr_LoadFunction("callback", "CodeCallback_EntityUse");
    callbackEntityThink = Scr_LoadFunction("callback", "CodeCallback_EntityThink");
    callbackEntityTouch = Scr_LoadFunction("callback", "CodeCallback_EntityTouch");
    /*callbackTest = Scr_LoadFunction("callback", "test");
    Com_Printf("callbackTest: %d\n", callbackTest);
    Cmd_AddCommand("loadscript", loadScript);
    Cmd_AddCommand("callscript", callScript);*/

    /*for(size_t i = 0; i < MAX_ENTITY_SIZE; i++) {
        Entity *ent = game->getEntity(i);
        if(ent) {
            ent->nextthink = 0;
            ent->pain = 0;
            ent->die = 0;
            ent->think = 0;
            ent->use = 0;
            ent->touch = 0;
        }
    }*/

    //cracking_hook_function(0x20022B27, (int)GameDamageMidHook);
    return g;
}

void Scr_LoadGameType() {
    int result;
    result = Scr_ExecThread(*(DWORD*)0x202D89C8, 0);
    Scr_FreeThread(result);
}

void GScr_Trace(int a1) {
    trace_t trace;
    vec3_t start, endl, mins, maxs;
    int ignore = Scr_GetInt(4);
    int contentmask = Scr_GetInt(5);
    Scr_GetVector(0, start);
    Scr_GetVector(1, endl);
    Scr_GetVector(2, mins);
    Scr_GetVector(3, maxs);
    //Com_Printf("trap_Trace called; start: (%f %f %f) end: (%f %f %f)\n", start[0], start[1], start[2], endl[0], endl[1], endl[2]);
    syscall(TRAP_TRACE, &trace, start, mins, maxs, endl, ignore, contentmask);
    //Com_Printf("finished trap_trace\n");
    //Com_Printf("::output %f %f %f\n", trace.endpos[0], trace.endpos[1], trace.endpos[2]);
    Scr_MakeArray();
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
    Scr_AddVector(trace.endpos); Scr_AddArray();
    Scr_AddInt(trace.surfaceFlags); Scr_AddArray();
    Scr_AddInt(trace.entityNum); Scr_AddArray();
    Scr_AddFloat(trace.fraction); Scr_AddArray();
    Scr_AddBool(trace.startsolid); Scr_AddArray();
    Scr_AddBool(trace.allsolid); Scr_AddArray();
    Scr_AddVector(trace.plane.normal); Scr_AddArray();
    Scr_AddInt(trace.contents); Scr_AddArray();
}

void GScr_parsefunc(int a1) {
    int handle = Scr_GetFunc(0);
    Com_Printf("parsefunc: %d\n", handle);
    int result = Scr_ExecThread(handle, 0);
    Scr_FreeThread(result);
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
    if(idx > trap_Argc()) {
        Scr_AddString("");
        return;
    }
    char buf[MAX_STRING_CHARS];
    trap_Argv(idx, buf, sizeof(buf));
    Scr_AddString(buf);
}

void GScr_setAirAccelerate(int a1) {
    float a = Scr_GetFloat(0);
    int addr = 0x20058DBC;
    DWORD tmp;
    VirtualProtect((void*)addr, sizeof(float), PAGE_EXECUTE_READWRITE, &tmp);
    *(float*)addr = a;
	VirtualProtect((void*)addr, sizeof(float), tmp, &tmp);
}

void GScr_return(int a1) {
    scr_return = true;
}

BOOL Scr_Continue() {
    BOOL ret = scr_return;
    scr_return = false;
    return !ret;
}

//ERGH late 3:47 AM going to sleep
//i'll do <methods> tommorow

void Cmd_isMySQLSupported() {
    #ifdef uMYSQL
    Com_Printf("MySQL is supported! %s\n", mysql_get_client_info());
    #else
    Com_Printf("MySQL isn't supported.\n");
    #endif
}

void InitScript() {
    int loc = 0x46F191;
    int len = 6;
    DWORD old;
    VirtualProtect((void*)loc, len, PAGE_EXECUTE_READWRITE, &old);
    *(BYTE*)loc = 0xe8;
    //*(DWORD*)0x46F192 = (int)Scr_GetCustomFunction;
    cracking_hook_call(loc, (int)Scr_GetCustomFunction);
    *(BYTE*)(loc+5) = 0x90;
    VirtualProtect((void*)loc, len, old, &old);

    loc = 0x46F454;
    VirtualProtect((void*)loc, len, PAGE_EXECUTE_READWRITE, &old);
    *(BYTE*)loc = 0xe8;
    cracking_hook_call(loc, (int)Scr_GetCustomMethod);
    *(BYTE*)(loc+5) = 0x90;
    VirtualProtect((void*)loc, len, old, &old);


    Cmd_AddCommand("dumpmethods", dumpScriptMethods);
    Cmd_AddCommand("dumpfunctions", dumpScriptFunctions);

    /*
        Usage;
        Scr_AddFunction(<script function name>, <void which gets called>, <developer>, <isMethod>);
    */
    Scr_AddFunction("parsefunc", GScr_parsefunc, 0, false);

    Scr_AddFunction("creturn", GScr_return, 0, false);
    Scr_AddFunction("trace", GScr_Trace, 0, false);

    Scr_AddFunction("cmd_argv", GScr_Cmd_Argv, 0, false);
    Scr_AddFunction("trap_argv", GScr_trap_Argv, 0, false);
    Scr_AddFunction("cmd_argc", GScr_Cmd_Argc, 0, false);

    Scr_AddFunction("love", GScr_default, 0, false);
    Scr_AddFunction("printconsole", GScr_console_print, 0, false);

    Scr_AddFunction("vectorscale", GScr_VectorScale, 0, false);

    Scr_AddFunction("setairaccelerate", GScr_setAirAccelerate, 0, false);

    Scr_AddFunction("backbuttonpressed", GScr_backButtonPressed, 0, true);
    Scr_AddFunction("forwardbuttonpressed", GScr_forwardButtonPressed, 0, true);
    Scr_AddFunction("leftbuttonpressed", GScr_leftButtonPressed, 0, true);
    Scr_AddFunction("rightbuttonpressed", GScr_rightButtonPressed, 0, true);
    Scr_AddFunction("moveupbuttonpressed", GScr_moveupButtonPressed, 0, true);
    Scr_AddFunction("movedownbuttonpressed", GScr_movedownButtonPressed, 0, true);
    Scr_AddFunction("aimbuttonpressed", GScr_aimButtonPressed, 0, true);
    Scr_AddFunction("reloadbuttonpressed", GScr_reloadButtonPressed, 0, true);
    Scr_AddFunction("getip", GScr_getIP, 0, true);
    Scr_AddFunction("getplayerangles", GScr_getPlayerAngles, 0, true);
    Scr_AddFunction("getvelocity", GScr_getVelocity, 0, true);
    Scr_AddFunction("setvelocity", GScr_setVelocity, 0, true);
    //Scr_AddFunction("jumpbuttonpressed", GScr_jumpButtonPressed, 0, true);*/
    Scr_AddFunction("num", GScr_default, 0, true);
    Scr_AddFunction("getping", GScr_default, 0, true);
    Scr_AddFunction("dropclient", GScr_default, 0, true);

    Scr_AddFunction("ismysqlsupported", GScr_isMySQLSupported, 0, false);
    Cmd_AddCommand("mysql", Cmd_isMySQLSupported);
    #ifdef uMYSQL
    Scr_AddFunction("mysql_init", GScr_mysql_init, 0, false);
    Scr_AddFunction("mysql_free_result", GScr_mysql_free_result, 0, false);
    Scr_AddFunction("mysql_store_result", GScr_mysql_store_result, 0, false);
    Scr_AddFunction("mysql_real_escape_string", GScr_mysql_real_escape_string, 0, false);
    Scr_AddFunction("mysql_num_fields", GScr_mysql_num_fields, 0, false);
    Scr_AddFunction("mysql_num_rows", GScr_mysql_num_rows, 0, false);
    Scr_AddFunction("mysql_affected_rows", GScr_mysql_affected_rows, 0, false);
    Scr_AddFunction("mysql_fetch_field", GScr_mysql_fetch_field, 0, false);
    Scr_AddFunction("mysql_error", GScr_mysql_error, 0, false);
    Scr_AddFunction("mysql_close", GScr_mysql_close, 0, false);
    Scr_AddFunction("mysql_errno", GScr_mysql_errno, 0, false);
    Scr_AddFunction("mysql_field_seek", GScr_mysql_field_seek, 0, false);
    Scr_AddFunction("mysql_query", GScr_mysql_query, 0, false);
    Scr_AddFunction("mysql_real_connect", GScr_mysql_real_connect, 0, false);
    #endif

    Scr_AddFunction("fwrite", GScr_fwrite, 0, false);
    Scr_AddFunction("fclose", GScr_fclose, 0, false);
    Scr_AddFunction("fread", GScr_fread, 0, false);
    Scr_AddFunction("fopen", GScr_fopen, 0, false);
    Scr_AddFunction("fsize", GScr_fsize, 0, false);
    Scr_AddFunction("fexists", GScr_fexists, 0, false);

    Scr_AddFunction("setbounds", ScrCmd_setBounds, 0, true);
    Scr_AddFunction("setsize", ScrCmd_setBounds, 0, true);
    Scr_AddFunction("settakedamage", ScrCmd_setTakeDamage, 0, true);
    Scr_AddFunction("nextthink", GScr_nextthink, 0, true);
    Scr_AddFunction("callback", GScr_callback, 0, true);
}

void GScr_default(int entityIndex) {
    Com_Printf("GScr_default called %d\n", entityIndex);
    Scr_AddUndefined();
}

void GScr_console_print(int entityIndex) {
    char* str = Scr_GetString(0);
    Com_Printf(str);
    Scr_AddUndefined();
}
