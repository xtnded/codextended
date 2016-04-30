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

#include "server.h"

/*
==================
SV_Version	
==================
*/
void SV_Version() {
	//printf(VERSION_STRING, EXTENDEDVERSION);
	#ifdef xDEBUG
	printf("Call of Duty Extended Developer v%d by riicchhaarrd\nA lot of thanks to kungfooman and libcod\n", CURRENTBUILD);
	#else
	printf("Call of Duty Extended v%d by riicchhaarrd\nA lot of thanks to kungfooman and libcod\n", CURRENTBUILD);
	#endif
}

extern challenge_t* challenges;

void SV_XStatus_f() {
	if(!sv_running->integer) {
		Com_Printf("Server is not running.\n");
		return ;
	}

	Com_Printf( "map: %s\n", mapname->string );
	
	Com_Printf( "num mUID                             name            lastmsg ip                    qport rate  ping\n" );
	Com_Printf( "--- -------------------------------- --------------- ------- --------------------- ----- ----- ----\n" );
	
	int i , j, l;
	const char* s;
	challenge_t* challenge;
	int guid ;
	int qport;
	int ping;
	client_t* cl;
	for(i = 0; i < sv_maxclients->integer; i++) {
		cl = getclient(i);
		if(!cl)
			continue;
		if(!cl->state)
			continue;
			
		Com_Printf( "%3i ", i ); //num	
		if(*xclients[i].mUID)
			Com_Printf("%.32s ", xclients[i].mUID); //mUID
		else
		{
			for(int k = 0; k != 33; k++)
				Com_Printf(" ");
		}
		
		Com_Printf( "%.15s", cl->name );
		l = 16 - strlen( cl->name );
		for ( j = 0 ; j < l ; j++ )
			Com_Printf( " " );
		
		int tt = *(int*)0x83B67A4 - *(int *)((int)cl + 68364);
		Com_Printf( "%7i ", tt );
		
		s = NET_AdrToString( cl->netchan.remoteAddress );
		Com_Printf("%s", s );
		l = 22 - strlen( s );
		for ( j = 0 ; j < l ; j++ )
			Com_Printf( " " );
	
		Com_Printf("%5i ", cl->netchan.qport );
		Com_Printf("%5i ", cl->rate );
		
		if ( cl->state == CS_CONNECTED ) {
			Com_Printf( "CNCT " );
		} else if ( cl->state == CS_ZOMBIE ) {
			Com_Printf( "ZMBI " );
		} else
		{
			ping = cl->ping < 9999 ? cl->ping : 9999;
			Com_Printf( "%4i", ping );
		}

	
		Com_Printf( "\n" );
	}
	Com_Printf( "\n" );
}

void SV_Status_f() {
	void (*call)();
	*(int*)&call = 0x80846B4;
	
	if(!sv_running->integer) {
		Com_Printf( "Server is not running.\n" );
		return;
	}

	call();
}

#ifdef xDEBUG

void SV_Test() {
	int argc = Cmd_Argc();
	printf("trap_argc = %d\n", argc);
}

void band_test() {
	int argc = Cmd_Argc();
	if(argc != 3) {
		printf("Error\n");
		return;
	}
	int index = 0;
	if(index < 0 || index >= sv_maxclients->integer) {
		printf("Error, wrong index.\n");
		return;
	}
	client_t* cl = getclient(index);
	NET_OutOfBandPrint( NS_SERVER, cl->netchan.remoteAddress, Cmd_Argv(2));
}
#endif

void Cmd_SetConfigstring() {
	if(Cmd_Argc()!=3) {
		printf("Usage: setcs <index> \"string\"\n");
		return;
	}
	int index = atoi(Cmd_Argv(1));
	char* str = Cmd_Argv(2);
	
	char cs[MAX_INFO_STRING];
	
	SV_GetConfigstring(index, cs, sizeof(cs));
	
	printf("The configstring %d was changed from '%s' to ", index, cs);
	SV_SetConfigstring(index, str);
	SV_GetConfigstring(index, cs, sizeof(cs));
	printf("'%s'.\n", cs);
}

void Cmd_GetConfigstrings() {
	if(Cmd_Argc()!=2) {
		printf("Usage: configstrings \"outputfile.txt\"\n");
		return;
	}
	
	char* out = Cmd_Argv(1);
	
	FILE* f = fopen(out, "wb");
	
	if(!f)
		return;
		
	int i;
	char cs[MAX_INFO_STRING];
	
	fprintf(f, "CONFIGSTRINGS CALL OF DUTY DUMP\n");
	for(i = 0; i < 2048; i++) {
		SV_GetConfigstring(i, cs, sizeof(cs));
		fprintf(f, "%d: %s\n", i, cs);
	}
}

void SV_Heartbeat_f( void ) {
	*(int*)0x83B67F4 = -9999999;
}

static void SV_ServerInfo_f( void ) {
	Com_Printf( "Server info settings:\n" );
	Info_Print( Cvar_InfoString( CVAR_SERVERINFO ) );
}

static void SV_SystemInfo_f( void ) {
	Com_Printf( "System info settings:\n" );
	Info_Print( Cvar_InfoString( CVAR_SYSTEMINFO ) );
}

static void SV_DumpUser_f( void ) {
	client_t    *cl;

	// make sure server is running
	if ( !sv_running->integer ) {
		Com_Printf( "Server is not running.\n" );
		return;
	}

	if ( Cmd_Argc() != 2 ) {
		Com_Printf( "Usage: info <userid>\n" );
		return;
	}

	cl = getclient(atoi(Cmd_Argv(1)));
	if ( !cl ) {
		return;
	}

	Com_Printf( "userinfo\n" );
	Com_Printf( "--------\n" );
	Info_Print( cl->userinfo );
}

/*
	stubs for whenever I want to fully rewrite them..
	
	-	 Richard
*/

static void SV_MapRestart_f( void ) {
	void (*call)( void );
	*(int*)&call = 0x8083DE4;
	
	call();
}

static void SV_Map_f( void ) {
	void (*call)( void );
	*(int*)&call = 0x8083C68;
	
	call();
}

static void SV_MapRotate_f( void ) {
	void (*call)( void );
	*(int*)&call = 0x80840C8;
	
	call();
}

void SV_GameCompleteStatus_f( void ) {
	void (*call)( void );
	*(int*)&call = 0x8084D54;
	
	call();
}

static void SV_KillServer_f( void ) {
	void (*call)( void );
	*(int*)&call = 0x8084D3C;
	
	call();
}

void SV_Say_f( void ) {
	void (*call)( void );
	*(int*)&call = 0x8084974;
	
	call();
}

void SV_StringUsage_f( void ) {
	void (*call)( void );
	*(int*)&call = 0x8084D74;
	
	call();
}

void SV_ScriptUsage_f( void ) {
	void (*call)( void );
	*(int*)&call = 0x8084D64;
	
	call();
}

void SV_KickNum_f( void ) {
	void (*call)( void );
	*(int*)&call = 0x8084BE4;
	
	call();
}

static bool sv_listmsg = false;

void SV_WasAdded(const char* cmd, int id, const char* name) {
	if(!sv_listmsg) {
		Com_Printf("Players found for '%s'\n", cmd);
		sv_listmsg=true;
	}
	Com_Printf("Client %d: %s\n", id, name);
}

client_t *search_players[64];

void X_GetPlayersByName( void ) {
	client_t    *cl;
	int i;
	char        *s;
	char cleanName[64];
	
	for(unsigned int p; p < 64; p++)
		search_players[p] = NULL;
	
	if (!sv_running->integer )
		return;

	if ( Cmd_Argc() < 2 ) {
		Com_Printf( "No substring given to search for.\n" );
		return;
	}

	s = Cmd_Argv( 1 );
	sv_listmsg = false;
	
	for ( i = 0 ; i < sv_maxclients->integer ; i++) {
		cl = getclient(i);
		if (!cl->state)
			continue;
			
		if ( strstr( cl->name, s ) ) {
			SV_WasAdded(s, i, cl->name);
			search_players[i] = cl;
			continue;
		}

		Q_strncpyz( cleanName, cl->name, sizeof( cleanName ) );
		Q_CleanStr( cleanName );
		if ( strstr( cleanName, s ) ) {
			search_players[i] = cl;
			SV_WasAdded(s, i, cl->name);
		}
	}
}

void SV_Kick_f( void ) {
	unsigned int i;
	bool multiple = false;
	
	if(!sv_running->integer ) {
		Com_Printf("Server is not running.\n");
		return;
	}
	
	if(Cmd_Argc()!=2) {
		Com_Printf("Usage: %s <substring>\n", Cmd_Argv(0));
		return;
	}
	
	if(Cmd_Argc()>2)
		multiple=true;
		
	X_GetPlayersByName();
	
	int count = 0;
	client_t* oneplayer = NULL;
	
	for(i = 0; i < 64; i++) {
		if(search_players[i] == NULL)
			continue;
		oneplayer=search_players[i];
		count++;
	}
	
	if(count > 1) {
		if(!multiple) {
			Com_Printf("Multiple search_players were found. If you want to affect multiple search_players at once.\nUse: %s \"%s\" multi\n", Cmd_Argv(0), Cmd_Argv(1));
			return;
		}
		for(i = 0; i < 64; i++) {
			if(search_players[i]!=NULL)
				SV_DropClient(search_players[i], "EXE_PLAYERKICKED");
		}
	} else if(count == 0) {
		Com_Printf("There were no search_players found for the substring '%s'\n", Cmd_Argv(1));
	} else if(count == 1) {
		SV_DropClient(oneplayer, "EXE_PLAYERKICKED");
	}
}

void SV_BanProcess(client_t* cl, int type, const char* reason) {
	const char* file = (type == IPBAN) ? "ipbans.txt" : "muidbans.txt";
	FILE* f = fopen(file, "a");
	
	if(!f) {
		Com_Printf("Could not open '%s'.\n", file);
		return;
	}
	
	challenge_t* challenge;
	char ip[16];
    sprintf(ip, "%d.%d.%d.%d", cl->netchan.remoteAddress.ip[0], cl->netchan.remoteAddress.ip[1], cl->netchan.remoteAddress.ip[2], cl->netchan.remoteAddress.ip[3]);
	char *muid = xclients[get_client_number(cl)].mUID;
	
	int k;
	
	list_each(banInfo_t*, cur, banlist) {
		if((!strcmp(cur->mUID, muid) && cur->type == MUIDBAN) || (cur->type == IPBAN && NET_CompareBaseAdr(cl->netchan.remoteAddress, cur->adr))) {
			Com_Printf("%s has already been banned.\n", cl->name);
			fclose(f);
			return;
		}
	}
	
	if(!*muid || strlen(muid) > 32) {
		Com_Printf("Error: Invalid mUID!\n");
		return;
	}
	
	banInfo_t* add = (banInfo_t*)xalloc(sizeof(banInfo_t));
	_list_add(&banlist, add);
	add->type = type;
	
	if(type==IPBAN) {
		add->adr = cl->netchan.remoteAddress;
		fprintf(f, "%s:%s\n", ip, reason);
	} else if(type == MUIDBAN) {
		fprintf(f, "%s:%s\n", muid, reason);
		Q_strncpyz(add->mUID, muid, sizeof(add->mUID));
	}
	
	fclose(f);
	
	SV_DropClient(cl, va("Banned: [Reason %s]", reason));
}

void SV_Ban_f( void ) {
	bool multiple = false;
	
	if(!sv_running->integer ) {
		Com_Printf("Server is not running.\n");
		return;
	}
	
	if(Cmd_Argc()!=4) {
		Com_Printf("Usage: %s <substring> \"reason\" \"muid|ip\"\n", Cmd_Argv(0));
		return;
	}
	
	char* reason = Cmd_Argv(2);
	
	if(!*reason)
		reason = "No reason.";
	
	char* bantypestr = Cmd_Argv(3);
	
	int bantype = IPBAN;
	
	if(!Q_stricmp(bantypestr, "muid"))
		bantype = MUIDBAN;
	
	if(Cmd_Argc() > 4)
		multiple = true;
		
	X_GetPlayersByName();
	
	int count = 0;
	
	client_t* oneplayer = NULL;
	
	int i;
	
	for(i = 0; i < 64; i++) {
		if(search_players[i] == NULL)
			continue;
		oneplayer = search_players[i];
		count++;
	}
	
	if(count > 1) {
		if(!multiple) {
			Com_Printf("Multiple search_players were found. If you want to affect multiple search_players at once.\nUse: %s \"%s\" \"reason\" \"muid|ip\" multi\n", Cmd_Argv(0), Cmd_Argv(1));
			return;
		}
		for(unsigned int i = 0; i < 64; i++) {
			if(search_players[i]!=NULL)
				SV_BanProcess(search_players[i], bantype, reason);
		}
	} else if(count == 0) {
		Com_Printf("There were no search_players found for the substring '%s'\n", Cmd_Argv(1));
	} else if(count == 1) {
		SV_BanProcess(oneplayer, bantype, reason);
	}
}

void SV_BanNum_f( void ) {
	if(!sv_running->integer ) {
		Com_Printf("Server is not running.\n");
		return;
	}
	
	if(Cmd_Argc()!=4) {
		Com_Printf("Usage: %s <id> \"reason\" \"muid|ip\"\n", Cmd_Argv(0));
		return;
	}
	
	client_t* cl = getclient(atoi(Cmd_Argv(1)));
	
	if(!cl->state) {
		Com_Printf("Client is not active.\n");
		return;
	}
	
	char* reason = Cmd_Argv(2);
	
	if(!*reason)
		reason = "No reason.";
	
	char* bantypestr = Cmd_Argv(3);
	
	int bantype = IPBAN;
	
	if(!Q_stricmp(bantypestr, "muid"))
		bantype = MUIDBAN;
		
	SV_BanProcess(cl, bantype, reason);
}

void X_ReadBannedList(bool);

void SV_UnbanMUID_f() {
	if(Cmd_Argc() != 2) {
		Com_Printf("Usage: unbanmuid <muid>\n");
		return;
	}
	
	LinkedList it = banlist;
	char *muid = Cmd_Argv( 1 );
	
	FILE* f = fopen("muidbans.txt", "w");
	
	if(f) {
		while(it != NULL) {
			banInfo_t *info = (banInfo_t*)it->data;
			if(info->type == MUIDBAN) {
				if(!strcmp(info->mUID, muid)) {
					Com_Printf("mUID '%s' has been unbanned.\n", info->mUID);
					it = it->next;
					continue;
				}
				fprintf(f, "%s:%s\n", info->mUID, info->reason);
			}
			it = it->next;
		}
	
		fclose(f);
	}

	X_ReadBannedList(false);
}

void SV_UnbanIP_f() {
	if(Cmd_Argc()!=2) {
		Com_Printf("Usage: unbanip <ip>\n");
		return;
	}
	
	LinkedList it = banlist;
	
	char* ip = Cmd_Argv(1);
	netadr_t ipadr;
	NET_StringToAdr(ip, &ipadr);
	
	FILE* f = fopen("ipbans.txt", "w");
	
	if(f) {
		
		while(it != NULL) {
			banInfo_t *info = (banInfo_t*)it->data;
			if(info->type == IPBAN) {
				if(NET_CompareBaseAdr(info->adr, ipadr)) {
					Com_Printf("IP '%s' has been unbanned.\n", ip);
					it = it->next;
					continue;
				}
				fprintf(f, "%s:%s\n", NET_BaseAdrToString(info->adr), info->reason);
			}
			it = it->next;
		}
	
		fclose(f);
	}
	
	X_ReadBannedList(false);
	
	/*qboolean found = qfalse;
	netadr_t unban;
	
	if(Cmd_Argc() != 2) {
		Com_Printf("Usage: unbanip \"127.0.0.1\"\n");
		return;
	}
	
	FILE* f = fopen("ipban.txt", "w");
	
	if(!f) {
		Com_Printf("Failed to open ipban.txt\n");
		return;
	}
	
	NET_StringToAdr(Cmd_Argv(1), &unban);
	
	std::ifstream in("ipban.txt");
	std::string line;
	if(in.is_open()) {
		bannedIPs.clear();
		
		netadr_t adr;
		while(std::getline(in, line)) {
			if(line.empty())
				continue;
			NET_StringToAdr(line.c_str(), &adr);
			
			if(!strcmp(Cmd_Argv(1), line.c_str())) {
				Com_Printf("'%s' has been unbanned.\n", Cmd_Argv(1));
				found = qtrue;
				continue;
			}
			fprintf(f, "%s\n", NET_BaseAdrToString(adr));
			bannedIPs.push_back(adr);
		}
		if(!found) {
			Com_Printf("'%s' was not found in the banlist.\n", Cmd_Argv(1));
		}
	} else {
		Com_Printf("Error whilst reading ipban.txt, failed to correctly unban.\n");
	}
	
	fclose(f);
	*/
}

void SV_BanIP_f() {
	if(Cmd_Argc()!=2) {
		Com_Printf("Usage: banip <ip> [reason]\n");
		return;
	}
	
	char* ip = Cmd_Argv(1);
	char *reason = Cmd_Argv(2);
	netadr_t adr;
	
	NET_StringToAdr(ip, &adr);
	
	client_t* cl;
	
	if(sv_running->integer) {
		for(int i = 0; i < sv_maxclients->integer; i++) {
			cl = getclient(i);
			if(!cl->state)	continue;
			if(NET_CompareBaseAdr(adr, cl->netchan.remoteAddress)) {
				SV_DropClient(cl, "banned");
				break;
			}
		}
	}
	
	FILE* f = fopen("ipbans.txt", "a");
	if(f) {
		fprintf(f,"%s:%s\n",ip, reason);
		fclose(f);
	}
	
	Com_Printf("IP '%s' has been banned.\n", ip);
	
	X_ReadBannedList(false);
}

void SV_BanMUID_f() {
	if(Cmd_Argc()!=2) {
		Com_Printf("Usage: banmuid <muid> [reason]\n");
		return;
	}
	
	char *muid = Cmd_Argv(1);
	char *reason = Cmd_Argv(2);
	
	if(!*muid || strlen(muid) > 32) {
		Com_Printf("Error: Invalid mUID!\n");
		return;
	}
	
	client_t* cl;
		
	if(sv_running->integer) {
		for(int i = 0; i < sv_maxclients->integer; i++) {
			cl=getclient(i);
			if(!cl->state)	continue;
			if(!strcmp(xclients[i].mUID, muid)) {
				SV_DropClient(cl, reason);
				break;
			}
		}
	}
	
	FILE* f = fopen("muidbans.txt", "a");
	if(f) {
		fprintf(f, "%s:%s\n", muid, reason);
		fclose(f);
	}
	
	Com_Printf("mUID '%s' has been banned.\n", muid);
	
	X_ReadBannedList(false);
}

void SV_MuteName() {
	if(Cmd_Argc() != 2) {
		Com_Printf("Usage: mutename <id>\n");
		return;
	}
	
	int id = atoi( Cmd_Argv(1) );
	
	if(id < 0 || id > 64) {
		Com_Printf("Invalid number.\n");
		return;
	}
	
	client_t *cl = getclient( id );
	
	if( cl == NULL ) {
		Com_Printf("Client doesn't exist!\n");
		return;
	}
	
	x_clients[id].namemuted ^= 1;
	Com_Printf("Client: '%s' is now %s.\n", cl->name, x_clients[id].namemuted ? "muted" : "unmuted");
}

void SV_Mute() {
	if(Cmd_Argc() != 2) {
		Com_Printf("Usage: mute <id>\n");
		return;
	}
	
	int id = atoi( Cmd_Argv(1) );
	
	if(id < 0 || id > 64) {
		Com_Printf("Invalid number.\n");
		return;
	}
	
	client_t *cl = getclient( id );
	
	if( cl == NULL ) {
		Com_Printf("Client doesn't exist!\n");
		return;
	}
	
	x_clients[id].muted ^= 1;
	Com_Printf("Client: '%s' is now %s.\n", cl->name, x_clients[id].muted ? "muted" : "unmuted");
}

void X_ReadBannedList_sub() {
	X_ReadBannedList(true);
}

void SV_AddOperatorCommands(void) {
	static qboolean* initialized = (qboolean*)0x8160680;
	
	if(*initialized)
		return;
		
	*initialized = qtrue;
	
	Cmd_AddCommand("heartbeat", SV_Heartbeat_f);
	
	Cmd_AddCommand("kick", SV_Kick_f);
	Cmd_AddCommand("ban", SV_Ban_f);
	
	Cmd_AddCommand("reloadbans", X_ReadBannedList_sub);
	
	Cmd_AddCommand("clientkick", SV_KickNum_f);
	Cmd_AddCommand("clientban", SV_BanNum_f);
	
	Cmd_AddCommand("banip", SV_BanIP_f);
	Cmd_AddCommand("banmuid", SV_BanMUID_f);
	
	Cmd_AddCommand("unbanip", SV_UnbanIP_f);
	Cmd_AddCommand("unbanmuid", SV_UnbanMUID_f);
	
	Cmd_AddCommand("status", SV_Status_f);
	Cmd_AddCommand("xstatus", SV_XStatus_f);
	Cmd_AddCommand("serverinfo", SV_ServerInfo_f);
	Cmd_AddCommand("systeminfo", SV_SystemInfo_f);
	Cmd_AddCommand("dumpuser", SV_DumpUser_f);
	Cmd_AddCommand("map_restart", SV_MapRestart_f);
	Cmd_AddCommand("map", SV_Map_f);
	Cmd_AddCommand("devmap", SV_Map_f);
	Cmd_AddCommand("map_rotate", SV_MapRotate_f);
	Cmd_AddCommand("gameCompleteStatus", SV_GameCompleteStatus_f);
	Cmd_AddCommand("killserver", SV_KillServer_f);
	Cmd_AddCommand("scriptUsage", SV_ScriptUsage_f);
	Cmd_AddCommand("stringUsage", SV_StringUsage_f);
	Cmd_AddCommand("cs", Cmd_GetConfigstrings);
	Cmd_AddCommand("setcs", Cmd_SetConfigstring);
	
	Cmd_AddCommand("codextended", SV_Version);
	Cmd_AddCommand("xtnded", SV_Version);
	#ifdef xDEBUG
	Cmd_AddCommand("trap", SV_Test);
	Cmd_AddCommand("net", band_test);
	#endif
	Cmd_AddCommand("say", SV_Say_f);
}