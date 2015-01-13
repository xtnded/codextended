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
/*
	Call of Duty Extended
		by riicchhaarrd

	http://github.com/riicchhaarrd/CoDExtended
	http://cod1.eu/
*/
	
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <execinfo.h>
#include "server.h"
#include "surfaceflags.h"
#include "js.h"

#ifdef uMYSQL
MYSQL *db;
#endif

void crash_handler(int sig) {
	void *array[10];
	size_t size = backtrace(array, 10);
	
	fprintf(stderr, "Error: signal: %x {%d}\n", sig, sig);
	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(1);
}

void crashtest() {
	*(char*)0 = 0;
}

void changemode(int dir) {
	static struct termios oldt, newt;
	
	if (dir == 1) {
		tcgetattr( STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~( ICANON | ECHO );
		tcsetattr( STDIN_FILENO, TCSANOW, &newt);
		return;
	}
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}

int kbhit() {
	struct timeval tv;
	fd_set rdfs;
	
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	
	FD_ZERO(&rdfs);
	FD_SET (STDIN_FILENO, &rdfs);
	
	select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &rdfs);
}

#include "ascii_include.txt"
//xxd -i ascii_1.txt
//can be max 24 lines and 79 characters wide?

int main(int argc, char **argv) {
	if(GENTITY_SIZE != sizeof(gentity_t)) {
		printf("Error! gentity_t is not the same size as GENTITY_SIZE!\n");
		COD_Destructor();
		return;
	}
	#define RAND_COL_TEXT (31 + (rand() % 6))
	#define RAND_COL_BG (40 + (rand() % 7))
	int c, a;
	changemode(1);
	while(1) {
		if(kbhit()) {
			c = getchar();
			changemode(0);
			break;
		}
		printf("\x1B[2J\x1B[H");
		c = RAND_COL_BG;
		a = RAND_COL_TEXT;
		while(c-10 == a)
			a = RAND_COL_TEXT;
		printf("\e[%dm\e[1;%dm%s\n", c, a, ascii_txt);
		
		printf("\e[44m\e[1;36m[ Press ENTER to continue or type 'q' to exit ]\n");
		cprintf(PRINT_UNDERLINE | PRINT_BOLD, "Compile date: " __DATE__ " " __TIME__ "\n");
		usleep(500000 * 2);
	}
	printf("\e[0m\n");
	
	if(c == 'q') {
		COD_Destructor();
		return;
	} else if(c == 'c') {
		crashtest();
	}
	
	return ((int(*)(int,char**))0x80C6870)(argc, argv);
}

void COD_Destructor() {
	#if PATCH == 1
	void bl_clear();
	bl_clear();
	#endif
	
	#if PATCH == 1
	
	#ifdef BUILD_ECMASCRIPT
		js_destroy();
	#endif
	
	#endif
	
	#ifdef uMYSQL
	if(db != NULL)
		mysql_close(db);
	mysql_library_end();
	#endif
	//muntrace();
	
	//call original ofcourse to do less work :>
	#if PATCH == 1
	((void(*)())0x806D910)();
	#endif
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
	//printf("ret = %s\n", ret);
	return ret;
}

const char *__cdecl FS_ReferencedPakChecksums();
const char *__cdecl FS_LoadedPakPureChecksums();
const char *__cdecl FS_ReferencedPakNames();

void SV_UserinfoChanged(client_t*);
void SV_ConnectionlessPacket( netadr_t from, msg_t *msg );

void Cbuf_Init( void );

void CoDExtended() {
	signal(SIGSEGV, crash_handler);

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

	setbuf(stdout, NULL);
	mprotect((void *)0x08048000, 0x135000, PROT_READ | PROT_WRITE | PROT_EXEC);
	
	*(int*)0x804A698 = (int)main;
	
	srand (time(NULL));
	//mtrace();
	
	#if PATCH == 1
	__call(0x8094C54, (int)Scr_GetCustomFunction);
	__call(0x80951C4, (int)Scr_GetCustomMethod);
	
	void *Sys_LoadDll(char *name, char *dest, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...));
	__call(0x8092122, (int)Sys_LoadDll); //.text:08092122                 call    sub_80C5FE4
	
	__call(0x806BA27, Cbuf_Init);
	__call(0x806C724, Cbuf_Init);
	
	SVC_CHANDELIER[0] = 'z';
	SVC_CHANDELIER[1] = 'c';
	SVC_CHANDELIER[2] = 'h';
	SVC_CHANDELIER[3] = 'a';
	SVC_CHANDELIER[4] = 'n';
	SVC_CHANDELIER[5] = 'd';
	SVC_CHANDELIER[6] = 'e';
	SVC_CHANDELIER[7] = 'l';
	SVC_CHANDELIER[8] = 'i';
	SVC_CHANDELIER[9] = 'e';
	SVC_CHANDELIER[10] = 'r';
	SVC_CHANDELIER[11] = '\0';
	
	//xor_crypt(x_secret_salt, SVC_CHANDELIER, strlen(SVC_CHANDELIER));
	
	/*
		hardcode crack server
	*/
	#if 0
	*(byte*)0x80852B3 = 0x90;
	*(byte*)0x80852B4 = 0x90;
	#endif
	
	*(byte*)0x80854FF = 0xeb; //skip protocol check
	
	
	//jz      short loc_808C42E (patch rcon time)
	*(unsigned char*)0x808C41F = 0xeb;
	
	/*
		q3infoboom fix
	*/
	
	*(byte*)0x807f459 = 1;
	
	/*
		end 
	*/

	qboolean SV_ClientCommand(client_t*, msg_t*);
	
	__call(0x8087473, SV_ClientCommand);
	
	/*
	Direction Type Address         Text               
--------- ---- -------         ----               
Up        p    sub_806EA34+137 call    sub_806ECD4
Down      p    sub_806F0B0+13  call    sub_806ECD4
Down      p    .text:0806F0DF  call    sub_806ECD4
Down      p    sub_806F0E8+77  call    sub_806ECD4
Down      p    sub_806F16C+11  call    sub_806ECD4
Down      p    .text:0806F26B  call    sub_806ECD4
Down      p    sub_806F2E0+B3  call    sub_806ECD4
Down      p    sub_806F2E0+139 call    sub_806ECD4
Down      p    sub_806F428+F5  call    sub_806ECD4
Down      p    sub_806F97C+7B  call    sub_806ECD4
Down      p    sub_806FE04+55  call    sub_806ECD4
*/
	cvar_t *Cvar_Set2( const char *var_name, const char *value, qboolean force );
	__call(0x806FE04+0x55, Cvar_Set2);
	__call(0x806F97C+0x7B, Cvar_Set2);
	__call(0x806F428+0xF5, Cvar_Set2);
	__call(0x806F2E0+0x139, Cvar_Set2);
	__call(0x806F2E0+0xB3, Cvar_Set2);
	__call(0x806F26B, Cvar_Set2);
	__call(0x806F16C+0x11, Cvar_Set2);
	__call(0x806F0E8+0x77, Cvar_Set2);
	__call(0x806F0DF, Cvar_Set2);
	__call(0x806F0B0+0x13, Cvar_Set2);
	__call(0x806EA34+0x137, Cvar_Set2);
	
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
	
	
	//0806CC80                 call    sub_808A94C
	__jmp(0x8084A3C, (int)SV_AddOperatorCommands);
	__call(0x806CC80, (int)SV_Init);
	__call(0x808C8A1, (int)SV_ConnectionlessPacket);
	void SV_PacketEvent(netadr_t,msg_t*);
	__call(0x806BFEC, (int)SV_PacketEvent);
	__call(0x806C1B8, (int)SV_PacketEvent);
	__call(0x806E15C, (int)SV_PacketEvent);
	
	qboolean    Sys_GetPacket( netadr_t *net_from, msg_t *net_message );
	__jmp(0x80C71F0, (int)Sys_GetPacket);
	
	__call(0x808D258, (int)SV_MasterHeartBeat); //COD-1
	__call(0x808D280, (int)SV_MasterHeartBeat); //flatline
	
	__jmp(0x80717A4, (int)FS_ReferencedPakChecksums);
	__jmp(0x8071664, (int)FS_LoadedPakPureChecksums);
	__jmp(0x80716CC, (int)FS_ReferencedPakNames);
	
	/*
	__jmp(0x8071600, (int)FS_ReferencedPakChecksums); //FS_LoadedPakChecksums
	__jmp(0x8071580, (int)FS_ReferencedPakNames); //FS_LoadedPakNames
	*/
	
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
	
	void test_debug();
	test_debug();
	
	#else
	
		/*
			//TODO
			* merge 1.5 with 1.1
		*/
		__call(0x80713BF, (int)SV_Init);
		//__call(0x8093AD7, (int)SV_ConnectionlessPacket);
		
		/* Hardcode crack 1.5 */
		__nop(0x808966B, 6);
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

