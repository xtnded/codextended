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

#include "include.hpp"

int game_mp;
level_locals_t level;

char* modNames[] = {
    "MOD_UNKNOWN",
    "MOD_PISTOL_BULLET",
    "MOD_RIFLE_BULLET",
    "MOD_GRENADE",
    "MOD_GRENADE_SPLASH",
    "MOD_PROJECTILE",
    "MOD_PROJECTILE_SPLASH",
    "MOD_MELEE",
    "MOD_HEAD_SHOT",
    "MOD_MORTAR",
    "MOD_MORTAR_SPLASH",
    "MOD_KICKED",
    "MOD_GRABBER",
    "MOD_DYNAMITE",
    "MOD_DYNAMITE_SPLASH",
    "MOD_AIRSTRIKE",
    "MOD_WATER",
    "MOD_SLIME",
    "MOD_LAVA",
    "MOD_CRUSH",
    "MOD_TELEFRAG",
    "MOD_FALLING",
    "MOD_SUICIDE",
    "MOD_TRIGGER_HURT",
    "MOD_EXPLOSIVE"
};

void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory) {
    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
        return;

    do {
        const std::string file_name = file_data.cFileName;
        const std::string full_file_name = directory + "/" + file_name;
        const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (file_name[0] == '.')
            continue;

        if (is_directory)
            continue;

        out.push_back(full_file_name);
    } while (FindNextFile(dir, &file_data));

    FindClose(dir);
}

void Q_strncpyz( char *dest, const char *src, int destsize ) {
	if ( !src ) {
		Com_Error( ERR_FATAL, "Q_strncpyz: NULL src" );
	}
	if ( destsize < 1 ) {
		Com_Error( ERR_FATAL,"Q_strncpyz: destsize < 1" );
	}

	strncpy( dest, src, destsize - 1 );
	dest[destsize - 1] = 0;
}

int Q_stricmpn (const char *s1, const char *s2, int n) {
	int		c1, c2;

	// bk001129 - moved in 1.17 fix not in id codebase
        if ( s1 == NULL ) {
           if ( s2 == NULL )
             return 0;
           else
             return -1;
        }
        else if ( s2==NULL )
          return 1;



	do {
		c1 = *s1++;
		c2 = *s2++;

		if (!n--) {
			return 0;		// strings are equal until end point
		}

		if (c1 != c2) {
			if (c1 >= 'a' && c1 <= 'z') {
				c1 -= ('a' - 'A');
			}
			if (c2 >= 'a' && c2 <= 'z') {
				c2 -= ('a' - 'A');
			}
			if (c1 != c2) {
				return c1 < c2 ? -1 : 1;
			}
		}
	} while (c1);

	return 0;		// strings are equal
}

/*
    Why not find offsets to Quake's memcpy/memset or use G_MEMSET/G_MEMCPY
    IDK, cba to //richard
*/

void _memset(void* addr, int v, size_t len) {
    DWORD tmp;
    VirtualProtect((void*)addr, len, PAGE_EXECUTE_READWRITE, &tmp);
    memset(addr, v, len);
    VirtualProtect((void*)addr, len, tmp, &tmp);
}

void _memcpy(void* v1, void* v2, size_t len) {
    DWORD tmp;
    VirtualProtect((void*)v1, len, PAGE_EXECUTE_READWRITE, &tmp);
    memcpy(v1, v2, len);
    VirtualProtect((void*)v1, len, tmp, &tmp);
}

std::string getFileInPath(std::string s) {
    size_t found = 0;
    for(size_t i = s.size(); found == 0; i--) {
        if(s[i] == '\\' || s[i] == '/')
            found = i+1;
    }
    return s.substr(found, s.size());
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if(!item.empty())
            elems.push_back(item);
    }
    return elems;
}

char    * QDECL va( char *format, ... ) {
	va_list argptr;
	#define MAX_VA_STRING   32000
	static char temp_buffer[MAX_VA_STRING];
	static char string[MAX_VA_STRING];      // in case va is called by nested functions
	static int index = 0;
	char    *buf;
	int len;


	va_start( argptr, format );
	vsprintf( temp_buffer, format,argptr );
	va_end( argptr );

	if ( ( len = strlen( temp_buffer ) ) >= MAX_VA_STRING ) {
		Com_Error( ERR_DROP, "Attempted to overrun string in call to va()\n" );
	}

	if ( len + index >= MAX_VA_STRING - 1 ) {
		index = 0;
	}

	buf = &string[index];
	memcpy( buf, temp_buffer, len + 1 );

	index += len + 1;

	return buf;
}

const char *Q_stristr( const char *s, const char *find)
{
  register char c, sc;
  register size_t len;

  if ((c = *find++) != 0)
  {
    if (c >= 'a' && c <= 'z')
    {
      c -= ('a' - 'A');
    }
    len = strlen(find);
    do
    {
      do
      {
        if ((sc = *s++) == 0)
          return NULL;
        if (sc >= 'a' && sc <= 'z')
        {
          sc -= ('a' - 'A');
        }
      } while (sc != c);
    } while (Q_stricmpn(s, find, len) != 0);
    s--;
  }
  return s;
}

char **cmd_argv = (char **)0x08D2624;

Com_Error_t Com_Error = (Com_Error_t)0x437F70;
Com_Printf_t Com_Printf = (Com_Printf_t)0x437C00;
Cmd_AddCommand_t Cmd_AddCommand = (Cmd_AddCommand_t)0x42A870;
Cmd_RemoveCommand_t Cmd_RemoveCommand = (Cmd_RemoveCommand_t)0x42A9C0;
ClientCommand_t ClientCommand = (ClientCommand_t)0x20021EC0;

int Cmd_Argc() {
    return *(int*)0x8D5020;
}

char *Cmd_Argv(unsigned int index) {
    if(index > Cmd_Argc())
        return "";
    return cmd_argv[index];
}

/*
    Syscalls
*/

void trap_DropClient(int a1, const char* reason) {
    syscall(TRAP_DROP_CLIENT, a1, reason);
}

void trap_SendServerCommand( int clientNum, int UNKNOWN, const char *text ) {
    syscall(TRAP_SEND_SERVER_COMMAND, clientNum, UNKNOWN, text);
}

int trap_LinkEntity(int *a1) {
    return syscall(TRAP_LINK_ENTITY, a1);
}

int trap_UnlinkEntity(int *a1) {
    return syscall(TRAP_UNLINK_ENTITY, a1);
}

int trap_getClientPing(int a1) {
   return syscall(TRAP_GET_CLIENT_PING, a1);
}

int trap_FS_FOpenFile(const char *qpath, int *f, fsMode_t mode) {
	return syscall(TRAP_FS_FOPEN_FILE, qpath, f, mode);
}

void trap_FS_Read(void *buffer, int len, int f) {
	syscall(TRAP_FS_READ, buffer, len, f);
}

int trap_FS_Write(const void *buffer, int len, int f) {
	return syscall(TRAP_FS_WRITE, buffer, len, f);
}

int trap_FS_Rename(const char *from, const char *to) {
	return syscall(TRAP_FS_RENAME, from, to);
}

void trap_FS_FCloseFile(int f ) {
	syscall(TRAP_FS_FCLOSE_FILE, f );
}

int trap_Argc() {
    return syscall(TRAP_ARGC);
}

void trap_Argv( int n, char *buffer, int bufferLength ) {
	syscall(TRAP_ARGV, n, buffer, bufferLength);
}

void trap_CM_BoxTrace(trace_t * results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, int model, int brushmask)
{
     syscall(TRAP_CM_BOX_TRACE, results, start, end, mins, maxs, model, brushmask );
}

void trap_Trace( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask ) {
	syscall(TRAP_TRACE, results, start, mins, maxs, end, passEntityNum, contentmask );
}

int trap_AddDebugString(vec3_t a, vec3_t b, int c, char* d) {
    int v5;
    v5 = PASSFLOAT(c);
    return syscall(TRAP_ADDDEBUGSTRING, a, b, v5, d);
}

void trap_setBrushModel(int a1) {
    syscall(TRAP_SET_BRUSHMODEL, a1);
}

