#include "cracking.hpp"
#include "libcod.hpp"
#include "include.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define PLAYERSTATE_SIZE 0x22cc
#define GENTITY_SIZE 0x31c
#define PLAYERSTATE_OFFSET 0xCADF4
int g_entities = 0x201756A0;
int playerstates = 0x20242760;

int game_mp;

typedef struct {
    bool bunnyhop = false;
    char cmd[MAX_STRING_TOKENS];
    char chat[MAX_STRING_TOKENS];
} player;

struct {
    string dlWhitelist[1024];
} pserver;

player players[64];

DWORD WINAPI MyThread(LPVOID);
DWORD g_threadID;
HMODULE g_hModule;

void phpversion();
int sub_2001E5B0(int);

int* getPlayerState(int* gentity) {
    int* ps = (int*)(*(int*)(gentity + 348));
    return ps;
}

class cHook
{
	public:
	int from;
	int to;
	unsigned char oldCode[5];
	cHook(int from, int to)
	{
		this->from = from;
		this->to = to;
	}

	void hook()
	{
		memcpy((void *)oldCode, (void *)from, 5);
		cracking_hook_function(from, to);
	}

	void unhook()
	{

		memcpy((void *)from, (void *)oldCode, 5);
	}
};

extern "C" BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            g_hModule = hinstDLL;
            DisableThreadLibraryCalls(hinstDLL);
            CreateThread(NULL, NULL, &MyThread, NULL, NULL, &g_threadID);
            break;

        case DLL_PROCESS_DETACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;
    }
    return true;
}
/*
void fDownload(int *a, int *b) {
    //int c = *(int*)(a + 68612);
    //int *loc = (int*)(0x008580A84 + 0x5A9B4 * num);
    //char *name;
    //for(int i = 0; i < 64; i++) {
    //}
    //sub_458860();
}
*/

/*
    //TODO
        * Move original clientnumber into ECX register and call ClientCommand wherever I want
*/
void myClientCommand() {
    int *sp;
    asm ("movl %%ecx, %0" : "=r" (sp) );
    ClientCommand();
    int num = (int)sp;
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
        char said[1024];
        trap_Argv(1, said, 1024);
        strcpy(players[num].chat, said);

        FILE *f = fopen("C:/COD/chatlog.txt", "a");
        if(f != NULL) {
            fprintf(f, "%d:", num);
            fprintf(f, said);
            fprintf(f, "\n");
            fclose(f);
        }
    }
    //Com_Printf("[php] num=%i argc=%d cmd0=%s\n", num, Cmd_Argc(), Cmd_Argv(0));

    //Com_Printf("[php] cmd=%s\n",cmd);
	//asm("mov var, %0" : "=r" (out));
	//int *ecx = (int*)(0x02497BD0 + 0x10);
	//asm("movl %0, %%ecx" : "=r" (ecx));
    //ClientCommand();
}

void enableGlobalBunnyhop() {
    int argc = trap_Argc();
    int *loc = (int*)0x20242770;
    if(argc < 2) {
        Com_Printf("Not enough arguments.\n");
        return;
    }
    for(int i = 0; i < 64; i++) {
        if(players[i].bunnyhop) {
            loc = (int*)(0x20242770 + PLAYERSTATE_SIZE * i);
            *loc = 0;
        }
        players[i].bunnyhop = !players[i].bunnyhop;
    }
}

void setVelocity(int clientNum, float vel[3]) {
    /*int argc = Cmd_Argc();
    int num;
    Cmd_TokenizeString(Cmd_Cmd());
    if(argc < 4) {
        Com_Printf("Usage: velocity <clientnum> <x> <y> <z>.\n");
        return;
    }
    num = atoi(Cmd_Argv(1));*/
    float *x = (float*)(0x20242780 + PLAYERSTATE_SIZE * clientNum);
    float *y = (float*)((int)x+4);
    float *z = (float*)((int)x+8);
    //float velx = atof(Cmd_Argv(2));
    //Com_Printf("value: %f\n", velx);
    //memcpy(x, &velx, 4);
    /*x += atof(Cmd_Argv(2));
    *y += atof(Cmd_Argv(3));
    *z += atof(Cmd_Argv(4));*/
    *x = vel[0];
    *y = vel[1];
    *z = vel[2];
}

float* getPlayerAngles(int num) {
    float *x = (float*)(0x20242820 + PLAYERSTATE_SIZE * num);
    float *y = (float*)((int)x+4);
    float xy[2] = {*x, *y};
    delete[] x;
    delete[] y;
    return xy;
}

void bhop() {
    int argc = trap_Argc();
    int num;
    int *loc = (int*)playerstates + 0x10;
    if(argc < 2) {
        Com_Printf("Not enough arguments.\n");
        return;
    }
    char cmd[1024];
    trap_Argv(1, cmd, 1024);
    num = atoi((char*)cmd);
    //memcpy((void*)loc, (int*)&i, sizeof(int));
    if(players[num].bunnyhop) {
        loc = (int*)(playerstates + 0x10 + PLAYERSTATE_SIZE * num);
        *loc = 0;
    }
    players[num].bunnyhop = !players[num].bunnyhop;
}

int ClientSpawn(int a1, int a2, int a3) {
    int *gent = (int*)a1;
    Com_Printf("[php] a1=%d,a2=%d,a3=%d,gent=%p\n",a1,a2,a3,gent);
}

/*int G_Damage(int a1, int *a2, int *a3, int a4, int a5, int a6, int a7, int a8, int a9) {
    Com_Printf("[COM_PRINTF]: G_Damage called: %d, %p, %p, %d, %d, %d, %d, %d, %d, %d", a1, a2, a3, a4, a5, a6, a7, a8, a9);
}*/

int sub_200355C0();

int sub_200198F0(int a) {
    return 0;
}

int G_SetPlayerSize() {
    *(float*)(0x2006EA10) = 0;
    *(float*)(0x2006EA14) = 0;
    *(float*)(0x2006EA18) = 0;
    *(float*)(0x2006EA1C) = 0;
    *(float*)(0x2006EA20) = 0;
    *(float*)(0x2006EA24) = 0;
    return 0;
}

int sub_20018CE0(int n) {
    //int *a1 = (int*)(g_entities + n * GENTITY_SIZE);
    //int *a263 = (int*)(g_entities + 263 * GENTITY_SIZE);
    /*int result; // eax@1

    result = *(DWORD *)(a1 + 348);
    //if ( *(DWORD *)(result + 8672) || *(DWORD *)(result + 8676) ) {
    if(!server_settings.contents) {
        *(DWORD *)(a1 + 284) = 0;
    } else {
        result = *(DWORD *)(result + 8396) != 1 ? 0x2000000 : 0;
        *(DWORD *)(a1 + 284) = result;
    }*/
    //Com_Printf("[php::debug] ::%i\n", a1);//*((int*)a1+0x11c));
}

//----- (2000CF80) --------------------------------------------------------
void __cdecl PmoveSingle(int a1) {
    int *a = (int*)a1;
    Com_Printf("[php] a1 = %p\n", a);
}

int func_20002FA0(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) {
    Com_Printf("PlayAnim called; %i %i %i %i %i %i %i\n", a1, a2, a3, a4, a5, a6, a7, a8);
}

int sub_20032190(int d1) {
    vec3_t org;
    char* txt;
    vec3_t col;
    char* temp;
    float sz;
    float d;
    stackGetParamVector(0, org);
    stackGetParamString(1, &txt);
    stackGetParamVector(2, col);
    //TODO float
    stackGetParamString(3, &temp);
    sz = atof(temp);
    stackGetParamString(4, &temp);
    d = atof(temp);

    trap_AddDebugString(org, col, sz, txt);
    return 0;
}

typedef void (*sub_458680_t)(int*,int*); //fRedirect orig
sub_458680_t sub_458680 = (sub_458680_t)0x458680;

typedef void (*sub_458860_t)(int*,int*); //fDownload orig
sub_458860_t sub_458860 = (sub_458860_t)0x458860;

void fRedirect(int *a1, int *a2) {
    int v2; // ebx@1
    int v3; // ebp@1
    int v4; // eax@1
    int v5; // eax@2
    int v6; // eax@2
    int v7; // eax@5
    int v8; // eax@8
    signed int v9; // edx@11
    int v10; // eax@13
    signed int result; // eax@14
    int v12; // [sp+8h] [bp-Ch]@1
    int v13; // [sp+Ch] [bp-8h]@1
    int v14; // [sp+10h] [bp-4h]@1
    Com_Printf("fRedirect; %i\n", v4);
    sub_458680(a2,a1);
}

void fDownload(int *a1, int *a2) {
    sub_458680(a2,a1);
    /*char dlname[256];
    memcpy(&dlname, (int*)(a1 + 0x10A64), 256);
    Com_Printf("Download requested; %s (a1: %p, a2: %p)\n", dlname, a1, a2);*/
}


void __cdecl G_Damage(int *targ, int *inflictor, int *attacker, vec3_t dir, vec3_t point, int damage, int dflags, int mod);
cHook hG_Damage(0x20022A30, (int)G_Damage);

void __cdecl G_Damage(int *targ, int *inflictor, int *attacker, vec3_t dir, vec3_t point, int damage, int dflags, int mod) {
    hG_Damage.unhook();

    typedef void (__cdecl *G_Damage_t)(int *targ, int *inflictor, int *attacker, vec3_t dir, vec3_t point, int damage, int dflags, int mod);
    G_Damage_t G_Damage_f = (G_Damage_t)0x20022A30;
    G_Damage_f(targ, inflictor, attacker, dir, point, damage, dflags, mod);

    hG_Damage.hook();
}

typedef int (*loadCallback_t)(char*,char*); //loadcallbackscript
loadCallback_t loadCallback = (loadCallback_t)0x472AB0;

typedef int (*dword_20031760_t)();
dword_20031760_t dword_20031760 = (dword_20031760_t)0x20031760;

typedef int (*Scr_ExecThread_t)(int argc, int threadindex);
Scr_ExecThread_t Scr_ExecThread = (Scr_ExecThread_t)0x481E10;

typedef int (*Scr_ExecEntThread_t)(int argc, int threadindex, int, int a1); //ex 0-64 0 8910 9
Scr_ExecEntThread_t Scr_ExecEntThread = (Scr_ExecEntThread_t)0x481EC0;

typedef int (*Scr_FreeThread_t)(int threadindex);
Scr_FreeThread_t Scr_FreeThread = (Scr_FreeThread_t)0x482070;

int callbackTest;
int callbackPlayerDamage;

int GScr_LoadGameTypeScript() {
    int g = dword_20031760();
    callbackTest = loadCallback("maps/mp/gametypes/_callbacksetup", "CodeCallback_Test");
    //callbackTest = loadCallback("CodeCallback_Test", "maps/mp/gametypes/_callbacksetup");
    callbackPlayerDamage = *(int*)0x202D89D8;
    Com_Printf("callbackTest loaded: %i\n", callbackTest);
    Com_Printf(">>DEBUG\n");
    Com_Printf("callbackPlayerDamage: %i\n", callbackPlayerDamage);
    return g;
}

void loadscr() {
    game_mp = (int)GetModuleHandle("game_mp_x86.dll");
    g_entities = game_mp + 0x1756A0;
    playerstates = g_entities + 0xcd0c0;
    Com_Printf("[game_mp: %02x]\n", game_mp);
    Com_Printf("[g_entities: %02x]\n", g_entities);
    Com_Printf("[playerstates: %02x]\n", playerstates);
    cracking_hook_call(0x20031AD8, (int)GScr_LoadGameTypeScript);
    //cracking_hook_function(0x20002fa0, (int)func_20002FA0);
    //cracking_hook_call(0x0458952, (int)fRedirect);
    //cracking_hook_call(0x0462F62, (int)fDownload);

    //cracking_hook_function(0x2001B8F0, (int)G_SetPlayerSize);
    cracking_hook_function(0x20018CE0, (int)sub_20018CE0);
    //cracking_hook_function(0x20032190, (int)sub_20032190);

    //cracking_hook_function(0x2000CF80, (int)PmoveSingle);

    cracking_hook_function(0x200355C0, (int)sub_200355C0);
    cracking_hook_call(0x2002747C, (int)myClientCommand);
    cracking_hook_function(0x200198F0, (int)sub_200198F0);
    //cracking_nop(0x2001B458, 0x2001B486);
    //cracking_hook_function(0x20022A30, (int)G_Damage);
    //hG_Damage.hook();
}

typedef void* (*sub_4686D0_t)( const char *name, char *fqpath, int( **entryPoint ) ( int, ... ), int ( *systemcalls )( int, ... )); //Sys_LoadDll
sub_4686D0_t sub_4686D0 = (sub_4686D0_t)0x4686D0;

void *Sys_LoadDll( const char *name, char *fqpath, int( **entryPoint ) ( int, ... ),
						  int ( *systemcalls )( int, ... ) ) {
    void *l = sub_4686D0(name, fqpath, entryPoint, systemcalls);
    loadscr();
    return l;
}

void PatchCvarCheck( void ) {
 DWORD old;
 //Read Only
 VirtualProtect((void*)0x43BCDA,1,PAGE_EXECUTE_READWRITE,&old);
 *((BYTE*)0x43BCDA) = 0xEB;
 VirtualProtect((void*)0x43BCDA,1,old,&old);

 //Write Protected
 VirtualProtect((void*)0x43BD20,1,PAGE_EXECUTE_READWRITE,&old);
 *((BYTE*)0x43BD20) = 0xEB;
 VirtualProtect((void*)0x43BD20,1,old,&old);

 //Cheat Protected
 VirtualProtect((void*)0x43BD20,1,PAGE_EXECUTE_READWRITE,&old);
 *((BYTE*)0x43BD20) = 0xEB;
 VirtualProtect((void*)0x43BD20,1,old,&old);
}

void dl_whitelist() {
    int ln = 0;
    string l;
    Com_Printf("Reading whitelisted downloads.\n");
    ifstream f("C:/COD/dl_whitelist.txt");
    if(f.is_open()) {
        while(!f.eof()) {
            getline(f, l);
            pserver.dlWhitelist[ln] = l;
            ln++;
            Com_Printf("Added: %s\n", l.c_str());
        }
        f.close();
    } else {
        Com_Printf(":: No files were added.");
    }
}

void testCallback() {
    /*int a1 = 0;
    vec3_t dir = {0,0,0};
    //eInflictor, eAttacker, iDamage, iDFlags, sMeansOfDeath, sWeapon, vPoint, vDir, sHitLoc
    stackPushInt(0);
    stackPushInt(0);
    stackPushInt(200);
    stackPushInt(0);
    stackPushInt(0);
    stackPushString("colt_mp");
    stackPushVector(dir);
    stackPushVector(dir);
    stackPushString("");
    int test = Scr_ExecEntThread(9, callbackPlayerDamage, 0, a1);
    */
    stackPushInt(1337);
    int test = Scr_ExecEntThread(1, callbackTest, 0, 0);
    Com_Printf("int test = %i;\n", test);
    int result = Scr_FreeThread(test);
    Com_Printf("Result: %i\n", result);
}

/*
btDiscreteDynamicsWorld* dynamicsWorld;
btSequentialImpulseConstraintSolver* solver;
btCollisionDispatcher* dispatcher;
btDefaultCollisionConfiguration* collisionConfiguration;
btBroadphaseInterface* broadphase;
*/
DWORD WINAPI MyThread(LPVOID) {
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

    dl_whitelist();

    PatchCvarCheck();
    Com_Printf("PHP 1.5 loaded\n");
    //ClientCommand_f = (ClientCommand_t)0x20021EC0;
    Cmd_AddCommand("phpversion", phpversion);
    Cmd_AddCommand("help", phpversion);
    Cmd_AddCommand("load", loadscr); //manual loading
    Cmd_AddCommand("cb", testCallback); //manual loading
    cracking_hook_call(0x465629, (int)Sys_LoadDll);
    //ClientCommand_p = (ClientCommand_f) hookfunc(0x2002747C,(int)&ClientCommand);
    //cracking_hook_call(0x00462F62, (int)fDownload);
    //cracking_hook_call(0x2001FCAB, (int)myG_LogPrintf);
    //cracking_hook_function(0x20022A30, (int)G_Damage);
    //cracking_hook_function(0x2001B380, (int)ClientSpawn);
    //cracking_hook_call(0x4555e7, (int)sub_45cd90);
    //cracking_hook_function(0x2001E5B0, (int)sub_2001E5B0);
    //cracking_hook_call(0x20031AD8, (int)myGScr_LoadGameTypeScript);
    while(true) {
        /*if(GetAsyncKeyState(VK_LCONTROL) & 1) {
            //Com_Printf("Hello people %d\n", 1337);
            Sleep(1000);
        }
    Sleep(1);*/
    }
    /*
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
    */
    FreeLibraryAndExitThread(g_hModule, 0);
    return 0;
}

void phpversion() {
    /*typedef void (__stdcall *outz)(const char*);
    outz func2 = (outz) (0x080609D4);
    func2("status");*/
    //printf("hey");
    //MessageBox(NULL, "Call of Duty MessageBox", "Call of Duty", MB_OK);
    //char *p;
    //p = Cmd_Args();
    //int argc = Cmd_Argc();
    //Com_Printf("Call of Duty 1.5 running PHP1\n");
    int version = 1;
    Com_Printf("Running Call of Duty 1.5 with php%d.\nXfire: phpcod\n", version);
    //dword_200A49E0(argc-1);
    //Cmd_TokenizeString(Cmd_Cmd());
    //Com_Printf("Command was: %s\n", Cmd_Cmd());
    //for(int i = 0; i < argc; i++) {
    //    Com_Printf("Argument %d: %s\n", i, Cmd_Argv(i));
    //}
}

void entitygsc(int cmd, int a1) {
    switch(cmd) {
        case 10: {
            float *x = (float*)(playerstates + 0x20 + PLAYERSTATE_SIZE * a1);
            float *y = (float*)((int)x+4);
            float *z = (float*)((int)x+8);
            float vec[3] = {*x,*y,*z};
            stackPushVector(vec);
            }
        break;
        case 11: {
            float receiver[3];
            stackGetParamVector(2, receiver);
            setVelocity(a1, receiver);
            }
        break;
        case 12: {
            float *x = (float*)(playerstates + 0xc0 + PLAYERSTATE_SIZE * a1);
            float *y = (float*)((int)x+4);
            float angles[3] = {*x, *y, 0};
            stackPushVector(angles);
        }
        break;
        case 15: {
            *(int*)(playerstates + 0x10 + PLAYERSTATE_SIZE * a1) = 100;
        }
        break;
        case 14: {
            //Com_Printf("pushing: num: %i %s\n", a1, players[a1].cmd);
            stackPushString((char*)players[a1].cmd);
            strcpy(players[a1].cmd, (char*)"");
        }
        break;
        case 20: {
            stackPushString((char*)players[a1].chat);
            strcpy(players[a1].chat, (char*)"");
        }
        break;
        case 18: {
            int *ptr = (int*)(playerstates + 0xc + PLAYERSTATE_SIZE * a1);
            stackPushBool((*ptr & 0x8) == 0x8);
        }
        break;
        case 16: {
            int width, height;
            stackGetParamInt(2, &width);
            stackGetParamInt(3, &height);
            *(float*)(GENTITY_SIZE * a1 + g_entities + 280) = height;
            *(float*)(GENTITY_SIZE * a1 + g_entities + 276) = width;
            *(float*)(GENTITY_SIZE * a1 + g_entities + 272) = width;
            *(float*)(GENTITY_SIZE * a1 + g_entities + 264) = -width;
            *(float*)(GENTITY_SIZE * a1 + g_entities + 260) = -width;
            //syscall(53, (int*)(GENTITY_SIZE * a1 + g_entities));
        }
        break;
        case 17: {
            int in;
            stackGetParamInt(2, &in);
            *(int*)(GENTITY_SIZE*a1+g_entities+357) = in;
        }
        case 19: {
            int in;
            stackGetParamInt(2, &in);
            *(int*)(GENTITY_SIZE*a1+g_entities+373) = in;
        }
        break;

        case 22: {
            int ping = trap_getClientPing(a1);
            stackPushInt(ping);
        }
        break;

        case 23: {
            int *ptr = (int*)(GENTITY_SIZE*a1+g_entities);
            trap_LinkEntity(ptr);
        }
        break;

        case 24: {
            int *ptr = (int*)(GENTITY_SIZE*a1+g_entities);
            trap_UnlinkEntity(ptr);
        }
        break;

        case 25: {
            int *ptr = (int*)(PLAYERSTATE_SIZE * a1 + playerstates + 0x21f0);
            stackPushBool(*ptr & 0x10);
        }
        break;

        case 26: {
            char* reason;
            stackGetParamString(2, &reason);
            trap_DropClient(a1, reason);
        }
        break;

        case 27: {
            char* str;
            int *ptr = (int*)(PLAYERSTATE_SIZE * a1 + playerstates + 0x21fc);
            stackGetParamString(2, &str);
            /*if(strcmp(str, "left")) {
                stackPushBool(*ptr & 0x10);
            } else {
                stackPushBool(*ptr & 0x20);
            }*/
            stackPushInt(*ptr);
        }
        break;
    }
}

/*
    :: Script Function List
        17 //setPhysics
        19 //ent->takedamage
        16 //setSize/setBounds/ent->mins&maxs
        18 //jumpButtonPressed
        14 //getCommand
        20 //getChat
        15 //ps->jumpTime
        12 //getPlayerAngles
        10 //getVelocity
        11 //setVelocity
        70 //file_read
        71 //file_save
        72 //file_exists
*/

int sub_200355C0() {
    int a1;
    stackGetParamInt(0, &a1);
    switch(a1) {
        case 10:
        case 11:
        case 12:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:

        case 14: {
            int num;
            stackGetParamInt(1, &num);
            entitygsc(a1, num);
            }
        break;

        case 13: {
            BYTE *b = (BYTE*)(a1 * GENTITY_SIZE + g_entities + 348 + 8696);
            Com_Printf("debug; %d\n", (unsigned int)*b);
            //float vec[3] = {*buttons,0,0};
            //stackPushVector(vec);
        }
        break;

        case 3: {
            trace_t trace;
            vec3_t start, endl, mins, maxs;
            int ignore, contentmask;
            stackGetParamVector(1, start);
            stackGetParamVector(2, endl);
            stackGetParamVector(3, mins);
            stackGetParamVector(4, maxs);
            stackGetParamInt(5, &ignore);
            stackGetParamInt(6, &contentmask);
            //Com_Printf("trap_Trace called; start: (%f %f %f) end: (%f %f %f)\n", start[0], start[1], start[2], endl[0], endl[1], endl[2]);
            trap_Trace(&trace, start, mins, maxs, endl, ignore, MASK_SHOT);
            //Com_Printf("finished trap_trace\n");
            //Com_Printf("::output %f %f %f\n", trace.endpos[0], trace.endpos[1], trace.endpos[2]);
            stackPushVector(trace.endpos);
        }
        break;

        case 5: {
            int t;
            stackGetParamInt(0, &t);
        }
        break;

        case 70: {
            char* name;
            int len;
            stackGetParamString(1, &name);
            stackGetParamInt(2, &len);
            FILE *f = fopen(name, "r");
            if(f == NULL) {
                stackPushInt(0);
            } else {
                char txt[len];
                fgets(txt, len, f);
                puts(txt);
                fclose(f);
                stackPushString(txt);
            }
        }
        break;

        case 71: {
            char* name;
            char* mode;
            char* text;
            stackGetParamString(1, &name);
            stackGetParamString(2, &mode);
            stackGetParamString(3, &text);
            FILE *f = fopen(name, mode);
            if(f == NULL) {
                stackPushBool(false);
            } else {
                stackPushBool(true);
                fprintf(f, text);
                fclose(f);
            }
        }
        break;

        case 72: {
            char* name;
            stackGetParamString(1, &name);
            if(FILE *f = fopen(name, "r")) {
                fclose(f);
                stackPushBool(true);
            } else {
                stackPushBool(false);
            }
        }
        break;

        /*case 70: {
            char* name;
            int mode;
            fileHandle_t handle;
            stackGetParamString(1, &name);
            stackGetParamInt(2, &mode);
            int result = trap_FS_FOpenFile(name, &handle, FS_WRITE);
            if(result < 0)
                stackPushInt(-1);
            else
                stackPushInt(handle);
        }
        break;

        case 71: {
            fileHandle_t handle;
            char* buffer;
            stackGetParamInt(1, &handle);
            stackGetParamString(2, &buffer);
            trap_FS_Write(buffer, strlen(buffer), handle);
        }
        break;

        case 72: {
            fileHandle_t handle;
            char* buffer;
            int len;
            stackGetParamInt(1, &handle);
            stackGetParamInt(2, &len);
            trap_FS_Read(&buffer, len, handle);
        }
        break;

        case 73: {
            fileHandle_t handle;
            stackGetParamInt(1, &handle);
            trap_FS_FCloseFile(handle);
        }
        break;*/

        case 100: {
            int test = Scr_ExecThread(callbackTest, 0);
            Scr_FreeThread(test);
        }
        break;

        default: {
                stackPushString((char*)"Default Text //php");
            }
        break;
    }
    return 0;
}

int sub_2001E5B0(int a1) { //allowspectateteam
    if(a1 > 0x3FF)
        return 0;
    return 0;
}
