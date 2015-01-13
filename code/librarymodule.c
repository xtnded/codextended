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
#include "surfaceflags.h"
#include "server.h"

//hardcode patching it to 0x20 = 32 = CONTENTS_WATER
//which takes damage (from game without having to alter the way it works and you have no annoying blockers
//uhm might have to set 256 etc too
void G_SetPlayerContents(int a1) {
	/*int result = *(int*)(a1 + 344);
	if ( *(int*)(result + 8664) || *(int*)(result + 8668) || *(int*)(result + 8400) == 1 )
		*(int*)(a1 + 280) = 0;
	else
	*/
  
	
	if(!strcmp(x_contents->string, "manual"))
		return;
	if(x_contents->integer == -1)
		*(int*)(a1 + 280) = CONTENTS_BODY;
	else
		*(int*)(a1 + 280) = x_contents->integer;
	
}

int StuckInPlayer(int a1) {
	return 0;
}

gentity_t* mySpawnPlayerClone();

extern int bodyqueindex;

void myClientBegin(int);
void hG_Say(gentity_t *ent, gentity_t *target, int mode, const char *chatText);
void G_SayTo( gentity_t *ent, gentity_t *other, int mode, int color, const char *name, const char *message );

#if xDEBUG

void _PM_ClearAimDownSightFlag() {
	/*
	qpmove_t *pp = (qpmove_t*)pm;
	qps *ps = pp->ps;
	
	ps->pm_flags &= 0xDFu;
	*/
}

void QDECL _PM_UpdateAimDownSightFlag() {
	#if 0
	void (*_BG_UpdateConditionValue)(int,int,int,qboolean);
	*(int*)&_BG_UpdateConditionValue = GAME("BG_UpdateConditionValue");
	
	qpmove_t *pp = (qpmove_t*)pm;
	qps *ps = pp->ps;
	
	int v3 = *(int*)(&ps->cmdtime + 180);
	
	//something = *(int *)( *(int *)&pml[132] + 716)
	
	if(ps->pm_type <= 5 && pp->cmd.buttons & 0x10 && /*&& something*/v3 != 2 && v3 != 1 && v3 != 10 && v3 != 11 /*&& (*(int *)&pml[48] || pm_type == 1) )*/) {
		if(ps->pm_flags & 1) {
			if(!pp->oldcmd.flags & 0x10 || !pp->oldcmd.serverTime) {
				ps->pm_flags |= 0x20;
				*(byte*)&ps->pm_flags |= 4;
			}
		} else {
			ps->pm_flags |= 0x20;
		}
	} else {
		ps->pm_flags &= 0xDFu;
	}
	
	 //for animations
	if ( ps->pm_flags & 0x20 )
		_BG_UpdateConditionValue(*(int*)(ps + 172), 7, 1, 1);
	else
		_BG_UpdateConditionValue(*(int*)(ps + 172), 7, 0, 1);
	#endif
	
	int *pp = (int*)pm;
	int *ps = *pp;
	int *gclient = *ps;
	
	int *v4 = (int *)(ps + 12);
	
	int val = *(int*)(gclient + 21); //336? 84*4=336 /84/4=21??
	
	//Com_DPrintf("val = %d\n", val);
	
	if (val == 1023) {
		*v4 |= 0x20;
		return;
	}
		
	void (*call)();
	*(int*)&call=GAME("PM_UpdateAimDownSightFlag");
	call();
}
#endif

extern cvar_t *x_spectator_noclip ;

void *Sys_LoadDll(char *name, char *dest, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...)) {
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
	
	void *(*call)(char *name, char *dest, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...));
	*(int *)&call = 0x80C5FE4;
	void *ret = call(name, dest, entryPoint, systemcalls);
	
	char libn[512];
	char* check = Cvar_VariableString("fs_game");
	if(check[0] == '\0')
		sprintf(libn, "main/game.mp.i386.so");
	else
		sprintf(libn, "%s/game.mp.i386.so", check);
	//char* libn = "/home/cod/codextended/game.mp.i386.so";
	int unprotect_lib(char *libname);
	unprotect_lib(libn);
	
	//mprotect(ret, 0x8A400, PROT_READ | PROT_WRITE | PROT_EXEC);
	gamelib = ret;
	base = (int)dlsym(ret, "vmMain"); //0x4D84C
	g_entities = (gentity_t*)dlsym(ret, "g_entities");
	level = (level_locals_t*)GAME("level");
	pml = (char (*)[140])dlsym(ret, "pml");
	pm = (pmove_t*)dlsym(ret, "pm");
	
	void BG_Link();
	BG_Link();
	set_trap_func_ptr();
	scriptInitializing();
	
	int stuck = (int)dlsym(ret, "StuckInClient");
	__jmp(stuck, (int)StuckInPlayer);
	
	int cont = (int)dlsym(ret, "G_SetClientContents");
	__jmp(cont, (int)G_SetPlayerContents);
	
	
	int h66 = (int)dlsym(ret, "ClientEndFrame") + 0x173; //patch contents
	__nop(h66, h66+0xa);
	
	#if 0
	{
		//ClientCommand+54F  24FC                mov     eax, [esi+158h]
		unsigned off = (unsigned)(dlsym(gamelib, "ClientCommand") + 0x54f);
		unsigned end = (unsigned)(dlsym(gamelib, "ClientCommand") + 0x5B4 + 2);
		
		__nop(off, end - off);
		__call(off, 
	}
	#endif
	
	//int h93 = GAME("PM_Weapon")+0x63;
	//__nop(h93,h93+5);
	
	__call(GAME("ClientCommand")+0x62D, (int)Cmd_CallVote);
	
	void ClientBegin(int);
	__call(GAME("vmMain")+0xA0, (int)ClientBegin);
	
	#if xDEBUG
	/*
		aim in air if client allows it
		maybe add a groundEntityNum = 1023; force???
		- Richard
	*/
	//__jmp( dlsym(gamelib, "PM_UpdateAimDownSightFlag"), _PM_UpdateAimDownSightFlag);
	int thk = GAME("PmoveSingle");
	__call(thk + 0x3cc, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x3ea, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x404, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x441, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x49e, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x4d7, _PM_UpdateAimDownSightFlag);
	__jmp( GAME("PM_ClearAimDownSightFlag"), _PM_ClearAimDownSightFlag);
	#endif
	
	/*
		Newline, carriage return say fix.
	*/
	
	int clientcommand_off = GAME("ClientCommand");
	
	__call(clientcommand_off + 0x6EE, (int)hG_Say);
	__call(clientcommand_off + 0x6FE, (int)hG_Say);
	
	int g_say_off = GAME("G_Say");
	__call(g_say_off + 0x5EA, (int)hG_Say);
	__call(g_say_off + 0x791, (int)hG_Say);
	__call(g_say_off + 0x77D, (int)hG_Say);
	
	/*
		only one left to patch should be 
			
			Cmd_GameCommand_f+84 call    G_Say           ; Call Procedure
			Cmd_GameCommand_f+94 call    G_Say           ; Call Procedure
			
		since we're already ignoring the 'gc' command no need to
	*/
	
	/*
		End of fix
			- Richard
		//i should add more comments to make things more clearer for me when i look at the code later again. :3
	*/
	
	/*
		q3msgboom || codmsgboom fix
	*/
	int q21 = GAME("G_Say")+0x50e;
	int q22 = GAME("G_Say")+0x5ca;
	*(int*)q21=0x37f;
	*(int*)q22=0x37f;
	
	/*
		end of fix
	*/
	
	x_deadchat = Cvar_Get("x_deadchat", "1", 0);
	
	//deadchat fix
	int b2 = GAME("G_SayTo")+0x70;
	__nop(b2, b2+2);
	
	int b3 = GAME("G_Say")+0x3B6;
	int b4 = GAME("G_Say")+0x2B3;
	*(byte*)b3 = 0xeb;
	*(byte*)b4 = 0xeb;
	
	//end deadchat fix
	
	/*
		Spectator noclip
	*/
	
	if(x_spectator_noclip->integer) {
		int y7 = GAME("SpectatorThink")+0x123;
		*(int*)y7 = 0;
	}
	
	#if 0
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
	#endif
	
	//int b5 = GAME("PmoveSingle");
	//__jmp(b5+0x404, b5+0x3ea);
	
	#if 0
	#define DB_SERVER "localhost"
	#define DB_USER "root"
	#define DB_DATABASE "cod1"
	#endif
	cvar_t* db_password = Cvar_Get("db_password","",0);
	cvar_t* db_username = Cvar_Get("db_username","root",0);
	cvar_t* db_database = Cvar_Get("db_database","",0);
	cvar_t* db_server = Cvar_Get("db_server","localhost",0);
	
	#ifdef uMYSQL
	static int sql_do_once = 0;
	
	if(!sql_do_once) {
		if(mysql_real_connect(db,db_server->string,db_username->string,db_password->string,db_database->string,0,NULL,0) == NULL) {
			printf("Could not connect to the MySQL Database. [Error: %s]\n", mysql_error(db));
			//COD_Destructor();
			mysql_close(db);
		} else {
			printf("Connected to the MySQL Database.\n");
		}
		sql_do_once = 1;
	}
	#endif
	
	return ret;
}