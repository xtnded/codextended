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

/*
    ::      MDLL     ::
        riicchhaarrd

    Yes my file organizing is a mess
    - Richard
*/

using namespace std;

int game_mp;

vector<string> downloadList;

DWORD WINAPI tExtend(LPVOID);
DWORD g_threadID;
HMODULE g_hModule;

void version();
int sub_2001E5B0(int);

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            g_hModule = hinstDLL;
            DisableThreadLibraryCalls(hinstDLL);
            CloseHandle(CreateThread(NULL, NULL, &tExtend, NULL, NULL, &g_threadID));
            break;

        case DLL_PROCESS_DETACH:
        case DLL_THREAD_DETACH:
            delete game;
            FreeLibraryAndExitThread(g_hModule, 0);
            break;
    }
    return true;
}

/*
    //TODO
        * Move original clientnumber into ECX register and call ClientCommand wherever I want
*/
//0x2002747C
//idea why not call vmMain ClientCommand??
void myClientCommand() {
    int num;
    asm ("movl %%ecx, %0"
    :"=m" (num)
    );
    ClientCommand();
    //int num = (int)sp;
    //int num = *((int*)0x0313FFCC+10);
    //Com_Printf("Client: %d\n", num);
    //memcpy(&cmd_cmd, (int*)0x00917388, MAX_STRING_TOKENS); deprecated
    //memcpy(&cmd,(int*)0x0917388,1024);
    //trap_Argv(0, &cmd, 1024);
    //Com_Printf("Client: cmd:%s\n", cmd);
        //strcpy(players[num].cmd, cmd_cmd);
    //memcpy(&players[num].cmd, (int*)0x00917388, 1024);
        //Com_Printf("players[%i].cmd = %s\n", num, players[num].cmd);
    char cmd[1024];
    trap_Argv(0, cmd, 1024);
    if(strcmp(cmd, "say") == 0) {
        /*char said[1024];
        trap_Argv(1, said, 1024);
        strcpy(players[num].chat, said);

        FILE *f = fopen("C:/COD/chatlog.txt", "a");
        if(f != NULL) {
            fprintf(f, "%d:", num);
            fprintf(f, said);
            fprintf(f, "\n");
            fclose(f);
        }*/
    }
    //Com_Printf("[php] num=%i argc=%d cmd0=%s\n", num, Cmd_Argc(), Cmd_Argv(0));

    //Com_Printf("[php] cmd=%s\n",cmd);
	//asm("mov var, %0" : "=r" (out));
	//int *ecx = (int*)(0x02497BD0 + 0x10);
	//asm("movl %0, %%ecx" : "=r" (ecx));
    //ClientCommand();
}

void _memset(void* addr, int v, size_t len) {
    DWORD tmp;
    VirtualProtect((void*)addr, len, PAGE_EXECUTE_READWRITE, &tmp);
    memset(addr, v, len);
    VirtualProtect((void*)addr, len, tmp, &tmp);
}

void _memcpy(void* v1, void* v2, size_t len) {
    DWORD tmp;
    VirtualProtect((void*)v1, len, PAGE_EXECUTE_READWRITE, &tmp);
    memcpy(v1, v2, len);
    VirtualProtect((void*)v1, len, tmp, &tmp);
}

void ScriptInitiating() {
    game_mp = (int)GetModuleHandle("game_mp_x86.dll");
    Com_Printf("[game_mp: %02x]\n", game_mp);
    if(CODPATCH == 1) {
        _memset((void*)0x20018D7A, 0x90, 5);
        //cracking_hook_call(0x20024FF3, (int)myClientCommand);
    } else if(CODPATCH == 5) {
        cracking_hook_call(0x20031AD8, (int)GScr_LoadGameTypeScript);
        cracking_hook_call(0x2002747C, (int)myClientCommand);
        _memset((void*)0x200198F0, 0x90, 5);
    }
}

void G_SayTo(/*gentity_t*/int* ent, /*gentity_t*/int* other, int mode, int color, const char* name, const char* message) {

}

void *Sys_LoadDll( const char *name, char *fqpath, int( **entryPoint ) ( int, ... ), int ( *systemcalls )( int, ... ) ) {
    int addr = 0x4686D0;
    if(CODPATCH == 1)
        addr = 0x4633A0;
    void *l = call<void*, const char*, char*, int(**)(int,...)>(addr, name, fqpath, entryPoint, systemcalls);
    ScriptInitiating();
    return l;
}

void SV_BeginDownload_f(int* cl) {
    if(CODPATCH == 1)
        call<void*, int*>(0x454560, cl);
}

void SV_BeginDownload(int* cl) {
    int argc = Cmd_Argc();

    if(argc > 1) {
        char** arg1 = (char**)0x890BF4;
        char* dlname = (char*)(cl+68196);
        call<void*, int*>(0x4543C0, cl); //abort
        //call<void*, int*>(0x454360, cl); //close download
        if(Q_stristr(*arg1, ".."))
            return;
        BOOL found = false;
        string filename = getFileInPath(*arg1);
        for(vector<string>::const_iterator it=downloadList.begin(); it != downloadList.end(); it++) {
            if(strcmp(it->c_str(), filename.c_str()) == 0)
                found = true;
        }
        if(!found) {
            Com_Printf("WARNING: Someone is trying to download %s\n", *arg1);
            return;
        }
        //strcpy(*arg1, "main/php.exe");
    }
    asm ("movl %0, 8(%%esp)"::"r"(cl));
    SV_BeginDownload_f(cl);
}

void patchDownloading() {
    if(CODPATCH == 5)
        return;
    *(DWORD*)0x575B1C = (DWORD)SV_BeginDownload;
    string fs_game = string(Cvar_VariableString("fs_game"));
    string fs_basepath = string(Cvar_VariableString("fs_basepath"));
    if(fs_game.empty())
        fs_game = "main";
    string dir;
    dir += fs_basepath;
    dir.append("/");
    dir += fs_game;
    Com_Printf("Directory: %s\n", dir.c_str());

    vector<string> files;

    GetFilesInDirectory(files, dir);
    for(vector<string>::const_iterator it=files.begin(); it != files.end(); it++) {
        string s = getFileInPath(*it);
        if(strstr(s.c_str(), ".pk3") != NULL && !strstr(s.c_str(), "svr") && !strstr(s.c_str(), "pak")) {
            downloadList.push_back(s);
            Com_Printf("File added to download queue: %s\n", s.c_str());
        }
    }
}

/*
btDiscreteDynamicsWorld* dynamicsWorld;
btSequentialImpulseConstraintSolver* solver;
btCollisionDispatcher* dispatcher;
btDefaultCollisionConfiguration* collisionConfiguration;
btBroadphaseInterface* broadphase;
*/
DWORD WINAPI tExtend(LPVOID) {
    /*broadphase = new btDbvtBroadphase();

    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,-10,0));
    */
    /*
        For Bullet Physics
        // TODO have to dynamic load CODBSP
        // Too lazy
    */

    FILE *f = fopen("C:/COD/chatlog.txt", "w");
    if(f != NULL) {
        fprintf(f, "");
        fclose(f);
    }

    char vc[3];
    memcpy(&vc, (void*)0x5A60D0, 3);
    if(memcmp(vc, "1.5", 3) == 0)
        CODPATCH = 5;
    else
        CODPATCH = 1;
    game = new COD(CODPATCH);
    Com_Printf("Call of Duty Extended %.1f loaded\n", __EXTENDEDVERSION__);

    Cmd_AddCommand("codextended", version);
    Cmd_AddCommand("codversion", version);
    Cmd_AddCommand("help", version);
    Cmd_AddCommand("load", ScriptInitiating); //manual loading
    Cmd_AddCommand("dump", writePlayerMethods);

    InitScriptFunctions();

    if(CODPATCH == 5) {
        cracking_hook_call(0x465629, (int)Sys_LoadDll);
    } else {
        cracking_hook_call(0x460399, (int)Sys_LoadDll);
        _memset((void*)0x452FB3, 0x90, 2);
    }

    patchScriptCall();

    if(GetModuleHandle("game_mp_x86.dll") != NULL) {
        ScriptInitiating();
    }

    patchDownloading();

    //printf("MySQL client: %s\n", mysql_get_client_info());
    /*
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
    */

    return 0;
}

void version() {
    Com_Printf("Call of Duty Extended %d [ riicchhaarrd ]\n", __EXTENDEDVERSION__);
}
