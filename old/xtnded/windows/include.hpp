/*
===========================================================================
    Copyright (C) riicchhaarrd
    Copyright (C) 1999-2005 Id Software, Inc.

    This file is part of CoDExtended source code.

    CoDExtended source code is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    CoDExtended source code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
===========================================================================
*/

#ifndef INCLUDE_H
#define INCLUDE_H

#include "Windows.h"
#include <string>
#include <cctype>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "cracking.hpp"

//#define uMYSQL 1
#ifdef uMYSQL
#include <mysql.h>
#endif


#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

#define __EXTENDEDVERSION__ 1
extern int CODPATCH;

void GetFilesInDirectory(std::vector<std::string>&, const std::string&);
int Q_stricmpn(const char*, const char*, int);
std::string getFileInPath(std::string);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
const char* Q_stristr(const char*, const char*);

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];

typedef enum { qfalse, qtrue }	qboolean;

#define PASSFLOAT( x ) (*(INT*)&x)

#define CONTENTS_SOLID          1
#define CONTENTS_LAVA           8
#define CONTENTS_SLIME          16
#define CONTENTS_WATER          32
#define CONTENTS_FOG            64
#define CONTENTS_NOTTEAM1       0x0080
#define CONTENTS_NOTTEAM2       0x0100
#define CONTENTS_NOBOTCLIP      0x0200
#define CONTENTS_AREAPORTAL     0x8000
#define CONTENTS_PLAYERCLIP     0x10000
#define CONTENTS_MONSTERCLIP    0x20000
#define CONTENTS_TELEPORTER     0x40000
#define CONTENTS_JUMPPAD        0x80000
#define CONTENTS_CLUSTERPORTAL  0x100000
#define CONTENTS_DONOTENTER     0x200000
#define CONTENTS_BOTCLIP        0x400000
#define CONTENTS_MOVER          0x800000
#define CONTENTS_ORIGIN         0x1000000
#define CONTENTS_BODY           0x2000000
#define CONTENTS_CORPSE         0x4000000
#define CONTENTS_DETAIL         0x8000000
#define CONTENTS_STRUCTURAL     0x10000000
#define CONTENTS_TRANSLUCENT    0x20000000
#define CONTENTS_TRIGGER        0x40000000
#define CONTENTS_NODROP         0x80000000

#define    MASK_ALL                (-1)
#define    MASK_SOLID                (CONTENTS_SOLID)
#define    MASK_PLAYERSOLID        (CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_BODY)
#define    MASK_DEADSOLID            (CONTENTS_SOLID|CONTENTS_PLAYERCLIP)
#define    MASK_WATER                (CONTENTS_WATER|CONTENTS_LAVA|CONTENTS_SLIME)
#define    MASK_OPAQUE                (CONTENTS_SOLID|CONTENTS_SLIME|CONTENTS_LAVA)
#define    MASK_SHOT                (CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE)

#define MAX_STRING_CHARS    1024    // max length of a string passed to Cmd_TokenizeString
#define MAX_STRING_TOKENS   256     // max tokens resulting from Cmd_TokenizeString
#define MAX_TOKEN_CHARS     1024    // max length of an individual token

#define MAX_INFO_STRING     1024
#define MAX_INFO_KEY        1024
#define MAX_INFO_VALUE      1024

#define BIG_INFO_STRING     8192    // used for system info key only
#define BIG_INFO_KEY        8192
#define BIG_INFO_VALUE      8192


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

typedef struct {
	cvarHandle_t handle;
	int modificationCount;
	float value;
	int integer;
	char string[MAX_CVAR_VALUE_STRING];
} vmCvar_t;

typedef struct cplane_s {
	vec3_t	normal;
	float	dist;
	BYTE	type;
	BYTE	signbits;
	BYTE	pad[2];
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

typedef struct {
	boolean allowoverflow;     // if false, do a Com_Error
	boolean overflowed;        // set to true if the buffer size failed (with allowoverflow set)
	boolean oob;               // set to true if the buffer size failed (with allowoverflow set)
	byte    *data;
	int maxsize;
	int cursize;
	int uncompsize;             // NERVE - SMF - net debugging
	int readcount;
	int bit;                    // for bitwise reads and writes
} msg_t;


typedef enum {
	FS_READ,
	FS_WRITE,
	FS_APPEND,
	FS_APPEND_SYNC
} fsMode_t;

typedef void ( *xcommand_t )( void );

#define QDECL __cdecl

static int ( QDECL * syscall )( int arg, ... ) = ( int ( QDECL * )( int, ... ) ) - 1;

extern char **cmd_argv;

template <typename T, typename ... Ts> T call(size_t addr, Ts ... ts);

template <typename ... Ts> void Com_Printf(const char* fmt, Ts ... ts);

template <typename T, typename ... Ts>
T call(size_t addr, Ts ... ts) {
    T (*f)(...);
    *(T*)&f = (T)addr;
    return f(ts...);
}

template <typename ... Ts>
void Com_Printf(const char* fmt, Ts ... ts) {
    //char out[256];
    //sprintf(out, "Test: %f", CODPATCH);
    //MessageBox(NULL, out, "MDLL", MB_OK);
    if(CODPATCH == 5)
        call<void*, const char*, Ts...>(0x437C00, fmt, ts...);
    else if(CODPATCH == 1)
        call<void*, const char*, Ts...>(0x4357B0, fmt, ts...);
}

void Cmd_AddCommand(const char* name, xcommand_t cmd);
void Cmd_RemoveCommand(const char* name);
void ClientCommand();
static cvar_t* Cvar_FindVar(const char* n);
char* Cvar_VariableString(const char*);
void Cvar_Set(const char *var_name, const char *value);
void trap_DropClient(int a1, const char* reason);
int trap_LinkEntity(int *a1);
int trap_UnlinkEntity(int *a1);
int trap_getClientPing(int a1);
int trap_FS_FOpenFile(const char *qpath, int *f, fsMode_t mode);
void trap_FS_Read(void *buffer, int len, int f);
int trap_FS_Write(const void *buffer, int len, int f);
int trap_FS_Rename(const char *from, const char *to);
void trap_FS_FCloseFile(int f );
int trap_Argc();
void trap_Argv( int n, char *buffer, int bufferLength );
void trap_CM_BoxTrace(trace_t * results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, int model, int brushmask);
void trap_Trace( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask );
int trap_AddDebugString(vec3_t a, vec3_t b, int c, char* d);
void trap_setBrushModel(int a1);
int Cmd_Argc();
char *Cmd_Argv(unsigned int index);

#endif // INCLUDE_H
