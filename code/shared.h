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
#ifndef SHARED_H
#define SHARED_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <execinfo.h>
#include <stddef.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdint.h>
#include "cracking.h"
#include "x_util.h"

#define CURRENTBUILD 11

#ifdef uMYSQL
#include "mysql/mysql.h"
#endif
#include "x_util.h"
#include <malloc.h>
#include <mcheck.h>

typedef unsigned char byte;
typedef unsigned char boolean;
#define __cdecl __attribute__((__cdecl__))
#define QDECL __cdecl
#define __fastcall __attribute__((fastcall))

//static int ( QDECL * syscall )( int arg, ... ) = ( int ( QDECL * )( int, ... ) )0x8087DCC;

#define NOP 0x90
#define VERSION_STRING "Call of Duty Extended %.1f\n"
#define EXTENDEDVERSION 1f

#define DotProduct( x,y )         ( ( x )[0] * ( y )[0] + ( x )[1] * ( y )[1] + ( x )[2] * ( y )[2] )
#define VectorSubtract( a,b,c )   ( ( c )[0] = ( a )[0] - ( b )[0],( c )[1] = ( a )[1] - ( b )[1],( c )[2] = ( a )[2] - ( b )[2] )
#define VectorAdd( a,b,c )        ( ( c )[0] = ( a )[0] + ( b )[0],( c )[1] = ( a )[1] + ( b )[1],( c )[2] = ( a )[2] + ( b )[2] )
#define VectorCopy( a,b )         ( ( b )[0] = ( a )[0],( b )[1] = ( a )[1],( b )[2] = ( a )[2] )
#define VectorScale( v, s, o )    ( ( o )[0] = ( v )[0] * ( s ),( o )[1] = ( v )[1] * ( s ),( o )[2] = ( v )[2] * ( s ) )
#define VectorMA( v, s, b, o )    ( ( o )[0] = ( v )[0] + ( b )[0] * ( s ),( o )[1] = ( v )[1] + ( b )[1] * ( s ),( o )[2] = ( v )[2] + ( b )[2] * ( s ) )

#define VectorClear( a )              ( ( a )[0] = ( a )[1] = ( a )[2] = 0 )
#define VectorNegate( a,b )           ( ( b )[0] = -( a )[0],( b )[1] = -( a )[1],( b )[2] = -( a )[2] )
#define VectorSet( v, x, y, z )       ( ( v )[0] = ( x ), ( v )[1] = ( y ), ( v )[2] = ( z ) )

#define Vector2Set( v, x, y )         ( ( v )[0] = ( x ),( v )[1] = ( y ) )
#define Vector2Copy( a,b )            ( ( b )[0] = ( a )[0],( b )[1] = ( a )[1] )
#define Vector2Subtract( a,b,c )      ( ( c )[0] = ( a )[0] - ( b )[0],( c )[1] = ( a )[1] - ( b )[1] )

#define Vector4Set( v, x, y, z, n )   ( ( v )[0] = ( x ),( v )[1] = ( y ),( v )[2] = ( z ),( v )[3] = ( n ) )
#define Vector4Copy( a,b )            ( ( b )[0] = ( a )[0],( b )[1] = ( a )[1],( b )[2] = ( a )[2],( b )[3] = ( a )[3] )
#define Vector4MA( v, s, b, o )       ( ( o )[0] = ( v )[0] + ( b )[0] * ( s ),( o )[1] = ( v )[1] + ( b )[1] * ( s ),( o )[2] = ( v )[2] + ( b )[2] * ( s ),( o )[3] = ( v )[3] + ( b )[3] * ( s ) )
#define Vector4Average( v, b, s, o )  ( ( o )[0] = ( ( v )[0] * ( 1 - ( s ) ) ) + ( ( b )[0] * ( s ) ),( o )[1] = ( ( v )[1] * ( 1 - ( s ) ) ) + ( ( b )[1] * ( s ) ),( o )[2] = ( ( v )[2] * ( 1 - ( s ) ) ) + ( ( b )[2] * ( s ) ),( o )[3] = ( ( v )[3] * ( 1 - ( s ) ) ) + ( ( b )[3] * ( s ) ) )

#define SnapVector( v ) {v[0] = ( (int)( v[0] ) ); v[1] = ( (int)( v[1] ) ); v[2] = ( (int)( v[2] ) );}

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];

typedef enum { qfalse, qtrue }	qboolean;

typedef int clipHandle_t;

typedef enum {
	ERR_FATAL,                  // exit the entire game with a popup window
	ERR_VID_FATAL,              // exit the entire game with a popup window and doesn't delete profile.pid
	ERR_DROP,                   // print to console and disconnect from game
	ERR_SERVERDISCONNECT,       // don't kill server
	ERR_DISCONNECT,             // client disconnected from the server
	ERR_NEED_CD,                // pop up the need-cd dialog
	ERR_AUTOUPDATE
} errorParm_t;

/*
    CVARS
*/

#define CVAR_ARCHIVE        1   // set to cause it to be saved to vars.rc
								// used for system variables, not for player
								// specific configurations
#define CVAR_USERINFO       2   // sent to server on connect or change
#define CVAR_SERVERINFO     4   // sent in response to front end requests
#define CVAR_SYSTEMINFO     8   // these cvars will be duplicated on all clients
#define CVAR_INIT           16  // don't allow change from console at all,
								// but can be set from the command line
#define CVAR_LATCH          32  // will only change when C code next does
								// a Cvar_Get(), so it can't be changed
								// without proper initialization.  modified
								// will be set, even though the value hasn't
								// changed yet
#define CVAR_ROM            64  // display only, cannot be set by user at all
#define CVAR_USER_CREATED   128 // created by a set command
#define CVAR_TEMP           256 // can be set even when cheats are disabled, but is not archived
#define CVAR_CHEAT          512 // can not be changed if cheats are disabled
#define CVAR_NORESTART      1024    // do not clear when a cvar_restart is issued
#define CVAR_WOLFINFO       2048    // DHM - NERVE :: Like userinfo, but for wolf multiplayer info

#define CVAR_UNSAFE         4096    // ydnar: unsafe system cvars (renderer, sound settings, anything that might cause a crash)
#define CVAR_SERVERINFO_NOUPDATE        8192    // gordon: WONT automatically send this to clients, but server browsers will see it

// nothing outside the Cvar_*() functions should modify these fields!
typedef struct cvar_s {
	char        *name;
	char        *string;
	char        *resetString;       // cvar_restart will reset to this value
	char        *latchedString;     // for CVAR_LATCH vars
	int flags;
	qboolean modified;              // set each time the cvar is changed
	int modificationCount;          // incremented each time the cvar is changed
	float value;                    // atof( string )
	int integer;                    // atoi( string )
	struct cvar_s *next;
	struct cvar_s *hashNext;
} cvar_t;

#define MAX_CVAR_VALUE_STRING   256

typedef int cvarHandle_t;

// the modules that run in the virtual machine can't access the cvar_t directly,
// so they must ask for structured updates
typedef struct {
	cvarHandle_t handle;
	int modificationCount;
	float value;
	int integer;
	char string[MAX_CVAR_VALUE_STRING];
} vmCvar_t;


#define PASSFLOAT( x ) (*(INT*)&x)

#define    MASK_ALL                (-1)
#define    MASK_SOLID                (CONTENTS_SOLID)
#define    MASK_PLAYERSOLID        (CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_BODY)
#define    MASK_DEADSOLID            (CONTENTS_SOLID|CONTENTS_PLAYERCLIP)
#define    MASK_WATER                (CONTENTS_WATER|CONTENTS_LAVA|CONTENTS_SLIME)
#define    MASK_OPAQUE                (CONTENTS_SOLID|CONTENTS_SLIME|CONTENTS_LAVA)
#define    MASK_SHOT                (CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE)

//#define MAX_STRING_CHARS    1024    // max length of a string passed to Cmd_TokenizeString
#define MAX_STRING_CHARS    1024    // max length of a string passed to Cmd_TokenizeString
#define MAX_STRING_TOKENS   256     // max tokens resulting from Cmd_TokenizeString
#define MAX_TOKEN_CHARS     1024    // max length of an individual token

#define MAX_INFO_STRING     1024
#define MAX_INFO_KEY        1024
#define MAX_INFO_VALUE      1024

#define BIG_INFO_STRING     8192    // used for system info key only
#define BIG_INFO_KEY        8192
#define BIG_INFO_VALUE      8192

#define MAX_QPATH           64      // max length of a quake game pathname
#define MAX_OSPATH          256     // max length of a filesystem pathname

// rain - increased to 36 to match MAX_NETNAME, fixes #13 - UI stuff breaks
// with very long names
#define MAX_NAME_LENGTH     36      // max length of a client name

#define MAX_SAY_TEXT        150

#define MAX_BINARY_MESSAGE  32768   // max length of binary message

typedef struct cplane_s {
	vec3_t	normal;
	float	dist;
	byte	type;
	byte	signbits;
	byte	pad[2];
} cplane_t;

typedef struct {
	float		fraction;
	vec3_t		endpos;

	cplane_t	plane;
	int			surfaceFlags;
	int			contents;

	qboolean	allsolid;
	qboolean	startsolid;
	int			entityNum;

} trace_t;

/*
========================================================================

  ELEMENTS COMMUNICATED ACROSS THE NET

========================================================================
*/

#define ANGLE2SHORT(x)  ((int)((x)*65536/360) & 65535)
#define SHORT2ANGLE(x)  ((x)*(360.0/65536))

#define SNAPFLAG_RATE_DELAYED 1
#define SNAPFLAG_NOT_ACTIVE   2 // snapshot used during connection and for zombies
#define SNAPFLAG_SERVERCOUNT  4 // toggled every map_restart so transitions can be detected

//
// per-level limits
//
#define MAX_CLIENTS     64    // absolute limit
#define MAX_LOCATIONS   64

#define GENTITYNUM_BITS   10    // don't need to send any more
#define MAX_GENTITIES   (1<<GENTITYNUM_BITS)

// entitynums are communicated with GENTITY_BITS, so any reserved
// values that are going to be communcated over the net need to
// also be in this range
#define ENTITYNUM_NONE    (MAX_GENTITIES-1)
#define ENTITYNUM_WORLD   (MAX_GENTITIES-2)
#define ENTITYNUM_MAX_NORMAL  (MAX_GENTITIES-2)

typedef int gentity_t;
struct entityState_s;
struct entityState_t;
struct playerState_s;

typedef struct {
	int unknown; //0
	byte* data; //4
	int maxsize; //8 (most likely maxsize value 16384)
	int cursize; //12 //value 129
	int readcount; //16 //value 16
	int bit; //20 //value 0
} msg_t; //size is 0x18 (24)

extern short   BigShort( short l );
extern short   LittleShort( short l );
extern int     BigLong( int l );
extern int     LittleLong( int l );
extern float   BigFloat( float l );
extern float   LittleFloat( float l );

typedef void (*MSG_BeginReading_t)( msg_t *msg );
typedef int     (*MSG_ReadLong_t)( msg_t *sb );
typedef char* (*MSG_ReadStringLine_t)( msg_t *sb );
typedef char * (*MSG_ReadString_t)( msg_t *);

extern MSG_ReadStringLine_t MSG_ReadStringLine;
extern MSG_ReadLong_t MSG_ReadLong;
extern MSG_BeginReading_t MSG_BeginReading;
extern MSG_ReadString_t MSG_ReadString;

typedef enum {
	FS_READ,
	FS_WRITE,
	FS_APPEND,
	FS_APPEND_SYNC
} fsMode_t;

typedef enum {
    ET_GENERAL,
    ET_PLAYER,
    ET_CORPSE,
    ET_ITEM,
    ET_MISSILE,
    ET_MOVER,
    ET_PORTAL,
    ET_INVISIBLE,
    ET_SCRIPTMOVER,
	ET_UNKNOWN,
	ET_FX,
	ET_TURRET
} entityType_t;

typedef enum {
	TR_STATIONARY,
	TR_INTERPOLATE,             // non-parametric, but interpolate between snapshots
	TR_LINEAR,
	TR_LINEAR_STOP,
	TR_LINEAR_STOP_BACK,        //----(SA)	added.  so reverse movement can be different than forward
	TR_SINE,                    // value = base + sin( time / duration ) * delta
	TR_GRAVITY,
	// Ridah
	TR_GRAVITY_LOW,
	TR_GRAVITY_FLOAT,           // super low grav with no gravity acceleration (floating feathers/fabric/leaves/...)
	TR_GRAVITY_PAUSED,          //----(SA)	has stopped, but will still do a short trace to see if it should be switched back to TR_GRAVITY
	TR_ACCELERATE,
	TR_DECCELERATE,
	// Gordon
	TR_SPLINE,
	TR_LINEAR_PATH
} trType_t;

typedef struct {
	trType_t trType;
	int trTime;
	int trDuration;             // if non 0, trTime + trDuration = stop time
//----(SA)	removed
	vec3_t trBase;
	vec3_t trDelta;             // velocity, etc
//----(SA)	removed
} trajectory_t;

/*
typedef struct {
    byte unk[480];
    int maxclients;
    int frameNum;
    int time;
    int previousTime;
    int startTime; //?idk
} level_locals_t;
*/
typedef unsigned char* level_locals_t;

extern level_locals_t level;

//gentity->eFlags
#define EF_DEAD 0x1
#define EF_CROUCH 0x20
#define EF_PRONE 0x40
#define EF_NODRAW 0x100
#define EF_AIM 0x200 //if a player is typing in chat or if their console is open or menu is open
#define EF_FIRING 0x400
#define EF_TALK 0x40000
#define ET_MIX = 0x3000000 //ET_BOUNCE | ET_BOUNCE_HALF
//#define ET_TURRET = 0xC000 //when player is on mg42 e.g
/* Already using ET_TURRET in entity types */
//gentity->flags
#define FL_NODRAW 0x1000

/*
typedef struct gentity_s {
    int number; //0
    entityType_t eType; //4
    int eFlags; //console open //crouch, stand, prone //8
    trajectory_t pos;
    trajectory_t apos;
    int unknown; //80
    int unknown2;
    int unknown3;
    int weaponunknown; //something with weapon
    int weaponunknown2; //something with weapon
    int weaponunknown3; //something with weapon
    float unknownangle; //104
    float unknownangle2;
    float unknownangle3;
    int unknown4; //116
    int unknown5;
    int groundEntityNum;
    int constantLight; //128
    int loopSound;
    int event;
    int eventParm; //140
    int clientNum; //144
    byte unknown148[20]; //164
    int eventSequence;
    int events[4];
    int eventParms[4];
    int weapon;
    int legsAnim;
    int torsoAnim; //208
    int unknown212;
    int unknown216;
    int unknown220;
    int animMovetype;
    int unknown228;
    byte unknown232[8];
    qboolean linked;
} gentity_t;

typedef struct gentity_s gentity_t;
*/

extern char* modNames[];

typedef struct gitem_s {
	char        *classname;
	//rest idc
} gitem_t;

typedef enum {
    TRAP_PRINT,
    TRAP_ERROR,
    TRAP_ERROR_LOCALIZED,
    TRAP_MILLISECONDS,
    TRAP_CVAR_REGISTER,
    TRAP_CVAR_UPDATE,
    TRAP_CVAR_SET,
    TRAP_CVAR_VARIABLE_INTEGER_VALUE,
    TRAP_CVAR_VARIABLE_VALUE,
    TRAP_CVAR_VARIABLE_STRINGBUFFER,
    TRAP_ARGC,
    TRAP_ARGV,
    TRAP_HUNK_ALLOC_INTERNAL,
    TRAP_HUNK_ALLOC_LOW_INTERNAL,
    TRAP_HUNK_ALLOC_ALIGN_INTERNAL,
    TRAP_HUNK_ALLOC_LOW_ALIGN_INTERNAL,
    TRAP_HUNK_ALLOCATE_TEMP_MEMORY_INTERNAL,
    TRAP_HUNK_FREE_TEMP_MEMORY_INTERNAL,
    TRAP_FS_FOPEN_FILE,
    TRAP_FS_READ,
    TRAP_FS_WRITE,
    TRAP_FS_RENAME,
    TRAP_FS_FCLOSE_FILE,
    TRAP_SEND_CONSOLE_COMMAND,
    TRAP_LOCATE_GAME_DATA,
    TRAP_GETGUID,
    TRAP_DROP_CLIENT,
    TRAP_SEND_SERVER_COMMAND,
    TRAP_SET_CONFIGSTRING,
    TRAP_GET_CONFIGSTRING,
    TRAP_GET_CONFIGSTRINTRAP_CONST,
    TRAP_IS_LOCAL_CLIENT,
    TRAP_GET_CLIENT_PING,
    TRAP_GET_USERINFO,
    TRAP_SET_USERINFO,
    TRAP_GET_SERVERINFO,
    TRAP_SET_BRUSHMODEL,
    TRAP_TRACE,
    TRAP_TRACE_CAPSULE,
    TRAP_SIGHT_TRACE,
    TRAP_SIGHT_TRACE_CAPSULE,
    TRAP_SIGHT_TRACE_TO_ENTITY,
    TRAP_CM_BOX_TRACE,
    TRAP_CM_CAPSULE_TRACE,
    TRAP_CM_BOX_SIGHT_TRACE,
    TRAP_CM_CAPSULE_SIGHT_TRACE,
    TRAP_LOCATIONAL_TRACE,
    TRAP_POINT_CONTENTS,
    TRAP_IN_PVS,
    TRAP_IN_PVS_IGNORE_PORTALS,
    TRAP_IN_SNAPSHOT,
    TRAP_ADJUST_AREA_PORTAL_STATE,
    TRAP_AREAS_CONNECTED,
    TRAP_LINK_ENTITY,
    TRAP_UNLINK_ENTITY,
    TRAP_ENTITIES_IN_BOX,
    TRAP_ENTITY_CONTACT,
    TRAP_GET_USERCMD,
    TRAP_GET_ENTITY_TOKEN,
    TRAP_FS_GET_FILELIST,
    TRAP_MAP_EXISTS,
    TRAP_REAL_TIME,
    TRAP_SNAP_VECTOR,
    TRAP_ENTITY_CONTACT_CAPSULE,
    TRAP_COM_SOUNDALIAS_STRING,
    TRAP_COM_PICK_SOUNDALIAS,
    TRAP_COM_SOUNDALIAS_INDEX,
    TRAP_SURFACETYPE_FROM_NAME,
    TRAP_SURFACETYPE_TO_NAME,
    TRAP_ADDTESTCLIENT,
    TRAP_GET_ARCHIVED_CLIENTINFO,
    TRAP_ADDDEBUGSTRING,
    TRAP_ADDDEBUGLINE,
    TRAP_SETARCHIVE,
    TRAP_Z_MALLOC_INTERNAL,
    TRAP_Z_FREE_INTERNAL,
    TRAP_XANIM_CREATE_TREE,
    TRAP_XANIM_CREATE_SMALL_TREE,
    TRAP_XANIM_FREE_SMALL_TREE,
    TRAP_XMODEL_EXISTS,
    TRAP_XMODEL_GET,
    TRAP_DOBJ_CREATE,
    TRAP_DOBJ_EXISTS,
    TRAP_SAFE_DOBJ_FREE,
    TRAP_XANIM_GET_ANIMS,
    //not sure about 85 | 0x55
    //could be TRAP_XANIM_GET_ROOT
    TRAP_XANIM_CLEAR_TREE_GOAL_WEIGHTS = 86,
    TRAP_XANIM_CLEAR_GOAL_WEIGHT,
    TRAP_XANIM_CLEAR_TREE_GOAL_WEIGHTS_STRICT,
    TRAP_XANIM_SET_COMPLETE_GOAL_WEIGHT_KNOB,
    TRAP_XANIM_SET_COMPLETE_GOAL_WEIGHT_KNOB_ALL,
    TRAP_XANIM_SET_ANIM_RATE,
    TRAP_XANIM_SET_TIME,
    TRAP_XANIM_SET_GOAL_WEIGHT_KNOB,
    TRAP_XANIM_CLEAR_TREE,
    TRAP_XANIM_HAS_TIME,
    TRAP_XANIM_IS_PRIMITIVE,
    TRAP_XANIM_GET_LENGTH,
    TRAP_XANIM_GET_LENGTH_SECONDS,
    TRAP_XANIM_SET_COMPLETE_GOAL_WEIGHT,
    TRAP_XANIM_SET_GOAL_WEIGHT,
    TRAP_XANIM_CALC_ABS_DELTA,
    TRAP_XANIM_CALC_DELTA,
    TRAP_XANIM_GET_REL_DELTA,
    TRAP_XANIM_GET_ABS_DELTA,
    TRAP_XANIM_IS_LOOPED,
    TRAP_XANIM_NOTETRACK_EXISTS,
    TRAP_XANIM_GET_TIME,
    TRAP_XANIM_GET_WEIGHT,
    TRAP_DOBJ_DUMP_INFO,
    TRAP_DOBJ_CREATE_SKEL_FOR_BONE,
    TRAP_DOBJ_CREATE_SKEL_FOR_BONES,
    TRAP_DOBJ_UPDATE_SERVERTIME,
    TRAP_DOBJ_INIT_SERVERTIME,
    TRAP_DOBJ_GET_HIERARCHY_BITS,
    TRAP_DOBJ_CALC_ANIM,
    TRAP_DOBJ_CALC_SKEL,
    TRAP_XANIM_LOAD_ANIMTREE,
    TRAP_XANIM_SAVE_ANIMTREE,
    TRAP_XANIM_CLONE_ANIMTREE,
    TRAP_DOBJ_NUM_BONES,
    TRAP_DOBJ_GET_BONE_INDEX,
    TRAP_DOBJ_GET_MATRIX_ARRAY,
    TRAP_DOBJ_DISPLAY_ANIM,
    TRAP_XANIM_HAS_FINISHED,
    TRAP_XANIM_GET_NUM_CHILDREN,
    TRAP_XANIM_GET_CHILD_AT,
    TRAP_XMODEL_NUM_BONES,
    TRAP_XMODEL_GET_BONE_NAMES,
    TRAP_DOBJ_GET_ROT_TRANS_ARRAY,
    TRAP_DOBJ_SET_ROT_TRANS_INDEX,
    TRAP_DOBJ_SET_CONTROL_ROT_TRANS_INDEX,
    TRAP_XANIM_GET_ANIM_NAME,
    TRAP_DOBJ_GET_TREE,
    TRAP_XANIM_GET_ANIMTREE_SIZE,
    TRAP_XMODEL_DEBUTRAP_BOXES,
    TRAP_GET_WEAPONINFO_MEMORY,
    TRAP_FREE_WEAPONINFO_MEMORY,
    TRAP_FREE_CLIENT_SCRIPT_PERS,
    TRAP_RESET_ENTITY_PARSE_POINT,


    TRAP_MEMSET = 200,
    TRAP_MEMCPY,
    TRAP_STRNCPY,
    TRAP_SIN,
    TRAP_COS,
    TRAP_ATAN2,
    TRAP_SQRT,
    TRAP_MATRIXMULTIPLY,
    TRAP_ANGLEVECTORS,
    TRAP_PERPENDICULARVECTOR,
    TRAP_FLOOR,
    TRAP_CEIL
    /*
    from
    typedef enum {
	TRAP_MEMSET = 100,
	TRAP_MEMCPY,
	TRAP_STRNCPY,
	TRAP_SIN,
	TRAP_COS,
	TRAP_ATAN2,
	TRAP_SQRT,
	TRAP_MATRIXMULTIPLY,
	TRAP_ANGLEVECTORS,
	TRAP_PERPENDICULARVECTOR,
	TRAP_FLOOR,
	TRAP_CEIL,

	TRAP_TESTPRINTINT,
	TRAP_TESTPRINTFLOAT
    } sharedTraps_t;
    */
} gameImport_t;


typedef enum {
    GAME_INIT,
	GAME_SHUTDOWN,
	GAME_CLIENT_CONNECT,
	GAME_CLIENT_BEGIN,
	GAME_CLIENT_USERINFO_CHANGED,
	GAME_CLIENT_DISCONNECT,
	GAME_CLIENT_COMMAND,
	GAME_CLIENT_THINK,
	GAME_GET_FOLLOW_PLAYERSTATE,
	GAME_UPDATE_CVARS,
	GAME_RUN_FRAME,
	GAME_CONSOLE_COMMAND
	//dont rlly need rest atm
} gameExport_t;

/*
int __stdcall trap_XAnimGetRoot(int a1, int a2)
{
  int v3; // ST0C_4@1
  int v4; // eax@1

  LOWORD(v3) = 0;
  v4 = trap_XAnimGetAnims(a2);
  HIWORD(v3) = Scr_GetAnimsIndex(v4);
  *(_DWORD *)a1 = v3;
  return a1;
}
*/

typedef void ( *xcommand_t )( void );

typedef void (*Cmd_AddCommand_t)(const char*, xcommand_t);
typedef void (*Com_Error_t)(int code, const char *fmt, ...);

extern Cmd_AddCommand_t Cmd_AddCommand;
extern Com_Error_t Com_Error;

void myClientCommand();
void myClientEndFrame(int*);
void myClientUserinfoChanged( int clientNum );
char* myClientConnect(int, int);
void myClientDisconnect(int);
/*
================
CMD
================
*/

typedef void (*Com_Printf_t)(const char*, ...);
typedef void (*Com_DPrintf_t)(const char*, ...);

extern Com_Printf_t Com_Printf;
extern Com_DPrintf_t Com_DPrintf;

typedef int (*Cmd_Argc_t)();
typedef char* (*Cmd_Argv_t)(unsigned int);
typedef void (*Cmd_ArgvBuffer_t)(int arg, char *buffer, int bufferLength);
typedef void (*Cmd_TokenizeString_t)(const char*);

extern Cmd_Argc_t Cmd_Argc;
extern Cmd_Argv_t Cmd_Argv;
extern Cmd_ArgvBuffer_t Cmd_ArgvBuffer;
extern Cmd_TokenizeString_t Cmd_TokenizeString;

#define MAX_STACK   256
#define STACK_MASK  ( MAX_STACK - 1 )

typedef void vm_t;

typedef int (QDECL *VM_Call_t)( int, int callnum, ... );
extern VM_Call_t VM_Call;

extern vm_t *gvm;

/*
================
CVAR
================
*/

typedef cvar_t* (*Cvar_FindVar_t)(const char*);
typedef void (*Cvar_Set_t)(const char*, const char*);
typedef cvar_t* (*Cvar_Get_t)( const char *var_name, const char *var_value, int flags );

extern Cvar_Set_t Cvar_Set;
extern Cvar_Get_t Cvar_Get;

char* Cvar_VariableString(const char*);
char* Cvar_InfoString(int bit);




/*
==============
COMMON
==============
*/

int Q_isprint( int c );
int Q_islower( int c );
int Q_isupper( int c );
int Q_isalpha( int c );
int Q_isnumeric( int c );
int Q_isalphanumeric( int c );
int Q_isforfilename( int c );

const char *Q_stristr( const char *s, const char *find);

// portable case insensitive compare
int     Q_stricmp( const char *s1, const char *s2 );
int     Q_strncmp( const char *s1, const char *s2, int n );
int     Q_stricmpn( const char *s1, const char *s2, int n );
char    *Q_strlwr( char *s1 );
char    *Q_strupr( char *s1 );
char    *Q_strrchr( const char* string, int c );

// buffer size safe library replacements
void    Q_strncpyz( char *dest, const char *src, int destsize );
void    Q_strcat( char *dest, int size, const char *src );

void Info_Print( const char *s );
char *Q_CleanStr( char *string );

void QDECL Com_sprintf( char *dest, int size, const char *fmt, ... );

char    * QDECL va( char *format, ... );
char *Info_ValueForKey( const char *s, const char *key );
void Info_SetValueForKey( char *s, const char *key, const char *value );

extern void* gamelib;
extern int base;
extern char (*zpml)[140];
extern void *zpm;


bool is_good_string(char* str);

#define g_client_size 8900

extern unsigned char* g_clients;

#ifdef uMYSQL
extern MYSQL *db;
#endif

bool Scr_Continue();

void SV_AddOperatorCommands(void);
void COD_Destructor();

/*
static int GAME(const char* n) {
	//printf("GAME(%s);\n", n);
	if(gamelib!=NULL)
	return (int)dlsym(gamelib, n);
	return 0;
}*/
#define GAME(n) ((gamelib!=NULL)?(int)dlsym(gamelib,n):0)

static char *utrim(char *str) //malloc unsafe trim
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

#endif // SHARED_H
