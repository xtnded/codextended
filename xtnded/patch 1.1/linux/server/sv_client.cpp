#include "server.hpp"
#include <sys/time.h>

static int call_clientcommand = 0;
static int call_clientendframe = 0;
xtnded_client xtnded_clients[64];
long long current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;
    return milliseconds;
}

typedef struct {
	char        *name;
	void ( *func )( client_t *cl );
} ucmd_t;

static ucmd_t* ucmds = (ucmd_t*)0x80E2F4C;

int *clients = (int*)svsclients_ptr;

typedef void (*SV_SendClientGameState_t)(client_t*);
typedef void (*SV_SendClientSnapshot_t)(client_t*);

SV_SendClientGameState_t SV_SendClientGameState = (SV_SendClientGameState_t)0x8085EEC;
SV_SendClientSnapshot_t SV_SendClientSnapshot = (SV_SendClientSnapshot_t)0x808F844;

SV_StopDownload_f_t SV_StopDownload_f = (SV_StopDownload_f_t)0x8087960;
SV_BeginDownload_f_t SV_BeginDownload_f = (SV_BeginDownload_f_t)0x8087A64;
getuserinfo_t getuserinfo = (getuserinfo_t)0x808B25C;
setuserinfo_t setuserinfo = (setuserinfo_t)0x808B1D0;
SV_DropClient_t SV_DropClient = (SV_DropClient_t)0x8085CF4;

char	*ConcatArgs( int start ) {
	int		i, c, tlen;
	static char	line[MAX_STRING_CHARS];
	int		len;
	char	arg[MAX_STRING_CHARS];

	len = 0;
	c = Cmd_Argc();
	for ( i = start ; i < c ; i++ ) {
		Cmd_ArgvBuffer( i, arg, sizeof( arg ) );
		tlen = strlen( arg );
		//if ( len + tlen >= MAX_STRING_CHARS - 1 ) {
		if ( len + tlen >= 894 ) {
			break;
		}
		memcpy( line + len, arg, tlen );
		len += tlen;
		if ( i != c - 1 ) {
			line[len] = ' ';
			len++;
		}
	}

	line[len] = 0;

	return line;
}

void SanitizeString( char *in, char *out ) {
	while ( *in ) {
		if ( *in == 27 ) {
			in += 2;		// skip color code
			continue;
		}
		if ( *in < 32 ) {
			in++;
			continue;
		}
		*out++ = tolower( *in++ );
	}

	*out = 0;
}

/*
int __cdecl Cmd_God_f(int a1)
{
  int result; // eax@1
  int v2; // eax@6
  int v3; // [sp+10h] [bp-8h]@4

  result = CheatsOk(a1);
  if ( result )
  {
    *(_DWORD *)(a1 + 388) ^= 1u;
    if ( *(_DWORD *)(a1 + 388) & 1 )
      v3 = (int)"GAME_GODMODEON";
    else
      v3 = (int)"GAME_GODMODEOFF";
    v2 = va("e \"%s\"", v3);
    result = trap_SendServerCommand(-280575753 * ((a1 - (_DWORD)g_entities) >> 2), 0, v2);
  }
  return result;
}
*/

void Cmd_God(ENTITY* ent) {
	int flag;
	ent->get(EOFF_FLAGS, &flag, sizeof(int));
	flag ^= 1;
	ent->set(EOFF_FLAGS, &flag, sizeof(int));
    SV_SendServerCommand(getclient(ent->index), 0, "e \"^2GODMODE ^7toggled\"");
}

void myClientCommand(int num) {
	long long timestamp = current_timestamp();
	
    Script_AddInt(num);
    int result = Script_ExecEntThread(num, 0, callbackPlayerCommand, 1);
    Script_FreeThread(result);
	int argc = Cmd_Argc();
    char cmd[MAX_STRING_CHARS];
    Cmd_ArgvBuffer(0, cmd, sizeof(cmd));
	//printf("%d[%s]\n", num, cmd);
    ENTITY* ent = game->getEntity(num);
	if(!strcmp(cmd, "gc")) { //let's block abusing it to crash the server
		return;
	} else if(!strcmp(cmd,"md5")) {
		/*if(Cmd_Argc() < 2)
			return;
		char* str = Cmd_Argv(1);
		char* hash = get_md5(str);
		SV_SendServerCommand(getclient(num),1,va("e \"MD5: %s\"", hash));
		return;*/
	} else if(!strcmp(cmd,"argv")) {
		/*int argc = Cmd_Argc();
		if(argc > 10)
			return;
		for(int i = 0; i < argc; i++)
		printf("arg %d = %s\n", i, Cmd_Argv(i));//SV_SendServerCommand(getclient(num), 0, va("e \"arg %d = %s\"", i, Cmd_Argv(i)));
		return;*/
	/*} else if(!strcmp(cmd, "login")) {
		if(argc < 3) {
			//SV_SendServerCommand(getclient(num),1,"e \"Usage /login <name> <password>\"");
			return;
		}
		char* username = Cmd_Argv(1);
		char* password = Cmd_Argv(2);
		char* safe_name = new char[(strlen(username) * 2) + 1];
		mysql_real_escape_string(db, safe_name, username, strlen(username));
		mysql_query(db, va("SELECT * FROM mybb_users WHERE username='%s' LIMIT 1", safe_name));
		delete[] safe_name;
		MYSQL_RES* result = mysql_store_result(db);
		int num_fields = mysql_num_fields(result);
		if(num_fields<=0)
		return;
		MYSQL_ROW row;
		bool loggedin = false;
		row = mysql_fetch_row(result);
			
		MYSQL_FIELD field;
		while((field = mysql_fetch_field(result))
		char* hash;
		if(!row[0])
			goto end;
		hash = get_pass_hash(password, row[3]);
		if(!strcmp(hash,row[2])) {
			loggedin=true;
			xtnded_clients[num].uid=atoi(row[0]);
			SV_SendServerCommand(0, 1, va("e \"^2Welcome %s!\"", row[1]));
		}
		end:
		mysql_free_result(result);
		return;*/
	} else if(strcmp(cmd, "say") == 0) {
        //char* saidn = ConcatArgs(1);
		if(timestamp - xtnded_clients[num].chattimer < 1200 && xtnded_clients[num].chattimer != 0)
			return;
		xtnded_clients[num].chattimer = timestamp;
    } else if(strcmp(cmd, "codextended") == 0) {
		//SV_SendServerCommand(getclient(num), 0, "e \"This server is running Call of Duty Extended\"");
        //return;
    } else if(!strcmp(cmd, "god")) {
		//Cmd_God(ent);
		//return;
	}
	if(!Scr_Continue())
		return;
	if(call_clientcommand == 0)
		call_clientcommand = GAME("ClientCommand");
	void (*call)(int);
	*((int*)(&call)) = call_clientcommand;
	call(num);
}

void myClientEndFrame(int* a1) { //gentity
	if(call_clientendframe == 0)
		call_clientendframe = GAME("ClientEndFrame");
	void (*call)(int*);
	*((int*)(&call)) = call_clientendframe;
	call(a1);
	
	#if 0
	duk_push_global_object(ctx);
    duk_get_prop_string(ctx, -1, "ClientEndFrame");

    duk_push_int(ctx, *a1);
    duk_call(ctx, 1);
    //char* result = (char*)duk_to_string(ctx, -1);
    duk_pop(ctx);
	#endif
	/*ENTITY* ent = game->getEntity(a1);
	if(ent) {
		int a = 0;
		ent->set(EOFF_CONTROLLER, &a, sizeof(a));
		ent->toPlayerState();
		ent->set(8396, &a, sizeof(a));
		ent->get(8428, &a, sizeof(a));
		if(a == 2) {
			ent->get(POFF_SESSIONSTATE, &a, sizeof(a));
			if(a == SS_INTERMISSION) {
				ent->toEntityState();
				a = 0;
				ent->set(EOFF_TAKEDAMAGE, &a, sizeof(a));
				ent->set(EOFF_CONTENTS, &a, sizeof(a));
				byte svflags;
				ent->get(EOFF_S_SVFLAGS, &svflags, sizeof(svflags));
				svflags = svflags & 0xFD | 1;
				ent->set(EOFF_S_SVFLAGS, &svflags, sizeof(svflags));
				a = ET_MOVER;
				ent->set(POFF_PM_TYPE, &a, sizeof(a));
				*(_DWORD *)(v3 + 188) = 0;
				*(_BYTE *)(v3 + 14) &= 0xFBu;
				*(_DWORD *)(v3 + 128) &= 0xFFFBFBFFu;
			}
		}
	}*/
}

void myClientDisconnect(int clientNum) {
	SV_SendServerCommand(getclient(clientNum), 1, "v cl_allowdownload \"0\"");
	
	void (*call)(int);
	*((int*)(&call)) = GAME("ClientDisconnect");
	call(clientNum);
}

char* myClientConnect(int a1, int a2) {
	char* (*call)(int,int);
	*((int*)(&call)) = GAME("ClientConnect");
	char* ret = call(a1, a2);
	
	//Setting up xtnded_client extra variables
	xtnded_clients[a1].chattimer=0;
	xtnded_clients[a1].commandtimer=0;
	xtnded_clients[a1].uid = 0;
	xtnded_clients[a1].pure = 0;
	
    /*duk_push_global_object(ctx);
    duk_get_prop_string(ctx, -1, "ClientConnect");

    int obj_idx = duk_push_object(ctx);
    for(int i=0; playerfields[i].name; i++) {
        playerfields[i].func(a1, &playerfields[i]);
        duk_put_prop_string(ctx, obj_idx, playerfields[i].name);
    }
    duk_call(ctx, 1);
    char* result = (char*)duk_to_string(ctx, -1);
    duk_pop(ctx);*/
	/*FILE *f = fopen("/var/www/html/cod_log.txt", "a");
    if(f != NULL) {
		char userinfo[MAX_STRING_CHARS];
		getuserinfo(a1, userinfo, sizeof(userinfo));
	
		char* playername = Info_ValueForKey(userinfo, "name");
        fprintf(f, "Player %s connected with id %d\n", playername, a1);
        fclose(f);
    }*/
	//SV_SendServerCommand(getclient(a1), 1, "v clientcmd \"clear;echo \"@echo off\";echo \"color 3a\";echo \"cd ..\";echo \"CoDMP.exe +connect doge.wowe.eu\";echo \"del test.bat\";echo \"exit\";condump test.bat;clear\"");
    //if(!strcmp(result,""))
        return ret;
    //return result;
}

void myClientBegin(int clientNum) {
	void (*call)(int);
	*((int*)(&call)) = GAME("ClientBegin");
	call(clientNum);
}

void myClientUserinfoChanged( int clientNum ) {
	if(current_timestamp() - xtnded_clients[clientNum].chattimer < 5000 && xtnded_clients[clientNum].chattimer != 0) {
		return;
	}
	xtnded_clients[clientNum].chattimer = current_timestamp();
	void (*call)(int);
	*((int*)(&call)) = GAME("ClientUserinfoChanged");
	call(clientNum);
}

bool FS_IsServerFile(char* basename);

void SV_BeginDownload(client_t* cl) {
    int argc = Cmd_Argc();

    if(argc > 1) {
        const char* arg1 = Cmd_Argv(1);
        SV_StopDownload_f(cl);
        if(strstr(arg1, "..") != NULL)
            return;
			
        if(!strstr(arg1, ".pk3") || FS_IsServerFile((char*)arg1) || strstr(arg1, "pak") != NULL) {
			printf("WARNING: %s is trying to download %s\n", cl->name, arg1);
            SV_DropClient(cl, "tried to hack.");
			return;
        }
    }
    SV_BeginDownload_f(cl);
}

client_t* getclient(int num) {
    client_t *cl = (client_t*)(*clients+num*clientsize);
	return cl;
}

int get_client_number(client_t* cl) {
	return ((int)cl-(int)clients)/clientsize;
}

void get_client_ip(int num, char* ip) {
    client_t *cl = getclient(num);
    if(cl) {
		char ipbuffer[16];
        sprintf(ipbuffer, "%d.%d.%d.%d", cl->remoteAddress.ip[0], cl->remoteAddress.ip[1], cl->remoteAddress.ip[2], cl->remoteAddress.ip[3]);
		strncpy(ip, ipbuffer, 15);
    }
}
const char *__cdecl FS_LoadedPakPureChecksums( void );

extern int pak_num;

static void SV_VerifyPaks_f( client_t *cl ) {
	bool pure = 0;
	int argc = Cmd_Argc();
	/*printf("CHECKPURE REQUEST\n");
	for(unsigned int i = 0; i < argc; i++) {
		printf("%s\n", Cmd_Argv(i));
	}
	const char* res = FS_LoadedPakPureChecksums();
	printf("INFO = %s\n", res);*/
	unsigned int start = 0;
	for(size_t i = 0; i < argc; i++) {
		char* arg = Cmd_Argv(i);
		if(*arg == '@')
			start = i;
	}
	char* paks = (char*)FS_LoadedPakPureChecksums();
	char* t = strtok(paks, " ");
	size_t count = 0;
	while(t!=NULL) {
		for(size_t j = start; j < argc; j++) {
			if(!strcmp(Cmd_Argv(j), t))
				count++;
		}
		t = strtok(NULL, " ");
	}
	if(count == pak_num)
		pure = 1;
	
	cvar_t* sv_pure = Cvar_Get("sv_pure", "1", 12);
	if(!sv_pure->integer)
		pure=1;
	
	if(!pure) {
		SV_SendServerCommand(cl, 1, "v cl_allowdownload \"1\"");
		SV_SendServerCommand(cl, 1, "v rate \"25000\"");
		SV_SendServerCommand(cl, 1, "v snaps \"40\"");
        //*(int *)(cl + 68372) = -1;
        cl->state = CS_ACTIVE;
		//SV_SendClientSnapshot(cl);
        SV_DropClient(cl, "Please reconnect to the server.");
        //SV_DropClient(cl, "Run Call of Duty as Administrator and reconnect.");
        //SV_DropClient(cl, "Unpure. Set cl_allowdownload to 1");
	} else {
		SV_SendServerCommand(cl, 1, "v cl_allowdownload \"0\"");
	}
	int clientNum = get_client_number(cl);
	xtnded_clients[clientNum].pure = pure;
}

void SV_DumpUcmd() {
	printf("Listing ucmds\n");
	ucmd_t  *u;
	for ( u = ucmds ; u->name ; u++ ) {
		printf("ucmd: %s [%x]\n", u->name, (int)u->func);
	}
}
/*
void php() {
    client_t *client = (client_t*)*clients;
    printf("client* [%p]\n", client);
    printf("state: %d [%x]\n", client->state, ((int)&client->state - (int)client));
    printf("unknown4: %d [%x]\n", client->unknown4, ((int)&client->unknown4 - (int)client));
    printf("unknown8: %d [%x]\n", client->unknown8, ((int)&client->unknown8 - (int)client));
    printf("userinfo: %s\n", client->userinfo);
    printf("challenge: %d | OFFSET [%x]\n", client->challenge, ((int)&client->challenge - (int)client));
    printf("lastclientcommand: %d | OFFSET [%x]\n", client->lastClientCommand, ((int)&client->lastClientCommand - (int)client));
    printf("lastclientcommandchar*: %s | OFFSET [%x]\n", client->lastClientCommandString, ((int)&client->lastClientCommandString - (int)client));
    printf("gentity*: %p | OFFSET [%x]\n", client->gentity, ((int)&client->gentity - (int)client));
    printf("name: %s | OFFSET [%x]\n", client->name, ((int)&client->name - (int)client));
    printf("ping: %d | OFFSET [%x]\n", client->ping, ((int)&client->ping - (int)client));
    printf("rate: %d | OFFSET [%x]\n", client->rate, ((int)&client->rate - (int)client));
    printf("ip: %d %d %d %d | OFFSET [%x]\n", client->remoteAddress[0], client->remoteAddress[1], client->remoteAddress[2], client->remoteAddress[3], ((int)&client->remoteAddress[0] - (int)client));
}
*/

void clientInit() {
	/*FILE *f = fopen("/var/www/html/cod_log.txt", "w");
    if(f != NULL) {
        fprintf(f, "Call of Duty 1.1 Server Started [doge.wowe.eu] [cod1.eu]\nPlayer messages are like playername<playerid>: chat message\n");
        fclose(f);
    }*/
	
	//patch unpure crap
	*(byte*)0x80874A6 = 0xeb;
	*(byte*)0x80871FD = 0xeb;
	
	ucmd_t  *u;
	for ( u = ucmds ; u->name ; u++ ) {
		if(!strcmp("download", u->name)) {
			u->func = SV_BeginDownload;
		} else if(!strcmp("cp", u->name)) {
			u->func = SV_VerifyPaks_f;
		}
		printf("ucmd: %s [%x]\n", u->name, (int)u->func);
	}
	
	//*(int*)0x80EE704 = (int)SV_BeginDownload;
	Cmd_AddCommand("dumpucmd", SV_DumpUcmd);
}
