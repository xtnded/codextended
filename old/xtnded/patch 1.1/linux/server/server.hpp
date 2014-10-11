#ifndef SERVER_H
#define SERVER_H

/*
    Still not quite happy with the including of some files.
    //richard
*/

#include "../game/shared.hpp"
#include "../game/script.hpp"

#define svsclients_ptr 0x83B67AC
#define clientsize 370940

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

typedef struct {
	netadrtype_t type;

	byte ip[4];
	byte ipx[10];

	unsigned short port;
} netadr_t;

/*
typedef void (QDECL *NET_OutOfBandPrint_t)( netsrc_t net_socket, netadr_t adr, const char *format, ... );
extern NET_OutOfBandPrint_t NET_OutOfBandPrint;
*/

typedef void (*NET_SendPacket_t)( netsrc_t sock, int length, const void *data, netadr_t to );
extern NET_SendPacket_t NET_SendPacket;

void QDECL NET_OutOfBandPrint( netsrc_t sock, netadr_t adr, const char *format, ... );

typedef enum {
	CS_FREE,        // can be reused for a new connection
	CS_ZOMBIE,      // client has been disconnected, but don't reuse connection for a couple seconds
	CS_CONNECTED,   // has been assigned to a client_t, but no gamestate yet
	CS_PRIMED,      // gamestate has been sent, but client hasn't sent a usercmd
	CS_ACTIVE       // client is fully in game
} clientState_t;

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
	int* gentity;
	char name[32];
	char downloadName[64];
	int download;
	int downloadSize;
	int downloadCount;
	char junk[0x41DF0];
	int ping;
	int rate;
	char junk2[16];
	/*int netadrtype; //also start of netadr_t
	byte ipv4[4];*/
	netadr_t remoteAddress;
} client_t;

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

typedef struct client_s {
	clientState_t state;
	int unknown4;
	int unknown8;
	char userinfo[MAX_INFO_STRING];
	byte unknown1032[66064];
	int challenge;
    byte inaccuratelastUserCmd[28];
    int lastClientCommand;
    char lastClientCommandString[MAX_STRING_CHARS];
    int* gentity; //sharedEntity_t  *gentity;
    char name[MAX_NAME_LENGTH];
    byte unknown1[270024];
    int ping;
    int rate;
    int snapshotMsec;                   // requests a snapshot every snapshotMsec unless rate choked
	int pureAuthentic;
	qboolean gotCP;  // TTimo - additional flag to distinguish between a bad pure checksum, and no cp command at all
    int unknown2;
    int dropped;
    byte remoteAddress[4];
} client_t;
*/

typedef struct xtnded_client {
	long long chattimer;
	long long commandtimer;
	int uid;
	bool pure;
};

extern xtnded_client xtnded_clients[64];

extern int* clients;

extern cvar_t  *sv_maxclients;

typedef void (__cdecl *SV_StopDownload_f_t)(client_t*);
extern SV_StopDownload_f_t SV_StopDownload_f;

typedef void (*SV_BeginDownload_f_t)(client_t*);
extern SV_BeginDownload_f_t SV_BeginDownload_f;

typedef void (*SV_DropClient_t)(client_t*, const char* reason);
extern SV_DropClient_t SV_DropClient;

void get_client_ip(int, char*);
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
