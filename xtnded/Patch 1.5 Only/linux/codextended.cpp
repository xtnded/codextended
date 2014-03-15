#include "codextended.hpp"
#include <math.h>
#include "common/patch.hpp"

/*
	Call of Duty Extended
	Patch 1.5 Linux
	
	- riicchhaarrd
*/

cvar_t* player_jumpheight;
cvar_t* jump_slowdownenable;
cvar_t* g_steep;

int PM_Jump(float f) {
	//TODO
}

int StuckInPlayer(int a1) {
	int tmp = (int)dlsym(gamelib, "PM_GetEffectiveStance");
	if(jump_slowdownenable->integer)
		*(byte*)(tmp+0x106F) = 0x7f;
	else
		*(byte*)(tmp+0x106F) = 0xeb;
	
	if(g_steep->integer) {
		//*(byte*)(tmp+0x36D2) = 0xeb;
		*(byte*)(tmp+0x36D2) = 0x90;
		*(byte*)(tmp+0x36D3) = 0x90;
	}
	
	*(float*)(tmp+0x10AC) = player_jumpheight->value; //jumpheight
	return 0;
}

void G_SetPlayerContents(int a1) {
}

/*
	thanks to kung foo man for unprotect_lib
*/
int unprotect_lib(char *libname)
{
    char buf[512];
    char flags[4];
    void *low, *high;
    FILE *fp;
    fp = fopen("/proc/self/maps", "r");
    if ( ! fp)
        return 0;
    while (fgets(buf, sizeof(buf), fp))
    {
        if ( ! strstr(buf, libname))
            continue;
        if (sscanf (buf, "%p-%p %4c", &low, &high, flags) != 3)
            continue;
        //printf(buf);
        //printf("%08x %08x diff:%.8p\n", low, high, (int)high-(int)low);
        mprotect((void *)low, (int)high-(int)low, PROT_READ | PROT_WRITE | PROT_EXEC);
    }
    fclose(fp);
    return 1;
}

void *zSys_LoadDll(char *name, char *dest, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...)) {
	char *err;
	char *error;
	char *fn;
	char *gamedir;
	char *basepath;
	char *homepath;
	char *pwdpath;
	char fname[100];
	void *dllEntry;
	void *libHandle;
	
	void *(*Sys_LoadDllz)(char *name, char *dest, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...));
	*(int *)&Sys_LoadDllz = 0x080D3DAD;
	void *ret = Sys_LoadDllz(name, dest, entryPoint, systemcalls);
	
	char* libn = "/home/cod/main/game.mp.i386.so";
	unprotect_lib(libn);
	
	//mprotect(ret, 0x8A400, PROT_READ | PROT_WRITE | PROT_EXEC);
	
	gamelib = ret;
	base = (int)dlsym(ret, "vmMain"); //0x4D84C
	gentities = (int)dlsym(ret, "g_entities");
	zpml = (char (*)[140])dlsym(ret, "pml");
	zpm = (int)dlsym(ret, "pm");
	for(int i = 0; i < MAX_ENTITY_SIZE; i++) {
		game->entities[i].index = i;
		game->entities[i].base = gentities + GENTITY_SIZE * i;
		game->entities[i].ptr = gentities + GENTITY_SIZE * i;
	}
	scriptInitializing();
	
	//cracking_hook_function(((int)dlsym(ret, "PM_GetEffectiveStance")+0xEDD), (int)PM_Jump);
	
	int stuck = (int)dlsym(ret, "StuckInClient");
	cracking_hook_function(stuck, (int)StuckInPlayer);
	
	int cont = (int)dlsym(ret, "G_SetClientContents");
	cracking_hook_function(cont, (int)G_SetPlayerContents);
	
	/*
	int tmp = (int)dlsym(ret, "PmoveSingle");
	int calls[6] = {0x78e,0x7c9,0x7ff,0x88f,0x924,0x967};
	for(unsigned short i = 0; i < 6; i++) {
		int loc = tmp + calls[i];
		cracking_nop(loc, loc+5);
	}*/
	//printf("LAZY[%d] NOW[%d] GLOBAL[%d] LOCAL[%d]\n", RTLD_LAZY, RTLD_NOW, RTLD_GLOBAL, RTLD_LOCAL);
	
	
	/*
	====
	CLIENT
	====
	*/
	/*
	byte s[5] = {0xe8, 0xdf, 0x71, 0xfc, 0xff};
	int b = GAME("vmMain");
	int result = search_memory(b, b+0x244, &s[0], sizeof(s));
	printf("result = %x\n", result);
	cracking_hook_call(result, (int)myClientCommand);
	*/
	/*
	====
	EFLAG
	====
	*/
	/*byte flagBytes[8] = {0x74,0x1A,0x8B,0x45,0xBC,0x8B,0x55,0xBC};
	int flagStart = GAME("ClientEndFrame");
	if(flagStart != 0) {
		printf("not null\n");
		byte* flagMemory = (byte*)search_memory(flagStart, flagStart + 0x3AB2, &flagBytes[0], 8);
		if(flagMemory != 0) {
			*flagMemory = 0xeb;
			printf("flagMemory = %x\n", *flagMemory);
		}
	}*/
	return ret;
}

void Info_Out() {
	int argc = Cmd_Argc();
	if(argc < 2) {
		printf("Usage: out <hexadecimal offset>\n");
		printf("Return: <integer>\n");
		return;
	}
	
	int* off = (int*)atoi(Cmd_Argv(1));
	if(off) {
		printf("%x = %xh = %d\n", off, *off, *off);
	}
}

CODEXTENDED::CODEXTENDED() {
	setbuf(stdout, NULL);
	mprotect((void *)0x08048000, 0x135000, PROT_READ | PROT_WRITE | PROT_EXEC);
	printf("Compiled: " __DATE__ " " __TIME__ "\n");
	/*hook_Sys_LoadDll.load(0x080D3DAD, (int)Sys_LoadDll_80D3DAD);
	hook_Sys_LoadDll.hook();*/
	
	cracking_hook_call(0x809D975, (int)Script_GetCustomFunction);
	cracking_hook_call(0x809DBB1, (int)Script_GetCustomMethod);
	cracking_hook_call(0x809AEA8, (int)zSys_LoadDll);
	SV_AddOperatorCommands();
	
	Cvar_Set("sv_hostname", "^7CoDHost");
	player_jumpheight = Cvar_Get("player_jumpheight", "39", 0);
	jump_slowdownenable = Cvar_Get("jump_slowdownenable", "1", 0);
	g_steep = Cvar_Get("g_steep", "0", 0);
	//*(byte*)0x804b61c = 0x2f; //patch bsp version 0x2f = 47
	Cmd_AddCommand("out", Info_Out);
	clientInit();
}

ENTITY* CODEXTENDED::getEntities() {
    return entities;
}

ENTITY* CODEXTENDED::getEntity(unsigned short index) {
    return &entities[index];
}

CODEXTENDED* game;
