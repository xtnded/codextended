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

#define MAX_MSGLEN 32000

vm_t *gvm = (vm_t*)0x80E30C4;

/*
cvar_t  *sv_maxclients = (cvar_t*)0x8355250;
cvar_t *sv_privateClients = (cvar_t*)0x8355220;
cvar_t *g_gametype = (cvar_t*)0x83B6778;
cvar_t *mapname = (cvar_t*)0x835523C;
cvar_t *sv_hostname = (cvar_t*)0x8355248;
cvar_t *sv_maxRate = (cvar_t*)0x83B674C;
cvar_t *sv_maxPing = (cvar_t*)0x83B6754;
cvar_t *sv_minPing = (cvar_t*)0x83B6788;
cvar_t *sv_floodProtect = (cvar_t*)0x8355230;
cvar_t *sv_allowAnonymous = (cvar_t*)0x8355244;
cvar_t *sv_showCommands = (cvar_t*)0x83C1828;
cvar_t *sv_pure = (cvar_t*)0x83B6758;
cvar_t *sv_serverid = (cvar_t*)0x835524C;
cvar_t *rconPassword = (cvar_t*)0x83B6780;
cvar_t *sv_privatePassword = (cvar_t*)0x8355240;
cvar_t *sv_fps = (cvar_t*)0x835522C;
cvar_t *sv_timeout = (cvar_t*)0x83B6750;
cvar_t *sv_zombietime = (cvar_t*)0x83B677C;
cvar_t *sv_allowDownload = (cvar_t*)0x83B675C;
cvar_t *sv_master1 = (cvar_t*)0x83B6760;
cvar_t *sv_master2 = (cvar_t*)0x83B6764;
cvar_t *sv_master3 = (cvar_t*)0x83B6768;
cvar_t *sv_master4 = (cvar_t*)0x83B676C;
cvar_t *sv_master5 = (cvar_t*)0x83B6770;
cvar_t *sv_reconnectlimit = (cvar_t*)0x83B6784;
cvar_t *sv_showloss = (cvar_t*)0x83C1824;
cvar_t *sv_padPackets = (cvar_t*)0x8355254;
cvar_t *sv_killserver = (cvar_t*)0x8355234;
cvar_t *sv_onlyVisibleClients = (cvar_t*)0x83B6774;
cvar_t *sv_showAverageBPS = (cvar_t*)0x8355228;
cvar_t *sv_mapRotation = (cvar_t*)0x8355238;
cvar_t *sv_mapRotationCurrent = (cvar_t*)0x8355224;
*/

cvar_t  *sv_maxclients;
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
cvar_t* dedicated;
cvar_t* sv_running;
cvar_t* x_bannedmessage;

cvar_t* sv_master[MAX_MASTER_SERVERS];

cvar_t *x_authorize;
#if 0
std::vector<netadr_t> bannedIPs;
std::vector<int> bannedGUIDs;
#endif
typedef void (*Huff_Decompress_t)( msg_t *mbuf, int offset );
Huff_Decompress_t Huff_Decompress = (Huff_Decompress_t)0x8071F7C;

SV_GetClientScore_t SV_GetClientScore = (SV_GetClientScore_t)0x808D2DC;

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

void SVC_Info( netadr_t* from ) {
	int i, count;
	char    *gamedir;
	char infostring[MAX_INFO_STRING];
	char* g_password;
	
	if ( !SV_VerifyChallenge( Cmd_Argv( 1 ) ) ) {
		return;
	}
	
	g_password = Cvar_VariableString("g_password");
	#ifdef xDEBUG
	cvar_t* x_fakenum = Cvar_Get("x_fakenum", "0", 0);
	#endif
	// don't count privateclients
	count = 0;
	for ( i = sv_privateClients->integer ; i < sv_maxclients->integer ; i++ ) {
		if ( getclient(i)->state >= CS_CONNECTED ) {
			count++;
		}
	}

	infostring[0] = 0;
	
	#ifdef xDEBUG
	int lolclients = (x_fakenum->integer) ? x_fakenum->integer : count;
	#else
	int lolclients = count;
	#endif
	
	// echo back the parameter to status. so servers can use it as a challenge
	// to prevent timed spoofed reply packets that add ghost servers
	Info_SetValueForKey( infostring, "challenge", Cmd_Argv( 1 ) );
	
	Info_SetValueForKey( infostring, "protocol", va("%i", protocol->integer));
	Info_SetValueForKey( infostring, "hostname", sv_hostname->string );
	Info_SetValueForKey( infostring, "mapname", mapname->string );
	Info_SetValueForKey( infostring, "clients", va( "%i", lolclients ) );
	Info_SetValueForKey( infostring, "sv_maxclients", va( "%i", sv_maxclients->integer - sv_privateClients->integer ) );
	Info_SetValueForKey( infostring, "gametype", g_gametype->string );
	Info_SetValueForKey( infostring, "pure", va( "%i", sv_pure->integer ) );
	Info_SetValueForKey(infostring, "codextended", va("Build %d", CURRENTBUILD));
	
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
	
	challenge_t* challenge;
	
	if ( !SV_VerifyChallenge( Cmd_Argv( 1 ) ) ) {
		return;
	}

	strcpy( infostring, Cvar_InfoString( CVAR_SERVERINFO | CVAR_NORESTART ));

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

	NET_OutOfBandPrint( NS_SERVER, *from, "statusResponse\n%s\n%s", infostring, status );
}

void dumpbase(int* base, size_t len) {
	unsigned int result;
	float rf;
	char* rstr;
	
	FILE* f = fopen("/home/cod/dump.txt", "wb");
	if(f) {
		fprintf(f, "DUMP\n");
		for(unsigned int i = 0; i < len; i++) {
			result = *(unsigned int*)(base + i);
			rf = *(float*)(base + i);
			rstr = (char*)(base + i);
			fprintf(f, "%d: Hex: %x, Decimal: %d, Float: %f, String: %s\n", i, result, result, rf, rstr);
		}
		fclose(f);
	}
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
		
	int* svs_time = (int*)0x83B67A4;
	int* nextHeartbeatTime = (int*)0x83B67F4;
	
	if(*svs_time < *nextHeartbeatTime)
		return;
		
	*nextHeartbeatTime = *svs_time + HEARTBEAT_MSEC;
	
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
	
	char *where = "cod1.eu";
	
	if (NET_StringToAdr( where, &adr[MAX_MASTER_SERVERS] ) ) {
		adr[MAX_MASTER_SERVERS].port = BigShort( 20510 );
		NET_OutOfBandPrint( NS_SERVER, adr[MAX_MASTER_SERVERS], "heartbeat %s %d\n", hbname, CURRENTBUILD);
	}
}

#if 0
void SVC_RemoteCommand(netadr_t *from, msg_t *msg) {
	bool valid = 0;
	
	static unsigned int lasttime = 0;
	unsigned int time;
	
	unsigned int (*Com_Milliseconds)() = ( unsigned int (*) )0x806D988;

	time = Com_Milliseconds();
	
	if( time < (lasttime + 500) )
		return;
	lasttime = time;
	
	if(!strlen(sv_rconPassword->string) || strcmp(Cmd_Argv(1), sv_rconPassword->string)) {
		
	}
}
#endif

void SV_ConnectionlessPacket( netadr_t from, msg_t *msg ) {
	char* s;
	char* c;
	
	MSG_BeginReading(msg);
	MSG_ReadLong(msg);
	
	
	//dumpbase((int*)msg, sizeof(msg_t));
	if ( !Q_strncmp( "connect", (char*)&msg->data[4], 7 ) ) {
		Huff_Decompress( msg, 12 );
	}
	
	s = MSG_ReadStringLine(msg);
	
	//Com_DPrintf("MSG_ReadStringLine = %s\n", s);
	
	Cmd_TokenizeString(s);
	
	c = Cmd_Argv(0);
	
	if ( !Q_stricmp( c,"getstatus" ) ) {
		SVC_Status( &from  );
	} else if ( !Q_stricmp( c,"getinfo" ) ) {
		//SVC_Info( &from );
		void (*info)(netadr_t);
		*(int*)&info = 0x808C1AC;
		info(from);
	} else if ( !Q_stricmp( c,"getchallenge" ) ) {
		SV_GetChallenge( &from );
	} else if ( !Q_stricmp( c,"connect" ) ) {
		SV_DirectConnect( &from );
	} else if ( !Q_stricmp( c,"ipAuthorize" ) ) {
		SV_AuthorizeIpPacket( from );
	} else if ( !Q_stricmp( c, "rcon" ) ) {
		/*void (*SVC_RemoteCommand)(netadr_t, msg_t*);
		*(int*)&SVC_RemoteCommand = 0x808C404;
		SVC_RemoteCommand(from,msg);
		*/
		((void (*)(netadr_t,msg_t*))0x808C404)(from,msg);
	} else if ( !Q_stricmp( c,"disconnect" ) ) {
		// if a client starts up a local server, we may see some spurious
		// server disconnect messages when their new server sees our final
		// sequenced messages to the old client*/
	} else {
		Com_DPrintf( "bad connectionless packet '%s' from %s.\n", s, NET_AdrToString( from ));
	}
	
	Com_DPrintf( "SV packet %s : %s\n", NET_AdrToString( from ), c );
	
	/*
	void (*call)(netadr_t from, msg_t *msg);
	*(int*)&call = 0x808C63C;
	
	call(from, msg);*/
}
