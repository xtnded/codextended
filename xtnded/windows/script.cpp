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
int callbackPlayerDamage;

loadCallback_t loadCallback = (loadCallback_t)0x472AB0;
dword_20031760_t dword_20031760 = (dword_20031760_t)0x20031760;
Scr_ExecThread_t Scr_ExecThread = (Scr_ExecThread_t)0x481E10;
Scr_ExecEntThread_t Scr_ExecEntThread = (Scr_ExecEntThread_t)0x481EC0;
Scr_FreeThread_t Scr_FreeThread = (Scr_FreeThread_t)0x482070;

int Scr_GetNumParam() {
    int addr = 0x483060;
    if(CODPATCH == 1)
        addr = 0x47DB30;
    return call<size_t>(addr);
}

int Scr_GetPointerType(int a1) {
    int addr = 0x482FC0;
    if(CODPATCH == 1)
        addr = 0x47DA90;
    return call<int, int>(addr, a1);
}

int Scr_GetType(int a1) {
    int addr = 0x482F50;
    if(CODPATCH == 1)
        addr = 0x47DA20;
    return call<int, int>(addr, a1);
}

int Scr_AddInt(int a1) {
    int addr = 0x483090;
    if(CODPATCH == 1)
        addr = 0x47DB60;
    return call<int, int>(addr, a1);
}

int Scr_AddBool(BOOL a1) {
    int addr = 0x483070;
    if(CODPATCH == 1)
        addr = 0x47DB40;
    return call<int, BOOL>(addr, a1);
}

int Scr_AddString(char* a1) {
    int addr = 0x483210;
    if(CODPATCH == 1)
        addr = 0x47DCE0;
    return call<int, char*>(addr, a1);
}

int Scr_AddVector(vec3_t a1) {
    int addr = 0x4832E0;
    if(CODPATCH == 1)
        addr = 0x47DDB0;
    return call<int, float*>(addr, a1);
}

BOOL Scr_GetBool(int index) {
    int addr = 0x482580;
    if(CODPATCH == 1)
        addr = 0x47D050;
    return call<BOOL, int>(addr, index);
}

int Scr_GetInt(int index) {
    int addr = 0x482630;
    if(CODPATCH == 1)
        addr = 0x47D100;
    return call<int, int>(addr, index);
}

float Scr_GetFloat(int index) {
    int addr = 0x482920;
    if(CODPATCH == 1)
        addr = 0x47D3F0;
    return call<float, int>(addr, index);
}

int Scr_GetVector(int index, vec3_t vec) {
    int addr = 0x482CC0;
    if(CODPATCH == 1)
        addr = 0x47D790;
    return call<int, int, float*>(addr, index, vec);
}

char* Scr_GetString(int index) {
    int addr = 0x482A60;
    if(CODPATCH == 1)
        addr = 0x47D530;
    return call<char*, int>(addr, index);
}

int Scr_GetFunc(int index) {
    return call<int, int>(0x482D90, index);
}

int Scr_AddUndefined() {
    int addr = 0x4830F0;
    if(CODPATCH == 1)
        addr = 0x47DBC0;
    return call<int>(addr);
}

Scr_FunctionCall Scr_GetFunction(const char** name, int* developer) {
    int addr = 0x20037A00;
    if(CODPATCH == 1)
        addr = 0x20034C00;
    return call<Scr_FunctionCall, const char**, int*>(addr, name, developer);
}

Scr_FunctionCall Scr_GetMethod(const char** name, int* developer) {
    int addr = 0x20037B00;
    if(CODPATCH == 1)
        addr = 0x20034D00;
    return call<Scr_FunctionCall, const char**, int*>(addr, name, developer);
}


Scr_FunctionCall Scr_GetCustomMethod(const char** fname, int* fdev) {
    Scr_FunctionCall m = Scr_GetMethod(fname, fdev);
    void (*fc)(int);
    *(int*)&fc = (int)m;
    if(m == NULL) {
        if(scriptMethods.find(std::string(*fname)) != scriptMethods.end()){
            Com_Printf("found: %s\n", *fname);
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

void writeScripts() {
    int* base = (int*)0x2006D770;
    FILE *f = fopen("C:/COD/scripts.txt", "w");
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

void writePlayerMethods() {
    FILE *f = fopen("C:/COD/player_methodscripts.txt", "w");
    if(f != 0) {
        for(int i = 0; i < 55; i++) {
            char** name = (char**)(0x20058A98 + 8 * i);
            int** func = (int**)(0x20058A9C + 4 * i);
            fprintf(f, "%s, %.2x\n", *name, *func);
        }
        fprintf(f, "\n//end of player methods\n\n");
        for(int i = 0; i < 45; i++) {
            char** name = (char**)(0x2006DC98+8*i);
            int** func = (int**)(0x2006DC98+4*i);
            fprintf(f, "%s, %.2x\n", *name, *func);
        }
        fclose(f);
    }
}

void patchDeveloperScripts() {
    int* base = (int*)0x2006D770;
    for(int i = 0; i < 110; i++) {
        int* b = (int*)base + i * 3;
        char** name = (char**)b;
        int** func = (int**)b+1;
        int** dev = (int**)b+2;
        if(*(int*)dev == 1) {
            *dev = 0;
            Com_Printf("Patched %s\n", *name);
        }
        //Com_Printf("[%s:%i]", *name, *dev);
    }
    //Com_Printf("\n");
}

int GScr_LoadGameTypeScript() {
    int g = dword_20031760();
    callbackTest = loadCallback("maps/mp/gametypes/_callbacksetup", "CodeCallback_Test");
    //callbackTest = loadCallback("CodeCallback_Test", "maps/mp/gametypes/_callbacksetup");
    callbackPlayerDamage = *(int*)0x202D89D8;
    //Com_Printf("callbackTest loaded: %i\n", callbackTest);
    //Com_Printf("callbackPlayerDamage: %i\n", callbackPlayerDamage);
    //patchDeveloperScripts();

    /*DWORD old;
    VirtualProtect((void*)0x46F191, 6, PAGE_EXECUTE_READWRITE, &old);
    BYTE ff[6] = {0xFF, 0x15, 0x94, 0xED, 0x93, 0x0};
    for(short i = 0; i < 6; i++)
    *(BYTE*)(0x46F191+i) = ff[i];
    VirtualProtect((void*)0x46F191, 6, old, &old);*/
    return g;
}

void patchScriptCall() {
    int loc = 0x46F191;
    if(CODPATCH == 1)
        loc = 0x469BEE;
    int len = 6;
    DWORD old;
    VirtualProtect((void*)loc, len, PAGE_EXECUTE_READWRITE, &old);
    *(BYTE*)loc = 0xe8;
    //*(DWORD*)0x46F192 = (int)Scr_GetCustomFunction;
    cracking_hook_call(loc, (int)Scr_GetCustomFunction);
    *(BYTE*)(loc+5) = 0x90;
    VirtualProtect((void*)loc, len, old, &old);

    loc = 0x46F454;
    if(CODPATCH == 1)
        loc = 0x469E8E;
    VirtualProtect((void*)loc, len, PAGE_EXECUTE_READWRITE, &old);
    *(BYTE*)loc = 0xe8;
    cracking_hook_call(loc, (int)Scr_GetCustomMethod);
    *(BYTE*)(loc+5) = 0x90;
    VirtualProtect((void*)loc, len, old, &old);
}


//ERGH late 3:47 AM going to sleep
//i'll do <methods> tommorow

void Cmd_isMySQLSupported() {
    #ifdef uMYSQL
    Com_Printf("MySQL is supported! %d\n", MYSQL_VERSION_ID);
    #else
    Com_Printf("MySQL isn't supported.\n");
    #endif
}

void InitScriptFunctions() {

    /*
        Usage;
        Scr_AddFunction(<script function name>, <void which gets called>, <developer>, <isMethod>);
    */

    Scr_AddFunction("love", GScr_default, 0, false);
    Scr_AddFunction("printconsole", GScr_console_print, 0, false);

    Scr_AddFunction("getplayerangles", GScr_getPlayerAngles, 0, true);
    Scr_AddFunction("getvelocity", GScr_getVelocity, 0, true);
    Scr_AddFunction("setvelocity", GScr_setVelocity, 0, true);
    //Scr_AddFunction("jumpbuttonpressed", GScr_jumpButtonPressed, 0, true);*/
    Scr_AddFunction("num", GScr_default, 0, true);
    Scr_AddFunction("getping", GScr_default, 0, true);
    Scr_AddFunction("getaim", GScr_default, 0, true);
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

    Scr_AddFunction("setbounds", GScr_default, 0, true);
    Scr_AddFunction("settakedamage", ScrCmd_setTakeDamage, 0, true);
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
