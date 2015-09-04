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
#include <sys/stat.h>

SV_SetConfigstring_t SV_SetConfigstring = (SV_SetConfigstring_t)0x8089BF0;
SV_GetConfigstring_t SV_GetConfigstring = (SV_GetConfigstring_t)0x808B05C;

netadr_t authorizeAddress;
netadr_t masterAddress;

LinkedList banlist = NULL;

void X_ReadBannedList(bool print_t) {
	FILE *f;
	netadr_t adr;
	banInfo_t *ban;
	char buf[256];
	char* bufp = NULL;
	
	list_clear(&banlist);
	
	f = fopen("ipbans.txt", "r");
	if(f) {
		while(fgets(buf, sizeof(buf), f)) {
			char *reason = strchr(buf, ':');
			if(reason != NULL)
				*reason++ = '\0';
			bufp = utrim(buf);
			if(!strlen(bufp))
				continue;
			NET_StringToAdr(bufp, &adr);
			ban = (banInfo_t*)xalloc(sizeof(banInfo_t));
			*ban->reason = 0;
			ban->type = IPBAN;
			ban->adr = adr;
			if(reason != NULL)
				strncpy(ban->reason, reason, sizeof(ban->reason));
			_list_add(&banlist, ban);
			if(print_t)
				printf("Added IP '%s' to the list.\n", NET_BaseAdrToString(adr));
		}
		fclose(f);
	}
	
	f = fopen("muidbans.txt", "r");
	
	if(f) {
		while(fgets(buf, sizeof(buf), f)) {
			bufp = utrim(buf);
			if(!strlen(bufp))
				continue;
			if(atoi(bufp) == 0)
				continue;
			ban = (banInfo_t*)xalloc(sizeof(banInfo_t));
			*ban->reason = 0;
			ban->type = MUIDBAN;
			Q_strncpyz(ban->mUID, bufp, sizeof(ban->mUID));
			_list_add(&banlist, ban);
			if(print_t)
				cprintf(PRINT_INFO, "Added mUID '%s' to the list.\n", bufp);
		}
		fclose(f);
	}
}

cvar_t *developer;

void SV_Init( void ) {
	void (*init)( void );
	#if CODPATCH == 1
	*(int*)&init = 0x808A94C;
	#else if CODPATCH == 5
	*(int*)&init = 0x80913B3;
	#endif
	
	init();
	
	if(clientsize != sizeof(client_t)) {
		
		cprintf(PRINT_UNDERLINE | PRINT_ERR, "ERROR: client_t size doesn't fit clientsize!!! %i != %i\n", sizeof(client_t), clientsize);
	} else {
		
		//cprintf(PRINT_UNDERLINE | PRINT_GOOD, "GOOD: SIZE IS SAME OF CLIENT\n");
	}
	
	if ( NET_StringToAdr( x_mastername, &x_master ) )
		x_master.port = BigShort( 20510 );
	
	X_ReadBannedList(true);
	
	developer = Cvar_Get("developer", "0", 256);
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
	Cvar_Get("sv_wwwDownload", "1", 1);
	Cvar_Get("sv_wwwBaseURL", "", CVAR_SYSTEMINFO | 1);
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
	
	x_globalbans = Cvar_Get("x_globalbans", "1", 0);
	x_requireclient = Cvar_Get("x_requireclient", "0", CVAR_ARCHIVE);
	x_requireveritas = Cvar_Get("x_requireveritas", "0", CVAR_ARCHIVE);
	x_spectator_noclip = Cvar_Get("x_spectator_noclip", "0", CVAR_ARCHIVE);
	x_connectmessage = Cvar_Get("x_connectmessage", "", CVAR_ARCHIVE);
	cl_allowDownload = Cvar_Get("cl_allowDownload", "0", CVAR_SYSTEMINFO);
	
	Cvar_Get("rate", "25000", CVAR_SYSTEMINFO);
	Cvar_Get("snaps", "40", CVAR_SYSTEMINFO);
	
	#ifdef xDEBUG
	void Com_DPrintf_2(const char*,...);
	__call(0x808743B, (int)Com_DPrintf_2);
	__nop(0x8087447, 5); //call to sendclientgamestate
	void MSG_WriteLong_2(msg_t *msg, int v);
	__call(0x8086003, (int)MSG_WriteLong_2);
	__nop(0x8086008, 0x8086057); //for( configstrings loop < 2048
	
	void SV_ExecuteClientMessage(client_t *cl, msg_t *msg);
	__call(0x808CA57, (int)SV_ExecuteClientMessage);
	
	if(1) {//if(x_requireclient->integer && sv_allowDownload->integer) {
		Cvar_Set("cl_allowDownload", "1");
		
		const char *FS_ReferencedUpdateName();
		const char *FS_UpdateChecksum();
		const char *FS_UpdateName();
		
		__jmp(0x80717A4, (int)FS_UpdateChecksum);
		__jmp(0x8071664, (int)FS_UpdateChecksum);
		__jmp(0x80716CC, (int)FS_ReferencedUpdateName);
		__jmp(0x8071600, (int)FS_UpdateChecksum); //FS_LoadedPakChecksums
		__jmp(0x8071580, (int)FS_UpdateName); //FS_LoadedPakNames
		
		struct stat st;
		char *localName = va("./%s.pk3", FS_ReferencedUpdateName());
		if(stat(localName, &st)) {
			if(!download_file(CL_UPDATE_PAK_LINK, localName)) {
				cprintf(PRINT_ERR|PRINT_UNDERLINE, "Error! Trying to use client as requirement, cannot download update pak!\n");
				COD_Destructor();
			}
			((void(*)(int))0x80621E4)(0);
		}
	}
	#endif
	
	extern cvar_t *x_cl_adsair;
	x_cl_adsair = Cvar_Get("x_cl_adsair", "1", 0);
	
	cvar_t *x_nopbots = Cvar_Get("x_nopbots", "0", 0);
	/*
		NOP SV_BotUsermove calls
	*/
	
	if(x_nopbots->integer) {
		__nop(0x808D152, 5);
		__nop(0x808D492, 5);
	}
	
	#if CODPATCH == 5
	sv_disableClientConsole = Cvar_Get("sv_disableClientConsole", "0", 8);
	#endif
	
	x_authorize = Cvar_Get("x_authorize", "0", 0);
	
	x_contents = Cvar_Get("x_contents", "-1", 0);
	
	Cvar_Get("codextended", va("CoDExtended v%d", CURRENTBUILD), CVAR_SERVERINFO | CVAR_ROM | CVAR_NORESTART);
	//Cvar_Get("codextended", "This server is powered by CoDExtended.", CVAR_SERVERINFO | CVAR_ROM | CVAR_NORESTART);
	
	x_bannedmessage = Cvar_Get("x_bannedmsg", "You have been banned from this server.", 0);
	
	sv_master[0] = Cvar_Get("sv_master1", "codmaster.activision.com", 0);
	sv_master[1] = Cvar_Get("sv_master2", "", CVAR_ARCHIVE);
	sv_master[2] = Cvar_Get("sv_master3", "", CVAR_ARCHIVE);
	sv_master[3] = Cvar_Get("sv_master4", "", CVAR_ARCHIVE);
	sv_master[4] = Cvar_Get("sv_master5", "", CVAR_ARCHIVE);
	
	#define MASTER_SERVER_NAME "codmaster.activision.com"
	
	cprintf(PRINT_GOOD, "Resolving %s\n", MASTER_SERVER_NAME );
	if ( NET_StringToAdr( MASTER_SERVER_NAME, &masterAddress ) )
		masterAddress.port = BigShort( 20510 );
				
	#define AUTHORIZE_SERVER_NAME "codauthorize.activision.com"
	
	if ( !authorizeAddress.ip[0] && authorizeAddress.type != NA_BAD ) {
		cprintf(PRINT_GOOD, "Resolving %s\n", AUTHORIZE_SERVER_NAME );
		if ( !NET_StringToAdr( AUTHORIZE_SERVER_NAME, &authorizeAddress ) ) {
			cprintf(PRINT_ERR, "Couldn't resolve address\n" );
			return;
		}
		authorizeAddress.port = BigShort( 20500 );
		cprintf(PRINT_GOOD, "%s resolved to %i.%i.%i.%i:%i\n", AUTHORIZE_SERVER_NAME,
					authorizeAddress.ip[0], authorizeAddress.ip[1],
					authorizeAddress.ip[2], authorizeAddress.ip[3],
					BigShort( authorizeAddress.port ) );
	}
	
	#ifdef BUILD_ECMASCRIPT
		js_init();
	#endif
}

void SV_Shutdown(char *finalmsg) {
	void (*o)(char*) = (void(*)(char*))0x808AD8C;
	o(finalmsg);
	
	js_destroy();
}

void SV_SpawnServer(char *server) {
	void (*spawnserver)(char*) = (void(*)(char*))0x808A220;
	spawnserver(server);
	
	js_load();
}