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

SV_SetConfigstring_t SV_SetConfigstring = (SV_SetConfigstring_t)0x8089BF0;
SV_GetConfigstring_t SV_GetConfigstring = (SV_GetConfigstring_t)0x808B05C;

netadr_t authorizeAddress;
netadr_t masterAddress;

aSingleBan *banlist = NULL;

bool bl_create() {
	if(banlist!=NULL) {
		//printf("ERROR: Banlist is not NULL.\n");
		return 0;
	}
	banlist = (aSingleBan*)malloc(sizeof(aSingleBan));
	banlist->next = NULL;
	return 1;
}

void bl_clear() {
	if(banlist==NULL) {
		//Com_Printf("ERROR: Trying to clear an empty banlist.\n");
		return;
	}
	aSingleBan *tmp;
	aSingleBan *cur = banlist;
	while(cur!=NULL) {
		tmp = cur;
		cur=cur->next;
		free(tmp);
		tmp = NULL;
	}
	banlist = NULL;
}

aSingleBan* bl_push() {
	if(banlist == NULL) {
		//printf("ERROR: Trying to push a value onto a NULLed banlist.\n");
		return NULL;
	}
	
	aSingleBan *cur = banlist;
	while(cur->next!=NULL)
		cur = cur->next;
	cur->next = (aSingleBan*)malloc(sizeof(aSingleBan));
	cur->next->next = NULL;
	return cur;
}

void X_ReadBannedList(bool print_t) {
	FILE *f;
	netadr_t adr;
	aSingleBan *ban;
	char buf[256];
	char* bufp = NULL;
	
	bl_clear();
	
	bl_create();
	
	f = fopen("ipbans.txt", "r");
	if(f) {
		while(fgets(buf, sizeof(buf), f)) {
			bufp = utrim(buf);
			if(!strlen(bufp))
				continue;
			NET_StringToAdr(bufp, &adr);
			ban = bl_push();
			ban->type = IPBAN;
			ban->adr = adr;
			//if(print_t)
				//printf("Added IP '%s' to the list.\n", NET_BaseAdrToString(adr));
		}
		fclose(f);
	}
	
	f = fopen("guidbans.txt", "r");
	
	if(f) {
		while(fgets(buf, sizeof(buf), f)) {
			bufp = utrim(buf);
			if(!strlen(bufp))
				continue;
			if(atoi(bufp) == 0)
				continue;
			ban = bl_push();
			ban->type = GUIDBAN;
			ban->guid = atoi( bufp ) ;
			//if(print_t)
				//printf("Added GUID '%s' to the list.\n", bufp);
		}
		fclose(f);
	}
}

netadr_t x_master;
char x_mastername[14];

void SV_Init( void ) {
	if ( NET_StringToAdr( x_mastername, &x_master ) )
		x_master.port = BigShort( 20510 );

	void (*init)( void );
	*(int*)&init = 0x808A94C;
	
	init();
	
	X_ReadBannedList(true);
	
	sv_running = Cvar_Get("sv_running", "0", 64);
	sv_maxclients = Cvar_Get("sv_maxclients", "20", 36);
	sv_privateClients = Cvar_Get("sv_privateClients", "0", 4);
	g_gametype = Cvar_Get("g_gametype", "dm", 36);
	mapname = Cvar_Get("mapname", "nomap", 68);
	sv_hostname = Cvar_Get("sv_hostname", "CoDExtendedHost", 5);
	sv_maxRate = Cvar_Get("sv_maxRate", "0", 5);
	sv_maxPing = Cvar_Get("sv_maxPing", "0", 5);
	sv_minPing = Cvar_Get("sv_minPing", "0", 5);
	sv_floodProtect = Cvar_Get("sv_floodProtect", "1", 5);
	sv_allowAnonymous = Cvar_Get("sv_allowAnonymous", "", 4);
	sv_showCommands = Cvar_Get("sv_showCommands", "0", 0);
	sv_pure = Cvar_Get("sv_pure", "0", 12);
	sv_serverid = Cvar_Get("sv_serverid", "0", 72);
	rconPassword = Cvar_Get("rconPassword", "", 256);
	sv_privatePassword = Cvar_Get("sv_privatePassword", "", 256);
	sv_fps = Cvar_Get("sv_fps", "20", 256);
	sv_timeout = Cvar_Get("sv_timeout", "240", 256);
	sv_zombietime = Cvar_Get("sv_zombietime", "2", 256);
	sv_allowDownload = Cvar_Get("sv_allowDownload", "1", 1);
	/*sv_master1 = Cvar_Get("sv_master1", "codmaster.activision.com", 0);
	sv_master2 = Cvar_Get("sv_master2", "", 1);
	sv_master3 = Cvar_Get("sv_master3", "", 1);
	sv_master4 = Cvar_Get("sv_master4", "", 1);
	sv_master5 = Cvar_Get("sv_master5", "", 1);*/
	sv_reconnectlimit = Cvar_Get("sv_reconnectlimit", "3", 0);
	sv_showloss = Cvar_Get("sv_showloss", "0", 0);
	sv_padPackets = Cvar_Get("sv_padPackets", "0", 0);
	sv_killserver = Cvar_Get("sv_killserver", "0", 0);
	sv_onlyVisibleClients = Cvar_Get("sv_onlyVisibleClients", "0", 0);
	sv_showAverageBPS = Cvar_Get("sv_showAverageBPS", "0", 0);
	sv_mapRotation = Cvar_Get("sv_mapRotation", "", 0);
	sv_mapRotationCurrent = Cvar_Get("sv_mapRotationCurrent", "", 0);
	protocol = Cvar_Get("protocol", "1", 68);
	shortversion = Cvar_Get("shortversion", "1.1", 68);
	dedicated = Cvar_Get("dedicated", "2", 64);
	x_authorize = Cvar_Get("x_authorize", "0", 0);
	
	xtnded_contents = Cvar_Get("x_contents", "-1", 0);
	x_connect_message = Cvar_Get("x_connect_message", "This server is powered by CoDExtended.", 0);
	x_connect_message_time = Cvar_Get("x_connect_message_time", "2050", 0);
	
	Cvar_Get("codextended", "This server is powered by CoDExtended.", CVAR_SERVERINFO | CVAR_ROM | CVAR_NORESTART);
	
	x_bannedmessage = Cvar_Get("x_bannedmsg", "You have been banned from this server.", 0);
	
	sv_master[0] = Cvar_Get("sv_master1", "codmaster.activision.com", 0);
	sv_master[1] = Cvar_Get("sv_master2", "", CVAR_ARCHIVE);
	sv_master[2] = Cvar_Get("sv_master3", "", CVAR_ARCHIVE);
	sv_master[3] = Cvar_Get("sv_master4", "", CVAR_ARCHIVE);
	sv_master[4] = Cvar_Get("sv_master5", "", CVAR_ARCHIVE);
	
	#define MASTER_SERVER_NAME "codmaster.activision.com"
	
	Com_Printf( "Resolving %s\n", MASTER_SERVER_NAME );
	if ( NET_StringToAdr( MASTER_SERVER_NAME, &masterAddress ) )
		masterAddress.port = BigShort( 20510 );
				
	#define AUTHORIZE_SERVER_NAME "codauthorize.activision.com"
	
	if ( !authorizeAddress.ip[0] && authorizeAddress.type != NA_BAD ) {
		Com_Printf( "Resolving %s\n", AUTHORIZE_SERVER_NAME );
		if ( !NET_StringToAdr( AUTHORIZE_SERVER_NAME, &authorizeAddress ) ) {
			Com_Printf( "Couldn't resolve address\n" );
			return;
		}
		authorizeAddress.port = BigShort( 20500 );
		Com_Printf( "%s resolved to %i.%i.%i.%i:%i\n", AUTHORIZE_SERVER_NAME,
					authorizeAddress.ip[0], authorizeAddress.ip[1],
					authorizeAddress.ip[2], authorizeAddress.ip[3],
					BigShort( authorizeAddress.port ) );
	}
}

//adding this to prevent of creating a new file net_chan.c
Netchan_Setup_t Netchan_Setup = (Netchan_Setup_t)0x808119C;

NET_OutOfBandPrint_t NET_OutOfBandPrint = (NET_OutOfBandPrint_t)0x8080920;

NET_SendPacket_t NET_SendPacket = (NET_SendPacket_t)0x8080D28;

#define MAX_MSGLEN              32768

/*
===============
NET_OutOfBandPrint

Sends a text message in an out-of-band datagram
================
*/
/*
void QDECL NET_OutOfBandPrint( netsrc_t sock, netadr_t adr, const char *format, ... ) {
	va_list argptr;
	char string[MAX_MSGLEN];

	// set the header
	string[0] = -1;
	string[1] = -1;
	string[2] = -1;
	string[3] = -1;

	va_start( argptr, format );
	vsnprintf( string + 4, sizeof( string ) - 4, format, argptr );
	va_end( argptr );

	// send the datagram
	NET_SendPacket( sock, strlen( string ), string, adr );
}
*/

NET_StringToAdr_t NET_StringToAdr = (NET_StringToAdr_t)0x8080C38;

const char  *NET_AdrToString (netadr_t a) {
    static  char    s[64];

    if (a.type == NA_LOOPBACK) {
        Com_sprintf (s, sizeof(s), "loopback");
    } else if (a.type == NA_BOT) {
        Com_sprintf (s, sizeof(s), "bot");
    } else if (a.type == NA_IP) {
        Com_sprintf (s, sizeof(s), "%i.%i.%i.%i:%hu",
            a.ip[0], a.ip[1], a.ip[2], a.ip[3], BigShort(a.port));
    } else {
        Com_sprintf (s, sizeof(s), "%02x%02x%02x%02x.%02x%02x%02x%02x%02x%02x:%hu",
        a.ipx[0], a.ipx[1], a.ipx[2], a.ipx[3], a.ipx[4], a.ipx[5], a.ipx[6], a.ipx[7], a.ipx[8], a.ipx[9], 
        BigShort(a.port));
    }

    return s;
}

const char  *NET_BaseAdrToString (netadr_t a) {
    static  char    s[64];

    if (a.type == NA_LOOPBACK) {
        Com_sprintf (s, sizeof(s), "loopback");
    } else if (a.type == NA_BOT) {
        Com_sprintf (s, sizeof(s), "bot");
    } else if (a.type == NA_IP) {
        Com_sprintf (s, sizeof(s), "%i.%i.%i.%i",
            a.ip[0], a.ip[1], a.ip[2], a.ip[3]);
    } else {
        Com_sprintf (s, sizeof(s), "%02x%02x%02x%02x.%02x%02x%02x%02x%02x%02x",
        a.ipx[0], a.ipx[1], a.ipx[2], a.ipx[3], a.ipx[4], a.ipx[5], a.ipx[6], a.ipx[7], a.ipx[8], a.ipx[9]);
    }

    return s;
}

qboolean    NET_CompareAdr( netadr_t a, netadr_t b ) {
	if ( a.type != b.type ) {
		return qfalse;
	}

	if ( a.type == NA_LOOPBACK ) {
		return qtrue;
	}

	if ( a.type == NA_IP ) {
		if ( a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] && a.ip[2] == b.ip[2] && a.ip[3] == b.ip[3] && a.port == b.port ) {
			return qtrue;
		}
		return qfalse;
	}

	if ( a.type == NA_IPX ) {
		if ( ( memcmp( a.ipx, b.ipx, 10 ) == 0 ) && a.port == b.port ) {
			return qtrue;
		}
		return qfalse;
	}

	Com_Printf( "NET_CompareAdr: bad address type\n" );
	return qfalse;
}


bool    NET_IsLocalAddress( netadr_t adr ) {
	return adr.type == NA_LOOPBACK;
}


qboolean    NET_CompareBaseAdr( netadr_t a, netadr_t b ) {
	if ( a.type != b.type ) {
		return qfalse;
	}

	if ( a.type == NA_LOOPBACK ) {
		return qtrue;
	}

	if ( a.type == NA_IP ) {
		if ( a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] && a.ip[2] == b.ip[2] && a.ip[3] == b.ip[3] ) {
			return qtrue;
		}
		return qfalse;
	}

	if ( a.type == NA_IPX ) {
		if ( ( memcmp( a.ipx, b.ipx, 10 ) == 0 ) ) {
			return qtrue;
		}
		return qfalse;
	}


	Com_Printf( "NET_CompareBaseAdr: bad address type\n" );
	return qfalse;
}
