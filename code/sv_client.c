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
#include <sys/time.h>

cvar_t *x_connect_message;
cvar_t *x_connect_message_time;

xtnded_client xtnded_clients[64];
x_challenge x_challenges[MAX_CHALLENGES];

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

//static ucmd_t* ucmds = (ucmd_t*)0x80E2F4C;

void SV_VerifyPaks_f(client_t*);
void SV_BeginDownload(client_t*);
void SV_GameCommand_f(client_t *cl) {

}
void SV_CoDExtended_f(client_t*);

static ucmd_t ucmds[] = {
	{"userinfo", (void*)0x8087B28},
	{"disconnect", (void*)0x8087AF8},
	{"cp", SV_VerifyPaks_f},
	{"vdr", (void*)0x8087B14},
	{"download", SV_BeginDownload},
	{"nextdl", (void*)0x8086168},
	{"stopdl", (void*)0x8087960},
	{"donedl", (void*)0x80879FC},
	{"retransdl", (void*)0x8087A2C},
	
	
	{"gc", SV_CoDExtended_f},
	{"codextended", SV_CoDExtended_f},
	{NULL, NULL}
};

client_t *clients = (client_t*)svsclients_ptr;

typedef void (*SV_SendClientGameState_t)(client_t*);
typedef void (*SV_SendClientSnapshot_t)(client_t*);

SV_SendClientGameState_t SV_SendClientGameState = (SV_SendClientGameState_t)0x8085EEC;
SV_SendClientSnapshot_t SV_SendClientSnapshot = (SV_SendClientSnapshot_t)0x808F844;

SV_StopDownload_f_t SV_StopDownload_f = (SV_StopDownload_f_t)0x8087960;
SV_BeginDownload_f_t SV_BeginDownload_f = (SV_BeginDownload_f_t)0x8087A64;
getuserinfo_t getuserinfo = (getuserinfo_t)0x808B25C;
setuserinfo_t setuserinfo = (setuserinfo_t)0x808B1D0;
SV_DropClient_t SV_DropClient = (SV_DropClient_t)0x8085CF4;

SV_FreeClientScriptId_t SV_FreeClientScriptId = (SV_FreeClientScriptId_t)0x808D34C;

challenge_t *challenges = (challenge_t*)0x83B67F8;

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

bool is_good_string(char* str) {
	int i;
	for(i = 0; i < strlen(str); i++)
		if(str[i] < 32 || str[i] > 126)
			return 0;
	return 1;
}

void SV_AuthorizeIpPacket( netadr_t from ) {
	int challenge;
	int i;
	char    *s;
	char    *r;
	int guid;
	char ret[1024];

	int* svs_time = (int*)0x83B67A4;
	
	if ( !NET_CompareBaseAdr( from, authorizeAddress ) ) {
		Com_Printf( "SV_AuthorizeIpPacket: not from authorize server\n" );
		return;
	}

	challenge = atoi( Cmd_Argv( 1 ) );

	for ( i = 0 ; i < MAX_CHALLENGES ; i++ ) {
		if ( challenges[i].challenge == challenge ) {
			break;
		}
	}
	
	if ( i == MAX_CHALLENGES ) {
		Com_Printf( "SV_AuthorizeIpPacket: challenge not found\n" );
		return;
	}

	// send a packet back to the original client
	challenges[i].pingTime = *svs_time;
	s = Cmd_Argv( 2 );
	r = Cmd_Argv( 3 );          // reason
	guid = atoi(Cmd_Argv(4));
	
	
	aSingleBan* cur = banlist;
	if(guid!=0) {
		while(cur!=NULL) {
			if(cur->type == GUIDBAN && guid == cur->guid) {
				printf("Banned GUID [%d] with IP [%s] tried to connect.\n", guid, NET_BaseAdrToString(challenges[i].adr));
				NET_OutOfBandPrint( NS_SERVER, challenges[i].adr, "error\n%s", x_bannedmessage->string);
				return;
			}
			cur=cur->next;
		}
	}
	
	#if 0
	for(unsigned int j = 0; j < Cmd_Argc(); j++) {
		printf("%s ", Cmd_Argv(j));
	}
	printf("\n");
	#endif
	
	if ( !Q_stricmp( s, "demo" ) ) {
		/*if ( Cvar_VariableValue( "fs_restrict" ) ) {
			// a demo client connecting to a demo server
			NET_OutOfBandPrint( NS_SERVER, from,
								"challengeResponse %i", challenges[i].challenge );
			return;
		}*/
		// they are a demo client trying to connect to a real server
		NET_OutOfBandPrint( NS_SERVER, challenges[i].adr, "print\nEXE_ERR_NOT_A_DEMO_SERVER" );
		// clear the challenge record so it won't timeout and let them through
		memset( &challenges[i], 0, sizeof( challenges[i] ) );
		return;
	}
	if ( !Q_stricmp( s, "accept" ) || !x_authorize->integer) {
		x_challenges[i].guid = guid;
		
		if ( sv_onlyVisibleClients->integer ) {
			NET_OutOfBandPrint( NS_SERVER, challenges[i].adr,
								"challengeResponse %i %i", challenges[i].challenge, sv_onlyVisibleClients->integer );
		} else {
			NET_OutOfBandPrint( NS_SERVER, challenges[i].adr,
								"challengeResponse %i", challenges[i].challenge );
		}
		return;
	}
	
	if(!Q_stricmp(s, "deny")) {
		if(!strcmp(r, "CLIENT_UNKNOWN_TO_AUTH") || !strcmp(r, "BAD_CDKEY"))
			NET_OutOfBandPrint( NS_SERVER, challenges[i].adr, "needcdkey" );
		else
			NET_OutOfBandPrint( NS_SERVER, challenges[i].adr, "error\nCould not authorize with the authorize server.");
	}
	
	// clear the challenge record so it won't timeout and let them through
	memset( &challenges[i], 0, sizeof( challenges[i] ) );
}

static time_t connect_t = 0;

void SV_DirectConnect( netadr_t* from ) {
	void (*call)(netadr_t);
	*(int*)&call = 0x8085498;
	
	char userinfo[MAX_INFO_STRING];
	
	Q_strncpyz(userinfo, Cmd_Argv(1), sizeof(userinfo));
	
	char* check_name = Info_ValueForKey(userinfo, "name");
	
	if(!is_good_string(check_name)) {
		NET_OutOfBandPrint( NS_SERVER, *from, "error\nIllegal name.");
		return;
	}
	
	aSingleBan* cur = banlist;
	
	while(cur!=NULL) {
		if(cur->type == IPBAN && NET_CompareBaseAdr(*from, cur->adr)) {
			NET_OutOfBandPrint( NS_SERVER, *from, "error\n%s", x_bannedmessage->string);
			printf("Banned IP [%s] tried to connect.\n", NET_BaseAdrToString(*from));
			return;
		}
		cur=cur->next;
	}
	
	/*
	if(connect_t==0)
		connect_t=time(NULL);
	
	
	time_t diff = time(NULL) - connect_t;
	if(diff < 10)
		return;
	connect_t = time(NULL);*/
	
	int svs_time = *(int*)0x83B67A4;
	x_challenge *xc;
	int i;
	
	xc = &x_challenges[0];
	for ( i = 0 ; i < MAX_CHALLENGES ; i++, xc++ ) {
		if ( NET_CompareAdr( *from, xc->adr ) ) {
				if(svs_time - xc->time < x_connect_message_time->integer)
					return;
			break;
		}
	}
	
	#if 0
	if ( i == MAX_CHALLENGES ) {
		NET_OutOfBandPrint( NS_SERVER, *from, "error\nNo or bad challenge for address.\n" );
		return;
	}
	#endif
	
	call(*from);
	
	/*char userinfo[MAX_INFO_STRING];
	int i;
	client_t    *cl, *newcl;
	client_t temp;
	sharedEntity_t *ent;
	int clientNum;
	int version;
	int qport;
	int challenge;
	char        *password;
	int startIndex;
	char        *denied;
	int count;

	int* svs_time = (int*)0x83B67A4;
	
	Com_DPrintf( "SVC_DirectConnect ()\n" );

	Q_strncpyz( userinfo, Cmd_Argv( 1 ), sizeof( userinfo ) );
	
	version = atoi( Info_ValueForKey( userinfo, "protocol" ) );
	if ( version != protocol->integer ) {
		NET_OutOfBandPrint( NS_SERVER, from, va("error\nThis server is running version %s\nand you have patch 1.%d.", shortversion->string, version));
		Com_DPrintf( "    rejected connect from version %i\n", version );
		return;
	}

	challenge = atoi( Info_ValueForKey( userinfo, "challenge" ) );
	qport = atoi( Info_ValueForKey( userinfo, "qport" ) );

	
	if ( SV_TempBanIsBanned( from ) ) {
		NET_OutOfBandPrint( NS_SERVER, from, "print\n%s\n", sv_tempbanmessage->string );
		return;
	}
	

	Com_DPrintf("debug: atleast made it here\n");
	
	// quick reject
	for ( i = 0; i < sv_maxclients->integer; i++) {
		cl = getclient(i);
		if ( NET_CompareBaseAdr( from, cl->remoteAddress )
			 && ( *(int*)(cl+84531) == qport
				  || from.port == cl->remoteAddress.port ) ) {
			if ( ( *svs_time - *(int*)(cl + 68368) )
				 < ( sv_reconnectlimit->integer * 1000 ) ) {
				Com_DPrintf( "%s:reconnect rejected : too soon\n", NET_AdrToString( from ) );
				return;
			}
			break;
		}
	}

	Com_DPrintf("debug: atleast made it here 2\n");
	
	// see if the challenge is valid (local clients don't need to challenge)
	if ( !NET_IsLocalAddress( from ) ) {
		int ping;


		for ( i = 0 ; i < MAX_CHALLENGES ; i++ ) {
			if ( NET_CompareAdr( from, challenges[i].adr ) ) {
				if ( challenge == challenges[i].challenge ) {
					break;      // good
				}
			}
		}
		if ( i == MAX_CHALLENGES ) {
			NET_OutOfBandPrint( NS_SERVER, from, "error\nNo or bad challenge for address.\n" );
			return;
		}
		// force the IP key/value pair so the game can filter based on ip
		Info_SetValueForKey( userinfo, "ip", NET_AdrToString( from ) );

		if ( challenges[i].firstPing == 0 ) {
			ping = *svs_time - challenges[i].pingTime;
			challenges[i].firstPing = ping;
		} else {
			ping = challenges[i].firstPing;
		}

		char* tmpname = Info_ValueForKey(userinfo, "name");
		
		Com_Printf( "Client %i (%s) connecting with %i challenge ping\n", i, tmpname, ping );
		challenges[i].connected = qtrue;

		// never reject a LAN client based on ping
		//if ( !Sys_IsLANAddress( from ) ) {
		if(1) {
			if ( sv_minPing->value && ping < sv_minPing->value ) {
				NET_OutOfBandPrint( NS_SERVER, from, "error\nServer is for high pings only\n" );
				Com_DPrintf( "Client %i rejected on a too low ping\n", i );
				return;
			}
			if ( sv_maxPing->value && ping > sv_maxPing->value ) {
				NET_OutOfBandPrint( NS_SERVER, from, "error\nServer is for low pings only\n" );
				Com_DPrintf( "Client %i rejected on a too high ping: %i\n", i, ping );
				return;
			}
		}
		Com_DPrintf("debug: atleast made it here 3\n");
	} else {
		// force the "ip" info key to "localhost"
		Info_SetValueForKey( userinfo, "ip", "localhost" );
	}

	newcl = &temp;
	memset( newcl, 0, clientsize);

	Com_DPrintf("debug: atleast made it here 4\n");
	// if there is already a slot for this ip, reuse it
	for ( i = 0; i < sv_maxclients->integer ; i++ ) {
		cl = getclient(i);
		if ( cl->state == CS_FREE ) {
			continue;
		}
		if ( NET_CompareBaseAdr( from, cl->remoteAddress )
			 && ( *(int*)(cl+84531) == qport
				  || from.port == cl->remoteAddress.port ) ) {
			Com_Printf( "%s:reconnect\n", NET_AdrToString( from ) );
			newcl = cl;
			SV_FreeClientScriptId(cl);
			// this doesn't work because it nukes the players userinfo

//			// disconnect the client from the game first so any flags the
//			// player might have are dropped
//			VM_Call( gvm, GAME_CLIENT_DISCONNECT, newcl - svs.clients );
			//
			goto gotnewcl;
		}
	}

	Com_DPrintf("debug: atleast made it here 5\n");
	// find a client slot
	// if "sv_privateClients" is set > 0, then that number
	// of client slots will be reserved for connections that
	// have "password" set to the value of "sv_privatePassword"
	// Info requests will report the maxclients as if the private
	// slots didn't exist, to prevent people from trying to connect
	// to a full server.
	// This is to allow us to reserve a couple slots here on our
	// servers so we can play without having to kick people.

	// check for privateClient password
	password = Info_ValueForKey( userinfo, "password" );
	if ( !strcmp( password, sv_privatePassword->string ) ) {
		startIndex = 0;
	} else {
		// skip past the reserved slots
		startIndex = sv_privateClients->integer;
	}

	newcl = NULL;
	for ( i = startIndex; i < sv_maxclients->integer ; i++ ) {
		cl = getclient(i);
		if ( cl->state == CS_FREE ) {
			newcl = cl;
			break;
		}
	}

	Com_DPrintf("DEBUG >> 6\n");
	
	if ( !newcl ) {
		if ( NET_IsLocalAddress( from ) ) {
			count = 0;
			for ( i = startIndex; i < sv_maxclients->integer ; i++ ) {
				cl = getclient(i);
				if ( cl->remoteAddress.type == NA_BOT ) {
					count++;
				}
			}
			// if they're all bots
			if ( count >= sv_maxclients->integer - startIndex ) {
				SV_DropClient( getclient(sv_maxclients->integer - 1), "only bots on server" );
				newcl = getclient(sv_maxclients->integer - 1);
			} else {
				Com_Error( ERR_FATAL, "server is full on local connect\n" );
				return;
			}
		} else {
			NET_OutOfBandPrint( NS_SERVER, from, "error\nEXE_SERVERISFULL" );
			Com_DPrintf( "Rejected a connection.\n" );
			return;
		}
	}

	Com_DPrintf("DEBUG >> 7\n");
	
	// we got a newcl, so reset the reliableSequence and reliableAcknowledge
	//cl->reliableAcknowledge = 0;
	//cl->reliableSequence = 0;
	
	Com_DPrintf("cl is %x\n", (int)cl);
	#if 0
	Com_DPrintf("67088 is %x\n", *(int*)(cl + 67088));
	Com_DPrintf("67084 is %x\n", *(int*)(cl + 67084));
    *(int *)(cl + 67088) = 0;
    *(int *)(cl + 67084) = 0;
	#endif
gotnewcl:
	Com_DPrintf("gotnewcl DEBUG >> 8\n");
	// build a new connection
	// accept the new client
	// this is the only place a client_t is ever initialized
	*newcl = temp;
	clientNum = ((int)newcl - (int)clients);
	ent = SV_GentityNum( clientNum );
	newcl->gentity = (gentity_t*)ent;
	
	int (*Scr_AllocArray)();
	*(int*)&Scr_AllocArray = 0x80A2610;
	
	*(int *)(newcl + 370928) = Scr_AllocArray();//Script_MakeArray();
	
	// save the challenge
	newcl->challenge = challenge;

	// save the address
	Netchan_Setup( NS_SERVER, (void *)(newcl + 338096), from, qport );
	// init the netchan queue

	// save the userinfo
	Q_strncpyz( newcl->userinfo, userinfo, sizeof( newcl->userinfo ) );

	// get the game a chance to reject this connection or modify the userinfo
	denied = myClientConnect(clientNum,0); // firstTime = qtrue
	if ( denied ) {
		NET_OutOfBandPrint( NS_SERVER, from, "print\n%s\n", denied );
		Com_DPrintf( "Game rejected a connection: %s.\n", denied );
		SV_FreeClientScriptId(newcl);
		return;
	}

	Com_DPrintf("DEBUG >> 9\n");
	
	SV_UserinfoChanged( newcl );

	// DHM - Nerve :: Clear out firstPing now that client is connected
	challenges[i].firstPing = 0;

	// send the connect packet to the client
	NET_OutOfBandPrint( NS_SERVER, from, "connectResponse" );

	Com_DPrintf( "Going from CS_FREE to CS_CONNECTED for %s\n", newcl->name );

	newcl->state = CS_CONNECTED;
	*(int*)(newcl + 68372) = *svs_time;//newcl->nextSnapshotTime = *svs_time;
	*(int*)(newcl + 68364) = *svs_time;//newcl->lastPacketTime = *svs_time;
	*(int*)(newcl + 68368) = *svs_time;//newcl->lastConnectTime = *svs_time;

	// when we receive the first packet from the client, we will
	// notice that it is from a different serverid and that the
	// gamestate message was not just sent, forcing a retransmit
	*(int*)(newcl + 67100) = -1;//newcl->gamestateMessageNum = -1;

	// if this was the first client on the server, or the last client
	// the server can hold, send a heartbeat to the master.
	/*count = 0;
	for ( i = 0,cl = clients ; i < sv_maxclients->integer ; i++,cl++ ) {
		if ( clients[i].state >= CS_CONNECTED ) {
			count++;
		}
	}
	
	if ( count == 1 || count == sv_maxclients->integer ) {
		SV_Heartbeat_f();
	}
	*/
}

void SV_GetChallenge( netadr_t* from ) {
	void (*chall)(netadr_t);
	*(int*)&chall = 0x8084D90;
	
	int i;
	int oldest;
	int oldestTime;
	x_challenge* xc;
	
	int svs_time = *(int*)0x83B67A4;
	
	if ( NET_CompareBaseAdr( *from, masterAddress ) )
		goto whatever_floats_your_boat;
	
	oldest = 0;
	oldestTime = 0x7fffffff;

	xc = &x_challenges[0];
	for ( i = 0 ; i < MAX_CHALLENGES ; i++, xc++ ) {
		if ( NET_CompareAdr( *from, xc->adr ) ) {
			if(svs_time - xc->time > 10000)
				xc->time = svs_time;
			break;
		}
		
		if(xc->time < oldestTime) {
			oldestTime = xc->time;
			oldest = i;
		}
	}

	if ( i == MAX_CHALLENGES ) {
		xc = &x_challenges[oldest];
		
		xc->adr = *from;
		xc->time = svs_time;
	}
	
	if(svs_time - xc->time < x_connect_message_time->integer) {
		NET_OutOfBandPrint( NS_SERVER, *from, "print\n%s\n", x_connect_message->string);
		return;
	}
	
	whatever_floats_your_boat:
	
	chall(*from);
}

#ifdef xDEBUG
void Cmd_DumpClient_f() {
	int index;
	client_t* cl;
	
	unsigned int result;
	float rf;
	char* rstr;
	int* base;
	
	if(Cmd_Argc()!=2) {
		printf("Usage: dclient <clientNum>\n");
		return;
	}
	
	index = atoi(Cmd_Argv(1));
	cl = getclient(index);
	base = (int*)(g_clients + 8900 * index);
	
	FILE* f = fopen("/home/cod/dump.txt", "wb");
	if(f) {
		fprintf(f, "DUMP\n");
		fprintf(f, "netname test = %s\n", (char*)(base + 8628));
		for(unsigned int i = 0; i < 8900; i++) {
			result = *(unsigned int*)(base + i);
			rf = *(float*)(base + i);
			rstr = (char*)(base + i);
			fprintf(f, "%d: Hex: %x, Decimal: %d, Float: %f, String: %s\n", i, result, result, rf, rstr);
		}
		fclose(f);
	}
	
	printf("Successfully dumped client %s\n", cl->name);
}
#endif

int get_client_number(client_t* cl);

void SV_UserinfoChanged( client_t* cl ) {
	char* request_name = Info_ValueForKey(cl->userinfo, "name");
	char* val;
	int* g_client;
	
	char newname[33] = {0};
	int i, j;
	
	j = 0;
	
	for(i = 0; i < strlen(request_name); i++) {
		if(i >= 31)
			break;
		if(request_name[i] < 32 || request_name[i] > 126)
			continue;
		newname[j++] = request_name[i];
	}
	
	Q_strncpyz(cl->name, newname, sizeof(cl->name));
	
	/*g_client = (int*)(g_clients + 8900 * get_client_number(cl));
	char* netname = (char*)(g_client + 2157);
	Q_strncpyz((char*)(g_client + 2157), newname, sizeof(cl->name));*/
	
	//game module won't fix the name in manual_change
	
	Info_SetValueForKey(cl->userinfo, "name", newname);
	
	val = Info_ValueForKey( cl->userinfo, "rate" );
	if ( strlen( val ) ) {
		i = atoi( val );
		cl->rate = i;
		if ( cl->rate < 1000 ) {
			cl->rate = 1000;
		} else if ( cl->rate > 90000 ) {
			cl->rate = 90000;
		}
	} else {
		cl->rate = 5000;
	}
	
	//skipping handicap since cod doesn't use it.
	
	val = Info_ValueForKey( cl->userinfo, "snaps" );
	if ( strlen( val ) ) {
		i = atoi( val );
		if ( i < 1 ) {
			i = 1;
		} else if ( i > 30 ) {
			i = 30;
		}
		cl->snapshotMsec = 1000 / i;
	} else {
		cl->snapshotMsec = 50;
	}
}

void hG_Say(gentity_t *ent, gentity_t *target, int mode, const char *msg) {
	void (*G_Say)( gentity_t *ent, gentity_t *target, int mode, const char *chatText );
	*(int*)&G_Say = GAME("G_Say");

	char line[1024] = {0};
	int i,j;
	
	j = 0;
	
	for(i = 0; i < strlen(msg); i++) {
		if(i >= 1023)
			break;
		if(msg[i] < 32 || msg[i] > 126)
			continue;
		line[j++] = msg[i];
	}
	
	if(callbackPlayerCommand) {
		Script_AddString(line);
		Script_AddInt(clientNum);
		int result = Script_ExecEntThread(clientNum, 0, callbackPlayerCommand, 2);
		Script_FreeThread(result);
	}
	
	if(!Scr_Continue())
		return;
	
	G_Say(ent, NULL, mode, line);
}

int last_client_number = 0;

extern int callbackPlayerCommand;

qboolean QDECL SV_ClientCommand(client_t *cl, msg_t *msg) {
	int seq, clientNum;
	const char *s;
	char *cmd;
	
	qboolean clientOk = qtrue;
	qboolean floodprotect = qtrue;

	clientNum = get_client_number(cl);
	
	seq = MSG_ReadLong(msg);
	s = MSG_ReadString(msg);

	if(cl->lastClientCommand >= seq)
		return qtrue;

	Com_DPrintf( "clientCommand: %s : %i : %s\n", cl->name, seq, s );

	if(seq > cl->lastClientCommand + 1) {
		Com_Printf( "Client %s lost %i clientCommands\n", cl->name,
					seq - cl->lastClientCommand + 1 );
		SV_DropClient( cl, "EXE_LOSTRELIABLECOMMANDS" );
		return qfalse;
	}

	if(!strncmp("team", s, 4) || !strncmp("score", s, 5) || !strncmp("mr", s, 2))
		floodprotect = qfalse;

	int *svs_time = (int*)0x83B67A4;

	if(cl->state >= CS_ACTIVE && *svs_time < *(int*)(&cl->state + 68360) && floodprotect)
		clientOk = qfalse;

	if(floodprotect)
		*(int*)(&cl->state + 68360) = *svs_time + 800;

	ucmd_t *u;
	qboolean bProcessed = qfalse;

	Cmd_TokenizeString(s);

	cmd = Cmd_Argv(0);
	
	for( u = ucmds; u->name; u++) {
		if(!strcmp(cmd, u->name)) {
			u->func(cl);
			bProcessed = qtrue;
			break;
		}
	}

	if(clientOk) {
		if(!u->name && *(int*)0x8355260 == 2) {
			
			//long long timestamp = current_timestamp();
	
			#define JS_PLAYERCOMMAND "player_command"
			
			#ifdef BUILD_ECMASCRIPT
			if(strcmp(cmd,"codextended")) {
				duk_push_global_object(ctx);
				if(duk_has_prop_string(ctx, -1, JS_PLAYERCOMMAND)) {
					duk_get_prop_string(ctx, -1, JS_PLAYERCOMMAND);
					duk_push_int(ctx, clientNum);
					duk_call(ctx, 1);
					int duk_ret_val = duk_to_int(ctx, -1);
					if(duk_ret_val)
						return;
				}
				duk_pop(ctx);
			}
			#endif
		
				
			VM_Call(*(int*)0x80E30C4, 6, get_client_number( cl ));
			//VM_Call(gvm, GAME_CLIENT_COMMAND, get_client_number( cl ));
			//((int (*)(int,...))GAME("vmMain"))(6,get_client_number(cl));
			//((void (QDECL *)(int))GAME("ClientCommand"))(get_client_number(cl));
			
		}
	}

	cl->lastClientCommand = seq;
	Com_sprintf(cl->lastClientCommandString, 0x400, "%s", s);
	return qtrue;
	
	#if 0

	last_client_number = (((int)&cl->state - *(int*)svsclients_ptr) / clientsize);
	Com_DPrintf("last_client_number = %d\n", last_client_number);

	return ((qboolean (*) (client_t*, msg_t*))0x8086E08)(cl, msg);
	#endif
}

void SV_CoDExtended_f( client_t *cl ) {
	SV_SendServerCommand(cl, 0, "e \"This server is powered by CoDExtended.\n^2Thanks for playing %s\"", cl->name);
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
    client_t *cl = (client_t*)(*(int*)clients+num*clientsize);
	//return &clients[num];
	return cl;
}

int CL_GetGuid(client_t* cl) {
	challenge_t* challenge;
	int guid = 0;
	challenge = &challenges[0];
	for (int j = 0 ; j < MAX_CHALLENGES ; j++, challenge++ ) {
		if (NET_CompareAdr( cl->remoteAddress, challenge->adr ) ) {
			guid = x_challenges[j].guid;
			break;
		}
	}
	return guid;
}

//yes i know i can just make a macro ._.
int get_client_number(client_t* cl) {
	return (((int)&cl->state - *(int*)svsclients_ptr) / clientsize);
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

void SV_VerifyPaks_f( client_t *cl ) {
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
	
	/*
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
	}*/
	int clientNum = get_client_number(cl);
	xtnded_clients[clientNum].pure = pure;
}

#ifdef xDEBUG
void SV_DumpUcmd() {
	printf("Listing ucmds\n");
	ucmd_t  *u;
	for ( u = ucmds ; u->name ; u++ ) {
		printf("ucmd: %s [%x]\n", u->name, (int)u->func);
	}
}
#endif

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

#if 0
void clientInit() {
	/*
	ucmd_t  *u;
	for ( u = ucmds ; u->name ; u++ ) {
		if(!strcmp("download", u->name)) {
			u->func = SV_BeginDownload;
		} else if(!strcmp("cp", u->name)) {
			u->func = SV_VerifyPaks_f;
		}
		printf("ucmd: %s [%x]\n", u->name, (int)u->func);
	}*/
	
	//*(int*)0x80EE704 = (int)SV_BeginDownload;
	#ifdef xDEBUG
	Cmd_AddCommand("dumpucmd", SV_DumpUcmd);
	Cmd_AddCommand("dclient", Cmd_DumpClient_f);
	#endif
}
#endif
