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

#ifndef SERVER_H
#define SERVER_H

/*
    Still not quite happy with the including of some files.
    //richard
*/

#include "shared.h"
#include "script.h"

/*
    pthreads for HeartBeat in threads
    unistd for using sleep while development/testing 
    //Nuke (t-knapp)
*/

#include <pthread.h>
//#include <unistd.h>

#if CODPATCH == 1
#define svsclients_ptr 0x83B67AC
#define clientsize 370940
#else
#define svsclients_ptr 0x83CCD90
#define clientsize 371124
#endif

#define SVF_NOCLIENT 0x1


#if CODPATCH == 1
#define svs_time (*(int*)0x83B67A4)
#else if CODPATCH == 5
#define svs_time (*(int*)0x83CCD88)
#endif

extern int clientversion;

typedef enum {
	svc_bad,
	svc_nop,
	svc_gamestate,
	svc_configstring,           // [short] [string] only in gamestate messages
	svc_baseline,               // only in gamestate messages
	svc_serverCommand,          // [string] to be executed by client game module
	svc_download,               // [short] size [size bytes]
	svc_snapshot,
	svc_EOF
} svc_ops_e; //not really server only it's for client aswell

static char *svc_strings[256] = {
	"svc_bad",
	"svc_nop",
	"svc_gamestate",
	"svc_configstring",
	"svc_baseline",
	"svc_serverCommand",
	"svc_download",
	"svc_snapshot",
	"svc_EOF"
};

typedef enum {
	NA_BOT,
	NA_BAD,                 // an address lookup failed
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	NA_IPX,
	NA_BROADCAST_IPX
} netadrtype_t;

typedef enum {
	NS_CLIENT,
	NS_SERVER
} netsrc_t;

#pragma pack(1)

typedef struct {
	netadrtype_t type;

	byte ip[4];
	byte ipx[10];

	unsigned short port;
} netadr_t; //size = 0x14 (20)

#pragma pack(push, 1)

typedef enum {
	GUIDBAN,
	IPBAN
} ban_types;

typedef struct {
	int type; //ban_types
	int guid;
	netadr_t adr;
	char reason[128];
} banInfo_t;

extern LinkedList banlist;

extern char x_mastername[14];
extern netadr_t x_master;

#define MAX_CHALLENGES 1024

#pragma pack(1)

typedef struct {
	netadr_t adr; //0
	int challenge; //20
	int time; //24
	int pingTime; //28
	int firstTime; //32
	int firstPing; //36
	int connected; //40
	#if CODPATCH == 5
	int guid; //44
	unsigned char __idk[36];
	#endif
} challenge_t;

#pragma pack(push, 1)

//Argument struct passed to heartbeat thread
typedef struct {
    const char *hbname;
    netadr_t adr;
} heartbeat_t;

typedef int sharedEntity_t;

typedef sharedEntity_t* (*SV_GentityNum_t)( int num );
extern SV_GentityNum_t SV_GentityNum;

typedef void (*SV_SetConfigstring_t)( int index, const char *val );
typedef void (*SV_GetConfigstring_t)( int index, char *buffer, int bufferSize );
extern SV_GetConfigstring_t SV_GetConfigstring;
extern SV_SetConfigstring_t SV_SetConfigstring;


void SV_GetChallenge( netadr_t* from );
void SV_Init( void );
void SV_DirectConnect( netadr_t* from );
void SV_AuthorizeIpPacket( netadr_t from );

void SV_MasterHeartBeat(const char*);

extern cvar_t  *sv_maxclients;
extern cvar_t *sv_privateClients;
extern cvar_t *g_gametype;
extern cvar_t *mapname;
extern cvar_t *sv_hostname;
extern cvar_t *sv_maxRate;
extern cvar_t *sv_maxPing;
extern cvar_t *sv_minPing;
extern cvar_t *sv_floodProtect;
extern cvar_t *sv_allowAnonymous;
extern cvar_t *sv_showCommands;
extern cvar_t *sv_pure;
extern cvar_t *sv_serverid;
extern cvar_t *rconPassword;
extern cvar_t *sv_privatePassword;
extern cvar_t *sv_fps;
extern cvar_t *sv_timeout;
extern cvar_t *sv_zombietime;
extern cvar_t *sv_allowDownload;
extern cvar_t *sv_master1;
extern cvar_t *sv_master2;
extern cvar_t *sv_master3;
extern cvar_t *sv_master4;
extern cvar_t *sv_master5;
extern cvar_t *sv_reconnectlimit;
extern cvar_t *sv_showloss;
extern cvar_t *sv_padPackets;
extern cvar_t *sv_killserver;
extern cvar_t *sv_onlyVisibleClients;
extern cvar_t *sv_showAverageBPS;
extern cvar_t *sv_mapRotation;
extern cvar_t *sv_mapRotationCurrent;
extern cvar_t *shortversion;
extern cvar_t *protocol;
extern cvar_t* dedicated;
extern cvar_t* sv_running;

#if CODPATCH == 5
extern cvar_t *sv_disableClientConsole;
#endif

extern cvar_t *x_requireclient;
extern cvar_t *x_requireveritas;
extern cvar_t *x_globalbans;
extern cvar_t *x_bannedmessage;
extern cvar_t *x_contents;
extern cvar_t *x_deadchat;
extern cvar_t *x_authorize;
extern cvar_t *x_spectator_noclip;
extern cvar_t *x_connectmessage;

extern cvar_t *cl_allowDownload; //the client will locally change any cvars to match the SYSTEMINFO cvars

#define MAX_MASTER_SERVERS 5
extern cvar_t* sv_master[MAX_MASTER_SERVERS];

extern char x_print_connect_message[1024];
extern char SVC_CHANDELIER[12];

typedef void (*Netchan_Setup_t)( netsrc_t sock, void*/*netchan_t*/ chan, netadr_t adr, int qport );
extern Netchan_Setup_t Netchan_Setup;

typedef void (QDECL *NET_OutOfBandPrint_t)( netsrc_t net_socket, netadr_t adr, const char *format, ... );
extern NET_OutOfBandPrint_t NET_OutOfBandPrint;

typedef qboolean (*NET_StringToAdr_t)( const char *s, netadr_t *a );
extern NET_StringToAdr_t NET_StringToAdr;

const char  *NET_BaseAdrToString (netadr_t a);
const char  *NET_AdrToString (netadr_t a);
qboolean    NET_CompareAdr( netadr_t a, netadr_t b );
qboolean    NET_CompareBaseAdr( netadr_t a, netadr_t b );
bool	   NET_IsLocalAddress( netadr_t adr );

typedef void (*NET_SendPacket_t)( netsrc_t sock, int length, const void *data, netadr_t to );
extern NET_SendPacket_t NET_SendPacket;

//void QDECL NET_OutOfBandPrint( netsrc_t sock, netadr_t adr, const char *format, ... );

extern netadr_t authorizeAddress;
extern netadr_t masterAddress;

typedef enum {
	CS_FREE,        // can be reused for a new connection
	CS_ZOMBIE,      // client has been disconnected, but don't reuse connection for a couple seconds
	CS_CONNECTED,   // has been assigned to a client_t, but no gamestate yet
	CS_PRIMED,      // gamestate has been sent, but client hasn't sent a usercmd
	CS_ACTIVE       // client is fully in game
} clientState_t;

#pragma pack(1)

typedef struct usercmd_s {
	int serverTime;
	byte buttons; //console,chat talking, aim down the sight, attackbutton, usebutton
	byte wbuttons; //lean right,left,reload
	byte weapon;
	byte flags;
	int angles[3];

	signed char forwardmove, rightmove, upmove;
	byte unknown; //could be doubleTap or client
} usercmd_t;

typedef struct { //usercmd_s i defined in server.h mmmmmmm
	playerState_t *ps;
	usercmd_t cmd;
	//other stuff
} pmove_t;

extern pmove_t *pm;

#define iprintln(m) SV_SendServerCommand(NULL, 0, "e \"%s\"", m)

typedef struct client_s {
	clientState_t state;
	int unknown4;
	int unknown8;
	char userinfo[1024];
	//here is some servercommands sent but i do not need them
	//and cba to figure out
	char unk[0x10214];
	int challenge;
	usercmd_t lastUsercmd;

	int lastClientCommand;
	char lastClientCommandString[1024];
	int gentity; //sharedEntity?
	char name[32];
	char downloadName[64];
	int download;
	int downloadSize;
	int downloadCount;
	char junk[0x41DF0];
	int ping;
	int rate;
	int snapshotMsec;
	char junk2[12];
	/*int netadrtype; //also start of netadr_t
	byte ipv4[4];*/
	netadr_t remoteAddress;
	int qport;
	
	char lazy_to_figure_out_so_fill_it_up[32812];//to initialize client_t structs properly
} client_t;

typedef struct animation_s {
	char name[64]; //pb_combatwalk_left_loop_pistol
	int a; //4294967295
	int b; //31
	int c; //733
	int d; //426038
	int e; //144
	int f; //16
	int g; //0
} animation_t;

#pragma pack(push, 1)

/*
from 1.5

typedef enum {
  UCMD_BUTTONS = 8, //for messagemode/console, cl_run (+speed) (aim down the sight)
  UCMD_WBUTTONS, //+reload, +leanright +leanleft
  UCMD_FORWARDMOVE = 23,
  UCMD_RIGHTMOVE,
  UCMD_UPMOVE
} usercmd_offset;


typedef struct usercmd_s {
	int serverTime;
	byte buttons;
	byte wbuttons;
	byte weapon;
	byte flags;
    byte unknown1[13];
    / *
        forward = 127
        back = 129
        right = 127
        left = 129
        up = 127
        prone = 129
    * /
	signed char forwardmove, rightmove, upmove;
	byte doubleTap;             // Arnout: only 3 bits used

	// rain - in ET, this can be any entity, and it's used as an array
	// index, so make sure it's unsigned
	byte identClient;           // NERVE - SMF
} usercmd_t;
*/

typedef struct {
	long long chattimer;
	long long commandtimer;
	int uid;
	bool pure;
	int clientusage;
	
	qboolean namemuted; //can this player rename?
	qboolean muted; //can this player chat?
	
	int perks[MAX_PERKS];
	int sprinting;
} xtnded_client, x_client;

#define xclient_t x_client

typedef struct {
	netadr_t adr;
	int challenge;
	int time;
	int guid;
} x_challenge;

extern x_challenge x_challenges[MAX_CHALLENGES];

extern xtnded_client xtnded_clients[64];

#define x_clients xtnded_clients
#define xclients x_clients

extern client_t* clients;

extern cvar_t  *sv_maxclients;

#if 0
extern std::vector<netadr_t> bannedIPs;
extern std::vector<int> bannedGUIDs;
#endif
extern challenge_t* challenges;

/*
	x commands
*/

void Cmd_CallVote(int);

typedef void (__cdecl *SV_StopDownload_f_t)(client_t*);
extern SV_StopDownload_f_t SV_StopDownload_f;

typedef void (*SV_BeginDownload_f_t)(client_t*);
extern SV_BeginDownload_f_t SV_BeginDownload_f;

typedef void (*SV_DropClient_t)(client_t*, const char* reason);
extern SV_DropClient_t SV_DropClient;

typedef void (*SV_FreeClientScriptId_t)(client_t*);
extern SV_FreeClientScriptId_t SV_FreeClientScriptId;

typedef int (*SV_GetClientScore_t)(client_t*);
extern SV_GetClientScore_t SV_GetClientScore;

void SV_Status_f();
void SV_UserinfoChanged( client_t* cl );

#define SHOWMSG_MSEC 2050

int CL_GetGuid(client_t* cl);
client_t* getclient(int);
char	*ConcatArgs( int start );
typedef void (*SV_Trace_t)( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask );
typedef void (QDECL *SV_SendServerCommand_t)(client_t*, int, const char*, ...); //actually is gamesendservercommand which then checks and then calls sendservercommand
typedef void (*getuserinfo_t)( int index, char *buffer, int bufferSize );
typedef void (*setuserinfo_t)(int,const char*);
extern SV_Trace_t SV_Trace;
extern SV_SendServerCommand_t SV_SendServerCommand;
extern getuserinfo_t getuserinfo;
extern setuserinfo_t setuserinfo;
#endif // SERVER_H
