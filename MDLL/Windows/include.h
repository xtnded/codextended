#include "Windows.h"
#include <string>
#include <cctype>

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

typedef int clipHandle_t;
typedef int		fileHandle_t;

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

typedef int (*Com_Printf_t)(const char*, ...);
Com_Printf_t Com_Printf = (Com_Printf_t)0x0437C00;

typedef int (*G_LogPrintf_t)(const char*, ...);
G_LogPrintf_t G_LogPrintf = (G_LogPrintf_t)0x20028E50;

typedef void (*Cmd_AddCommand_t)(const char*, xcommand_t);
Cmd_AddCommand_t Cmd_AddCommand = (Cmd_AddCommand_t)0x042A870;

typedef void (*ClientCommand_t)();
ClientCommand_t ClientCommand = (ClientCommand_t)0x20021EC0;

/*
    :: Syscalls ::
*/

static int ( __cdecl * syscall )( int arg, ... ) = ( int ( __cdecl * )( int, ... ) )0x04654C0;

int     trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode ) {
	return syscall(18, qpath, f, mode );
}

void trap_DropClient(int a1, const char* reason) {
    syscall(26, a1, reason);
}

int trap_LinkEntity(int *a1) {
    return syscall(53, a1);
}

int trap_UnlinkEntity(int *a1) {
    return syscall(54, a1);
}

int trap_getClientPing(int a1) {
   return syscall(32, a1);
}

void    trap_FS_Read( void *buffer, int len, fileHandle_t f ) {
	syscall(19, buffer, len, f );
}

int     trap_FS_Write( const void *buffer, int len, fileHandle_t f ) {
	return syscall(20, buffer, len, f );
}

int     trap_FS_Rename( const char *from, const char *to ) {
	return syscall(21, from, to );
}

void    trap_FS_FCloseFile( fileHandle_t f ) {
	syscall(22, f );
}

int trap_Argc() {
    return syscall(10);
}

void trap_Argv( int n, char *buffer, int bufferLength ) {
	syscall(11, n, buffer, bufferLength);
}

void trap_CM_BoxTrace(trace_t * results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask)
{
     syscall(38, results, start, end, mins, maxs, model, brushmask );
}

void trap_Trace( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask ) {
	syscall(37, results, start, mins, maxs, end, passEntityNum, contentmask );
}

int trap_AddDebugString(vec3_t a, vec3_t b, int c, char* d) {
    int v5;
    v5 = PASSFLOAT(c);
    return syscall(71, a, b, v5, d);
}

void trap_setBrushModel(int a1) {
    syscall(36, a1);
}

/*
    NOTE: Don't use cmd_* use trap_Argv/Argc instead //php
*/

/*
static int cmd_argc;
static char *cmd_argv[MAX_STRING_TOKENS];
static char cmd_tokenized[BIG_INFO_STRING + MAX_STRING_TOKENS];         // will have 0 bytes inserted
static char cmd_cmd[BIG_INFO_STRING];         // the original command we received (no token processing)

int Cmd_Argc() {
    return *((int*)0x8D5020);
}

char    *Cmd_Argv( int arg ) {
	if ( (unsigned)arg >= cmd_argc ) {
		return "";
	}
	return cmd_argv[arg];
}

char *Cmd_Cmd() {
    memcpy(&cmd_cmd, (int*)0x0931991, MAX_STRING_TOKENS);
	return cmd_cmd;
}

void Q_strncpyz( char *dest, const char *src, int destsize ) {
	strncpy( dest, src, destsize - 1 );
	dest[destsize - 1] = 0;
}

void Cmd_TokenizeString( const char *text_in ) {
	const char  *text;
	char    *textOut;

	// clear previous args
	cmd_argc = 0;

	if ( !text_in ) {
		return;
	}

	Q_strncpyz( cmd_cmd, text_in, sizeof( cmd_cmd ) );

	text = text_in;
	textOut = cmd_tokenized;

	while ( 1 ) {
		if ( cmd_argc == MAX_STRING_TOKENS ) {
			return;         // this is usually something malicious
		}

		while ( 1 ) {
			// skip whitespace
			while ( *text && *text <= ' ' ) {
				text++;
			}
			if ( !*text ) {
				return;         // all tokens parsed
			}

			// skip // comments
			if ( text[0] == '/' && text[1] == '/' ) {
				//bani - lets us put 'http://' in commandlines
				if ( text == text_in || ( text > text_in && text[-1] != ':' ) ) {
					return;         // all tokens parsed
				}
			}

			if ( text[0] == '/' && text[1] == '*' ) {
				while ( *text && ( text[0] != '*' || text[1] != '/' ) ) {
					text++;
				}
				if ( !*text ) {
					return;     // all tokens parsed
				}
				text += 2;
			} else {
				break;          // we are ready to parse a token
			}
		}

		// handle quoted strings
		if ( *text == '"' ) {
			cmd_argv[cmd_argc] = textOut;
			cmd_argc++;
			text++;
			while ( *text && *text != '"' ) {
				*textOut++ = *text++;
			}
			*textOut++ = 0;
			if ( !*text ) {
				return;     // all tokens parsed
			}
			text++;
			continue;
		}

		// regular token
		cmd_argv[cmd_argc] = textOut;
		cmd_argc++;

		// skip until whitespace, quote, or command
		while ( *text > ' ' ) {
			if ( text[0] == '"' ) {
				break;
			}

			if ( text[0] == '/' && text[1] == '/' ) {
				//bani - lets us put 'http://' in commandlines
				if ( text == text_in || ( text > text_in && text[-1] != ':' ) ) {
					break;
				}
			}

			if ( text[0] == '/' && text[1] == '*' ) {
				break;
			}

			*textOut++ = *text++;
		}

		*textOut++ = 0;

		if ( !*text ) {
			return;     // all tokens parsed
		}
	}

}
*/
