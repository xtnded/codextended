#include "codextended.hpp"
#include <math.h>
#include "common/patch.hpp"
#include "game/surfaceflags.h"
#include "game/js.hpp"

/*
	Call of Duty Extended
	Patch 1.1 Linux
	
	- riicchhaarrd
*/

MYSQL *db;

void fnop() {
	;
}

int StuckInPlayer(int a1) {
	return 0;
}

//hardcode patching it to 0x20 = 32 = CONTENTS_WATER
//which takes damage (from game without having to alter the way it works and you have no annoying blockers
void G_SetPlayerContents(int a1) {
  /*int result = *(int*)(a1 + 344);
  if ( *(int*)(result + 8664) || *(int*)(result + 8668) || *(int*)(result + 8400) == 1 )
    *(int*)(a1 + 280) = 0;
  else
    *(int*)(a1 + 280) = CONTENTS_WATER;*/
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

gentity_t* mySpawnPlayerClone();

extern int bodyqueindex;

void myClientBegin(int);

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
	*(int *)&Sys_LoadDllz = 0x80C5FE4;
	void *ret = Sys_LoadDllz(name, dest, entryPoint, systemcalls);
	
	char libn[512];
	char* check = Cvar_VariableString("fs_game");
	if(check[0] == '\0')
		sprintf(libn, "main/game.mp.i386.so");
	else
		sprintf(libn, "%s/game.mp.i386.so", check);
	//char* libn = "/home/cod/codextended/game.mp.i386.so";
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
	
	int stuck = (int)dlsym(ret, "StuckInClient");
	cracking_hook_function(stuck, (int)StuckInPlayer);
	
	int cont = (int)dlsym(ret, "G_SetClientContents");
	cracking_hook_function(cont, (int)G_SetPlayerContents);
	
	int h66 = (int)dlsym(ret, "ClientEndFrame") + 0x173; //patch contents
	cracking_nop(h66, h66+0xa);
	
	
	//int h93 = GAME("PM_Weapon")+0x63;
	//cracking_nop(h93,h93+5);
	
	game_error = (game_error_t)GAME("G_Error");
	
	int h88 = GAME("vmMain")+0x54;
	cracking_hook_call(h88, (int)myClientConnect);
	int h89 = GAME("vmMain")+0xa0;
	cracking_hook_call(h89, (int)myClientBegin);
	int h90 = GAME("vmMain")+0x91;
	cracking_hook_call(h90, (int)myClientDisconnect);
	/*
	====
	CLIENT
	====
	*/
	
	int b = GAME("vmMain")+0xb0;
	cracking_hook_call(b, (int)myClientCommand);
	
	int b1 = GAME("vmMain")+0x83;
	cracking_hook_call(b1, (int)myClientUserinfoChanged);
	
	
	//q3msgboom fix
	int q21 = GAME("G_Say")+0x50e;
	int q22 = GAME("G_Say")+0x5ca;
	*(int*)q21=0x37f;
	*(int*)q22=0x37f;
	
	//deadchat fix
	int b2 = GAME("G_SayTo")+0x70;
	cracking_nop(b2, b2+2);
	
	int b3 = GAME("G_Say")+0x3B6;
	int b4 = GAME("G_Say")+0x2B3;
	*(byte*)b3 = 0xeb;
	*(byte*)b4 = 0xeb;
	
	//end deadchat fix
	
	int y7 = GAME("SpectatorThink")+0x123;
	*(int*)y7 = 0;
	
	cracking_hook_call((GAME("ClientSpawn")+0x3e9), (int)myClientEndFrame);
	cracking_hook_call((GAME("G_RunFrame")+0x645), (int)myClientEndFrame);
	
	int gk1 = GAME("G_InitGame")+0x2E4;
	int gk2 = GAME("G_InitGame")+0x2B8;
	printf("GK1 = %d\n", *(byte*)gk1);
	printf("GK2 = %d\n", *(byte*)gk2);
	*(byte*)gk1 = 80;
	*(byte*)gk2 = 80;
	
	//int gk3 = GAME("G_SpawnPlayerClone")+10;
	//*(int*)gk3 = (int)&bodyqueindex;
	
	int bec = GAME("ScriptEntCmd_MoveTo")+0x2E;
	*(byte*)bec = 0xeb;
	//because fuck you limiting to script_*
	
	//int b5 = GAME("PmoveSingle");
	//cracking_hook_function(b5+0x404, b5+0x3ea);
	
	#if 0
	#define DB_SERVER "localhost"
	#define DB_USER "root"
	#define DB_DATABASE "cod1"
	#endif
	cvar_t* db_password = Cvar_Get("db_password","",0);
	cvar_t* db_username = Cvar_Get("db_username","root",0);
	cvar_t* db_database = Cvar_Get("db_database","",0);
	cvar_t* db_server = Cvar_Get("db_server","localhost",0);
	db = mysql_real_connect(db,db_server->string,db_username->string,db_password->string,db_database->string,0,NULL,0);
	if(!db) {
		printf("Could not connect to the MySQL Database.\n");
		COD_Destructor();
	} else {
		printf("Connected to the MySQL Database.\n");
	}
	return ret;
}

void Info_Out() {
	int argc = Cmd_Argc();
	if(argc < 2) {
		printf("Usage: out <hexadecimal offset>\n");
		printf("Return: <integer>\n");
		return;
	}
	
	int* off = (int*)strtoll(Cmd_Argv(1), NULL, 16);
	if(off) {
		printf("%d\n", *off);
	}
}

void Info_Set() {
	int argc = Cmd_Argc();
	if(argc < 3) {
		return;
	}
	
	int* off = (int*)strtoll(Cmd_Argv(1), NULL, 16);
	int value = atoi(Cmd_Argv(2));
	if(off) {
		int old = *off;
		*off = value; 
		printf("%d > %d\n", old, *off);
	}
}

/*

function get(a, i) {
tmp = (0xaea77f80 + 788*a)+i;
return tmp.toString(16);
}

*/

void Info_Get() {
	int argc = Cmd_Argc();
	if(argc<2){
		printf("Error\n");
		return;
	}
	char* str = Cmd_Argv(1);
	int* p = (int*)dlsym(gamelib,str);
	if(p) {
		printf("%s was found at %x (%d)\n", str, (int)p);
	}
}

void COD_Destructor() {	
	delete game;
	printf("xtnded finalizer...\n");
	//js_destroy();
	mysql_close(db);
	mysql_library_end();
	//muntrace();
	
	//call original ofcourse to do less work :>
	void (*call)();
	*((int*)(&call)) = 0x806D910;
	call();
}

char* Cvar_InfoString(int bit) {
	/*static char info[MAX_INFO_STRING];
	cvar_t  *var;

	info[0] = 0;
	
	cvar_t *cvar_vars = (cvar_t*)0x834A0E0;
	
	for ( var = cvar_vars ; var ; var = var->next ) {
		if ( var->flags & bit ) {
			printf("%s, ", var->name);
			Info_SetValueForKey( info, var->name, var->string );
		}
	}
	printf("\n");
	return info;*/
	char* (*call)(int);
	*((int*)(&call)) = 0x806FC30;
	char* ret = call(bit);
	Info_SetValueForKey(ret, "sv_maxclients", "2147");
	printf("ret = %s\n", ret);
	return ret;
}

void info_setvalueforkey( char *s, const char *key, const char *value) {
	/*
	0D 0A //newline
	*/
	if(!strcmp(key, "sv_maxclients")) {
		Info_SetValueForKey(s, key, "1000");
	/*} else if(!strcmp(key, "hostname")) {
		//const char hostname[] = {'#','#','#','#', '#','#','#','#', '#','#','#','#', '#','#','#','#', 0xd, 0xa,'#','#','#','#', '#','#','#','#', '#','#','#','#', '#','#','#','#', 0xd, 0xa,'#','#','#','#', '#','#','#','#', '#','#','#','#', '#','#','#','#', 0xd, 0xa};
		const char hostname[] = {'^','2','w','w', 'w','.','c','o', 'd','1','.','e', 'u',' ','!','!', 0xd, 0xa,'^','2','@','@', '@','@','@','@', '@','@','@','@', '@','@','@','@', 0xd, 0xa,'@','@','@','@', '@','@','@','@', '@','@','@','@', '@','@','@','@'};
		Info_SetValueForKey(s, key, hostname);
	/*} else if(!strcmp(key, "mapname")) {
		//Info_SetValueForKey(s, key, "mp_nuketown");*/
	} else {
		Info_SetValueForKey(s, key, value);
	}
}

void Cmd_Dummy() {
	char* (*callee)(int);
	*((int*)(&callee)) = 0x806FC30;
	char* ret = callee(4);
	printf("ret = %s\n", ret);
}

const char *__cdecl FS_ReferencedPakChecksums();
const char *__cdecl FS_LoadedPakPureChecksums();
const char *__cdecl FS_ReferencedPakNames();

CODEXTENDED::CODEXTENDED() {
	setbuf(stdout, NULL);
	mprotect((void *)0x08048000, 0x135000, PROT_READ | PROT_WRITE | PROT_EXEC);
	printf("Compiled: " __DATE__ " " __TIME__ "\n");
	//mtrace();
	
	cracking_hook_call(0x8094C54, (int)Script_GetCustomFunction);
	cracking_hook_call(0x80951C4, (int)Script_GetCustomMethod);
	cracking_hook_call(0x8092122, (int)zSys_LoadDll); //.text:08092122                 call    sub_80C5FE4
	
	SV_AddOperatorCommands();
	
	Cmd_AddCommand("a", Info_Out);
	Cmd_AddCommand("g", Info_Get);
	Cmd_AddCommand("s", Info_Set);
	
	*(byte*)0x80852B3 = 0x90;
	*(byte*)0x80852B4 = 0x90;
	*(byte*)0x80854FF = 0xeb;
	
	cracking_hook_function(0x80827D4, (int)info_setvalueforkey);
	
	//cracking_hook_function(0x0808C05F, (int)mid_statushook);
	
	//sendservercommand e game command > dropclient EXE_DISCONNECTED
	*(byte*)0x8085DBE = 0xeb;
	
	/*
		Direction Type Address         Text               
		--------- ---- -------         ----               
		Down      p    sub_808CDF8+35A call    sub_808CCCC
		Down      p    .text:0808D492  call    sub_808CCCC
	*/
	cracking_hook_call(0x808D152,(int)fnop);
	cracking_hook_call(0x808D492,(int)fnop);
	
	Cmd_AddCommand("dummy", Cmd_Dummy);
	
	cracking_hook_function(0x80717A4, (int)FS_ReferencedPakChecksums);
	cracking_hook_function(0x8071664, (int)FS_LoadedPakPureChecksums);
	cracking_hook_function(0x80716CC, (int)FS_ReferencedPakNames);
	
	//maxclients
	//*(byte*)0x8089F93 = 0xeb;
	//*(byte*)0x8089E49 = 0xeb;
	
	//hooking the "quit" commands and when server shutdowns basically for reasons that lib_unload was being a prick
	cracking_hook_call(0x0806B8CE, (int)COD_Destructor);
	//0806CB0C to D since push offset <shutdown offset>
	*(int*)0x0806CB0D = (int)COD_Destructor;
	
	clientInit();
	//js_init();
	db = mysql_init(NULL);
	if(!db) {
		printf("MySQL initialization failed.\n");
		COD_Destructor();
	} else {
		printf("MySQL initialized.\n");
	}
}

ENTITY* CODEXTENDED::getEntities() {
    return entities;
}

ENTITY* CODEXTENDED::getEntity(unsigned short index) {
    return &entities[index];
}

CODEXTENDED* game;
