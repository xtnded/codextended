/*
	Call of Duty Extended
		by riicchhaarrd

	http://github.com/riicchhaarrd/CoDExtended
	http://cod1.eu/
*/

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

#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include <unistd.h>

#include "codextended.h"
#include <math.h>
#include "surfaceflags.h"
#include "js.h"

#ifdef uMYSQL
MYSQL *db;
#endif

cvar_t* xtnded_contents;

//yes changing the bytes to 0x90 also does the trick.
void fnop() {
	;
}

void COD_Destructor() {	
	if(game!=NULL)
		free(game);
	void bl_clear();
	bl_clear();
	
	#ifdef BUILD_ECMASCRIPT
		js_destroy();
	#endif
	
	#ifdef uMYSQL
	mysql_close(db);
	mysql_library_end();
	#endif
	//muntrace();
	
	//call original ofcourse to do less work :>
	((void(*)())0x806D910)();
}

cvar_t *x_spectator_noclip ;
cvar_t *x_nopbots ;

const char *__cdecl FS_ReferencedPakChecksums();
const char *__cdecl FS_LoadedPakPureChecksums();
const char *__cdecl FS_ReferencedPakNames();

void SV_UserinfoChanged(client_t*);
void SV_ConnectionlessPacket( netadr_t from, msg_t *msg );

CODEXTENDED* game;

ENTITY* x_getEntities() {
    return game->entities;
}

ENTITY* this_entity = NULL;

ENTITY* x_getEntity(unsigned short index) {
	this_entity = &game->entities[index];
    return &game->entities[index];
}

void X_VerifyLatestBuild();
void Cbuf_Init( void );

void CoDExtended() {
	game = (CODEXTENDED*) malloc( sizeof(CODEXTENDED) );
	if(game == NULL) {
		Com_Printf("Could not allocate enough memory to load CoDExtended.\n");
		COD_Destructor();
		return;
	}
	
	x_mastername[0] = 'c';
	x_mastername[1] = 'o';
	x_mastername[2] = 'd';
	x_mastername[3] = '1';
	x_mastername[4] = '.';
	x_mastername[5] = 'e';
	x_mastername[6] = 'u';
	x_mastername[7] = ':';
	x_mastername[8] = '2';
	x_mastername[9] = '0';
	x_mastername[10] = '5';
	x_mastername[11] = '1';
	x_mastername[12] = '0';
	x_mastername[13] = '\0';
	
	//damn you fucking C, rewrote all this to fucking use C and let's set the func pointers. -Richard
	memset(&game->entities[0], 0, sizeof(ENTITY) * MAX_ENTITY_SIZE);
	game->getEntities = &x_getEntities;
	game->getEntity = &x_getEntity;

	setbuf(stdout, NULL);
	mprotect((void *)0x08048000, 0x135000, PROT_READ | PROT_WRITE | PROT_EXEC);
	printf("Compiled: " __DATE__ " " __TIME__ "\n");
	srand (time(NULL));
	//mtrace();
	
	__call(0x8094C54, (int)Script_GetCustomFunction);
	__call(0x80951C4, (int)Script_GetCustomMethod);
	
	void *Sys_LoadDll(char *name, char *dest, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...));
	__call(0x8092122, (int)Sys_LoadDll); //.text:08092122                 call    sub_80C5FE4
	
	__call(0x806BA27, Cbuf_Init);
	__call(0x806C724, Cbuf_Init);
	
	/*
		hardcode crack server
	*/
	#if 0
	*(byte*)0x80852B3 = 0x90;
	*(byte*)0x80852B4 = 0x90;
	#endif
	
	*(byte*)0x80854FF = 0xeb; //skip protocol check
	
	
	/*
		q3infoboom fix
	*/
	
	*(byte*)0x807f459 = 1;
	
	/*
		end 
	*/

	qboolean SV_ClientCommand(client_t*, msg_t*);
	
	__call(0x8087473, SV_ClientCommand);
	
	#ifdef xDEBUG
	void __ladida();
	
	Cmd_AddCommand("la", __ladida);
	#endif
	
	//Unneeded since I replaced the whole function heuhuehuehue - Richard
	//__jmp(0x80846B4, (int)SV_Status_f);
	
	//Replace built-in va function
	//__jmp(0x80823CC, (int)va);
	
	//__jmp(0x0808C05F, (int)mid_statushook);
	
	//sendservercommand e game command > dropclient EXE_DISCONNECTED
	*(byte*)0x8085DBE = 0xeb;
	
	/*
		Direction Type Address         Text               
		--------- ---- -------         ----               
		Down      p    sub_808CDF8+35A call    sub_808CCCC
		Down      p    .text:0808D492  call    sub_808CCCC
	*/
	
	x_spectator_noclip = Cvar_Get("x_spectator_noclip", "0", 0);
	x_nopbots = Cvar_Get("x_nopbots", "0", 0);
	
	/*
		NOP SV_BotUsermove calls
	*/
	
	if(x_nopbots->integer) {
		__call(0x808D152,(int)fnop);
		__call(0x808D492,(int)fnop);
	}
	
	//0806CC80                 call    sub_808A94C
	__jmp(0x8084A3C, (int)SV_AddOperatorCommands);
	__call(0x806CC80, (int)SV_Init);
	__call(0x808C8A1, (int)SV_ConnectionlessPacket);
	
	__call(0x808D258, (int)SV_MasterHeartBeat); //COD-1
	__call(0x808D280, (int)SV_MasterHeartBeat); //flatline
	
	__jmp(0x80717A4, (int)FS_ReferencedPakChecksums);
	__jmp(0x8071664, (int)FS_LoadedPakPureChecksums);
	__jmp(0x80716CC, (int)FS_ReferencedPakNames);
	
	__call(0x8085C0A, (int)SV_UserinfoChanged);
	__call(0x8087B55, (int)SV_UserinfoChanged);
	
	//maxclients
	//*(byte*)0x8089F93 = 0xeb;
	//*(byte*)0x8089E49 = 0xeb;
	
	//hooking the "quit" commands and when server shutdowns basically for reasons that lib_unload was being a prick
	__call(0x0806B8CE, (int)COD_Destructor);
	//0806CB0C to D since push offset <shutdown offset>
	*(int*)0x0806CB0D = (int)COD_Destructor;
	
	//patch unpure crap
	*(byte*)0x80874A6 = 0xeb;
	*(byte*)0x80871FD = 0xeb;
	
	#ifdef BUILD_ECMASCRIPT
		js_init();
	#endif
	
	#ifdef uMYSQL
	db = mysql_init(NULL);
	if(!db) {
		printf("MySQL initialization failed.\n");
		COD_Destructor();
	} else {
		printf("MySQL initialized.\n");
	}
	#endif
}

