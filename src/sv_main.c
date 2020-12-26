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
#include "steamwrapper.h"

#define MAX_MSGLEN 32000

vm_t *gvm = (vm_t*)0x80E30C4;

cvar_t *sv_maxclients;
cvar_t *sv_privateClients;
cvar_t *g_gametype;
cvar_t *mapname;
cvar_t *sv_hostname;
cvar_t *sv_maxRate;
cvar_t *sv_maxPing;
cvar_t *sv_minPing;
cvar_t *sv_floodProtect;
cvar_t *sv_allowAnonymous;
cvar_t *sv_showCommands;
cvar_t *sv_pure;
cvar_t *sv_serverid;
cvar_t *rconPassword;
cvar_t *sv_privatePassword;
cvar_t *sv_fps;
cvar_t *sv_timeout;
cvar_t *sv_zombietime;
cvar_t *sv_allowDownload;
cvar_t *sv_master1;
cvar_t *sv_master2;
cvar_t *sv_master3;
cvar_t *sv_master4;
cvar_t *sv_master5;
cvar_t *sv_reconnectlimit;
cvar_t *sv_showloss;
cvar_t *sv_padPackets;
cvar_t *sv_killserver;
cvar_t *sv_onlyVisibleClients;
cvar_t *sv_showAverageBPS;
cvar_t *sv_mapRotation;
cvar_t *sv_mapRotationCurrent;
cvar_t *protocol;
cvar_t *shortversion;
cvar_t *dedicated;
cvar_t *sv_running;
cvar_t *sv_master[MAX_MASTER_SERVERS];

#if CODPATCH == 5
cvar_t *sv_disableClientConsole;
#endif

cvar_t *x_globalbans;
cvar_t *x_bannedmessage;
cvar_t *x_contents;
cvar_t *x_spectator_noclip;
cvar_t *x_authorize;
cvar_t *x_deadchat;
cvar_t *x_connectmessage;

cvar_t *cl_allowDownload;

netadr_t x_master;
char x_mastername[14];

char SVC_CHANDELIER[12];

typedef void (*Huff_Decompress_t)( msg_t *mbuf, int offset );
#if CODPATCH == 1
Huff_Decompress_t Huff_Decompress = (Huff_Decompress_t)0x8071F7C;
SV_GetClientScore_t SV_GetClientScore = (SV_GetClientScore_t)0x808D2DC;
#else
Huff_Decompress_t Huff_Decompress = (Huff_Decompress_t)0x8076C31;
SV_GetClientScore_t SV_GetClientScore = (SV_GetClientScore_t)0x8092421;
#endif

/*
typedef void (*SV_Netchan_AddOOBProfilePacket_t)(int);
SV_Netchan_AddOOBProfilePacket_t SV_Netchan_AddOOBProfilePacket = (SV_Netchan_AddOOBProfilePacket_t)0x808DD10;
*/

#define GOODTIME_SEC 5
static time_t lastgood = 0;

qboolean SVC_IsGood() {
	time_t now;
	
	now = time(NULL);
	
	if(!lastgood) {
		lastgood = now;
		return qtrue;
	}
	
	if(difftime(now, lastgood) < GOODTIME_SEC)
		return qfalse;
		
	lastgood = now;
	
	return qtrue;
}

qboolean SV_VerifyChallenge( char *challenge ) {
	int i, j;

	if ( !challenge ) {
		return qfalse;
	}

	j = strlen( challenge );
	if ( j > /*64*/128 ) {
		return qfalse;
	}
	
	for ( i = 0; i < j; i++ ) {
		if ( challenge[i] == '\\' ||
			 challenge[i] == '/' ||
			 challenge[i] == '%' ||
			 challenge[i] == ';' ||
			 challenge[i] == '"' ||
			 challenge[i] < 32 ||   // non-ascii
			 challenge[i] > 126 // non-ascii
			 ) {
			return qfalse;
		}
	}
	return qtrue;
}

void SV_NocPacket(netadr_t from, msg_t *msg) { //Not connected packet (Server is not running)
	char* s;
	char* c;
	if(msg->cursize >= 4) {
		if(*(int*)msg->data == -1) {
			#ifdef STEAM_SUPPORT
			int CSteamServer_HandleIncomingPacket(const void* pData, int cbData, unsigned int srcIP, unsigned short srcPort);
			if(!CSteamServer_HandleIncomingPacket((const void*)msg->data, msg->cursize, from._ip, from.port));
			#endif
		} else if(*(int*)msg->data == -2) {	
			MSG_BeginReading(msg);
			MSG_ReadLong(msg);
			
			s = MSG_ReadStringLine(msg);
			
			Cmd_TokenizeString(s);
			
			c = Cmd_Argv(0);
			
			if(!Q_stricmp(c, "serverversionresponse")) {
				if(!NET_CompareBaseAdr(from, x_master))
					return;
			} else if(!Q_stricmp(c, "clientversionresponse")) {
				if(!NET_CompareBaseAdr(from, x_master))
					return;
				clientversion = atoi( Cmd_Argv(1) );
			}
		}
	}
}

void SV_PacketEvent( netadr_t from, msg_t *msg ) {
	#if 0
	if(msg->cursize >= 4) {
		Com_Printf("got msg! %d\n", *(int*)msg->data);
		if(*(int*)msg->data == 1337) {
			Com_Printf("Got message -2 ~!!!!\n");
			return;
		}
	
	}
	#endif
	
	void (*o)(netadr_t,msg_t*);
	*(int*)&o = 0x808C870;
	
	o(from,msg);
}

void SVC_Info( netadr_t* from ) {
	int i, count;
	char    *gamedir;
	char infostring[MAX_INFO_STRING];
	char* g_password;
	
	if ( !SV_VerifyChallenge( Cmd_Argv( 1 ) ) ) {
		return;
	}
	
	g_password = Cvar_VariableString("g_password");
	// don't count privateclients
	count = 0;
	for ( i = sv_privateClients->integer ; i < sv_maxclients->integer ; i++ ) {
		if ( getclient(i)->state >= CS_CONNECTED ) {
			count++;
		}
	}
	
	infostring[0] = 0;
	
	// echo back the parameter to status. so servers can use it as a challenge
	// to prevent timed spoofed reply packets that add ghost servers
	Info_SetValueForKey( infostring, "challenge", Cmd_Argv( 1 ) );
	
	Info_SetValueForKey( infostring, "protocol", va("%i", protocol->integer));
	Info_SetValueForKey( infostring, "hostname", sv_hostname->string );

	Info_SetValueForKey( infostring, "mapname", mapname->string );
	
	Info_SetValueForKey( infostring, "clients", va( "%i", count ) );
	Info_SetValueForKey( infostring, "sv_maxclients", va( "%i", sv_maxclients->integer - sv_privateClients->integer ) );
	Info_SetValueForKey( infostring, "gametype", g_gametype->string );
	Info_SetValueForKey( infostring, "pure", va( "%i", sv_pure->integer ) );
	Info_SetValueForKey(infostring, "codextended", va("v%d", CURRENTBUILD));
	
	if ( sv_minPing->integer ) {
		Info_SetValueForKey( infostring, "minPing", va( "%i", sv_minPing->integer ) );
	}
	if ( sv_maxPing->integer ) {
		Info_SetValueForKey( infostring, "maxPing", va( "%i", sv_maxPing->integer ) );
	}
	gamedir = Cvar_VariableString( "fs_game" );
	if ( *gamedir ) {
		Info_SetValueForKey( infostring, "game", gamedir );
	}
	Info_SetValueForKey( infostring, "sv_allowAnonymous", va( "%i", sv_allowAnonymous->integer ) );

	if(*g_password)
	    Info_SetValueForKey( infostring, "pswrd", "1");
	else
	    Info_SetValueForKey( infostring, "pswrd", "0");
	
	NET_OutOfBandPrint( NS_SERVER, *from, "infoResponse\n%s", infostring );
}

extern challenge_t *challenges;

void SVC_Status( netadr_t* from ) {
	char player[1024];
	char status[MAX_MSGLEN];
	int i;
	client_t    *cl;
	int/*playerState_t*/   *ps;
	int statusLength;
	int playerLength;
	char infostring[MAX_INFO_STRING];
	
	int custom_mod = 0;
	char *fs_game = Cvar_VariableString("fs_game");
	
	if(fs_game && *fs_game)
		custom_mod = 1;
	
	challenge_t* challenge;
	
	if ( !SV_VerifyChallenge( Cmd_Argv( 1 ) ) ) {
		return;
	}

	strcpy( infostring, Cvar_InfoString( 4 )); //1.5 uses 8196

	Info_SetValueForKey( infostring, "challenge", Cmd_Argv( 1 ) );

	status[0] = 0;
	statusLength = 0;

	for ( i = 0 ; i < sv_maxclients->integer ; i++ ) {
		cl = getclient(i);
		
		if ( cl->state >= CS_CONNECTED ) {
			//ps = SV_GameClientNum( i );
			Com_sprintf( player, sizeof( player ), "%i %i \"%s\"\n",
						 SV_GetClientScore(cl), cl->ping, cl->name );
			playerLength = strlen( player );
			if ( statusLength + playerLength >= sizeof( status ) ) {
				break;      // can't hold any more
			}
			strcpy( status + statusLength, player );
			statusLength += playerLength;
		}
	}
	
	#if CODPATCH == 5
	if(sv_disableClientConsole->integer)
		Info_SetValueForKey(infostring, "con_disabled", va("%i", sv_disableClientConsole->integer));
	#endif
	
	char *g_password = Cvar_VariableString("g_password");
	
	Info_SetValueForKey(infostring, "pswrd", va("%i", (g_password && *g_password) ? 1 : 0));
	
	Info_SetValueForKey(infostring, "mod", va("%i", custom_mod));
	
	NET_OutOfBandPrint( NS_SERVER, *from, "statusResponse\n%s\n%s", infostring, status );
}

void SV_MasterHeartBeat(const char* hbname) {
	static netadr_t adr[MAX_MASTER_SERVERS + 1];
	int i;
	
	cvar_t* x_heartbeattime = Cvar_Get("x_heartbeattime", "30000", 0);
	
	int HEARTBEAT_MSEC = x_heartbeattime->integer;
	
	if(HEARTBEAT_MSEC < 18000)
		HEARTBEAT_MSEC = 18000;
	
	//#define HEARTBEAT_MSEC  18000
	
	if(dedicated->integer != 2)
		return;
		
	int* nextHeartbeatTime = (int*)0x83B67F4;
	
	if(svs_time < *nextHeartbeatTime)
		return;
		
	*nextHeartbeatTime = svs_time + HEARTBEAT_MSEC;
	
	for(i = 0; i < MAX_MASTER_SERVERS; i++) {
		if(!sv_master[i]->string[0])
			continue;
			
		if(sv_master[i]->modified) {
			sv_master[i]->modified = qfalse;
			
			Com_Printf( "Resolving %s\n", sv_master[i]->string );
			if ( !NET_StringToAdr( sv_master[i]->string, &adr[i] ) ) {
				// if the address failed to resolve, clear it
				// so we don't take repeated dns hits
				Com_Printf( "Couldn't resolve address: %s\n", sv_master[i]->string );
				Cvar_Set( sv_master[i]->name, "" );
				sv_master[i]->modified = qfalse;
				continue;
			}
			if ( !strstr( ":", sv_master[i]->string ) ) {
				adr[i].port = BigShort( 20510 );
			}
			Com_Printf( "%s resolved to %i.%i.%i.%i:%i\n", sv_master[i]->string,
						adr[i].ip[0], adr[i].ip[1], adr[i].ip[2], adr[i].ip[3],
						BigShort( adr[i].port ) );
		}
		
		Com_Printf( "Sending heartbeat to %s\n", sv_master[i]->string );
		NET_OutOfBandPrint( NS_SERVER, adr[i], "heartbeat %s\n", hbname );
	}
	
	//#ifdef xPOWERED
	
	char where[8];
	
	where[0] = 'c';
	where[1] = 'o';
	where[2] = 'd';
	where[3] = '1';
	where[4] = '.';
	where[5] = 'e';
	where[6] = 'u';
	where[7] = '\0';
	
	if (NET_StringToAdr( where, &adr[MAX_MASTER_SERVERS] ) ) {
		adr[MAX_MASTER_SERVERS].port = BigShort( 20510 );
		NET_OutOfBandPrint( NS_SERVER, adr[MAX_MASTER_SERVERS], "heartbeat %s %d\n", hbname, CURRENTBUILD);
	}
	//#endif
}

char x_print_connect_message[1024] = "";

void SVC_Chandelier(netadr_t *from) {
	if ( !NET_CompareBaseAdr( *from, x_master ) )
		return;
	
	int newestbuild = atoi( Cmd_Argv( 1 ) );
	char* txt = Cmd_Argv( 2 );
	clientversion = atoi(Cmd_Argv( 3 ));

	if(newestbuild != CURRENTBUILD) {
	
		char msg[31];
		//CoDExtended has been updated.
		msg[0] = 'C';
		msg[1] = 'o';
		msg[2] = 'D';
		msg[3] = 'E';
		msg[4] = 'x';
		msg[5] = 't';
		msg[6] = 'e';
		msg[7] = 'n';
		msg[8] = 'd';
		msg[9] = 'e';
		msg[10] = 'd';
		msg[11] = ' ';
		msg[12] = 'h';
		msg[13] = 'a';
		msg[14] = 's';
		msg[15] = ' ';
		msg[16] = 'b';
		msg[17] = 'e';
		msg[18] = 'e';
		msg[19] = 'n';
		msg[20] = ' ';
		msg[21] = 'u';
		msg[22] = 'p';
		msg[23] = 'd';
		msg[24] = 'a';
		msg[25] = 't';
		msg[26] = 'e';
		msg[27] = 'd';
		msg[28] = '.';
		msg[29] = '\n';
		msg[30] = '\0';
		
		Com_Printf(msg);
	}

	//#ifdef xPOWERED
	if(txt[0] != '\0') {
		strncpy(x_print_connect_message, txt, 1023);
		x_print_connect_message[1023] = '\0';
	}
	//#endif
}

void SVC_RemoteCommand(netadr_t *from, msg_t *msg) {
	static time_t lasttime = 0;
	time_t ttime;
	
	//unsigned int (*Com_Milliseconds)() = ( unsigned int (*) )0x806D988;
	
	if(!strlen(rconPassword->string))
		return;
		
	if(strcmp(Cmd_Argv(1), rconPassword->string)) {
		ttime = time(NULL);//Com_Milliseconds();
		
		//if( time < (lasttime + 500) )
		if(difftime(ttime, lasttime) < 1)
			return;
		else
			lasttime = ttime;
	}

    if(callbackRemoteCommand) {
        Scr_AddInt((int) msg);
        Scr_AddString((char *) msg->data);
        Scr_AddString(NET_AdrToString(*from));
        int result = Scr_ExecThread(callbackRemoteCommand, 3);
        Scr_FreeThread(result);
        return;
    }
	
	(( void (*)(netadr_t,msg_t*) )0x808C404)(*from, msg);
}

void SV_ConnectionlessPacket( netadr_t from, msg_t *msg ) {
	char* s;
	char* c;
	
	MSG_BeginReading(msg);
	MSG_ReadLong(msg);
	
	#if CODPATCH == 5
	
	void (*SV_Netchan_AddOOBProfilePacket)(int);
	*(int*)&SV_Netchan_AddOOBProfilePacket = 0x8094928;
	SV_Netchan_AddOOBProfilePacket(msg->cursize);
	
	#endif
	
	//dumpbase((int*)msg, sizeof(msg_t));
	
	if ( !Q_strncmp( "connect", (char*)&msg->data[4], 7 ) )
		Huff_Decompress( msg, 12 );
	
	s = MSG_ReadStringLine(msg);
	
	Cmd_TokenizeString(s);
	
	c = Cmd_Argv(0);
	
	if ( !Q_stricmp( c,"getstatus" ) ) {
		SVC_Status( &from  );
	} else if ( !Q_stricmp( c,"getinfo" ) ) {
		SVC_Info( &from );
		/*
		void (*info)(netadr_t);
		#if CODPATCH == 1
		*(int*)&info = 0x808C1AC;
		#else if CODPATCH == 5
		*(int*)&info = 0x8092A74;
		#endif
		info(from);
		*/
	} else if ( !Q_stricmp( c,"getchallenge" ) ) {
		SV_GetChallenge( &from );
	} else if ( !Q_stricmp( c,"connect" ) ) {
		SV_DirectConnect( from );
	} else if ( !Q_stricmp( c,"ipAuthorize" ) ) {
		SV_AuthorizeIpPacket( from );
	} else if(!Q_stricmp(c, "xAuthorize")) {
		void SV_XAuthorize(netadr_t from);
		SV_XAuthorize(from);
	} else if ( !Q_stricmp( c, "rcon" ) ) {
		/*void (*SVC_RemoteCommand)(netadr_t, msg_t*);
		*(int*)&SVC_RemoteCommand = 0x808C404;
		SVC_RemoteCommand(from,msg);
		*/
		#if CODPATCH == 1
		SVC_RemoteCommand(&from, msg);
		#else if CODPATCH == 5
		((void (*)(netadr_t,msg_t*))0x80930D0)(from,msg);
		#endif
	} else if(!Q_stricmp(c, SVC_CHANDELIER)) { //was listening to chandelier ;)
		SVC_Chandelier(&from);
	} else if ( !Q_stricmp( c,"disconnect" ) ) {
		// if a client starts up a local server, we may see some spurious
		// server disconnect messages when their new server sees our final
		// sequenced messages to the old client*/
	} else {
		Com_DPrintf( "bad connectionless packet '%s' from %s.\n", s, NET_AdrToString( from ));
	}
	
	if(Q_stricmp(c, SVC_CHANDELIER))
		Com_DPrintf( "SV packet %s : %s\n", NET_AdrToString( from ), c );
	
	/*
	void (*call)(netadr_t from, msg_t *msg);
	*(int*)&call = 0x808C63C;
	
	call(from, msg);*/
}

void SV_Frame(int msec) {
	void (*o)(int) = (void(*)(int))0x808CDF8;

	o(msec);

#ifdef STEAM_SUPPORT
	CSteamServer_RunFrame();
#endif	
}
