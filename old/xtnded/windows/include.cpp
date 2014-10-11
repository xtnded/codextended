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

int CODPATCH;

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

extern char **cmd_argv = (char **)0x08D2624;

void Cmd_AddCommand(const char* name, xcommand_t cmd) {
    if(CODPATCH == 5)
        call<void*, const char*, xcommand_t>(0x42A870, name, cmd);
    else
        call<void*, const char*, xcommand_t>(0x428840, name, cmd);
}

void Cmd_RemoveCommand(const char* name) {
    if(CODPATCH == 5)
        call<void*, const char*>(0x42A9C0, name);
    else
        call<void*, const char*>(0x428990, name);
}

void ClientCommand() { //uhhhh yeah
    if(CODPATCH == 5)
        call<void*>(0x20021EC0);
    else
        call<void*>(0x2001FB60);
}

static cvar_t* Cvar_FindVar(const char* n) {
    return call<cvar_t*, const char*>(0x439280, n);
}

char* Cvar_VariableString(const char* n) {
    cvar_t* var;
    var = Cvar_FindVar(n);
    if(!var) {
        return "";
    }
    return var->string;
}

void Cvar_Set(const char *var_name, const char *value) {
	call<void*, const char*, const char*>(0x439650, var_name, value);
}

/*template <typename ... Ts>
static int __cdecl syscall(int arg, Ts ... ts) {
    int (*placeholder)(int, ...);
    if(CODPATCH == 5)
        *(int*)&placeholder = 0x4654C0;
    else if(CODPATCH == 1)
        *(int*)&placeholder = 0x460230;
    else
        *(int*)&placeholder = NULL;
    return placeholder(arg, ts...);
}*/

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

int trap_FS_FOpenFile(const char *qpath, int *f, fsMode_t mode) {
	return syscall(18, qpath, f, mode);
}

void trap_FS_Read(void *buffer, int len, int f) {
	syscall(19, buffer, len, f);
}

int trap_FS_Write(const void *buffer, int len, int f) {
	return syscall(20, buffer, len, f);
}

int trap_FS_Rename(const char *from, const char *to) {
	return syscall(21, from, to);
}

void trap_FS_FCloseFile(int f ) {
	syscall(22, f );
}

int trap_Argc() {
    return syscall(10);
}

void trap_Argv( int n, char *buffer, int bufferLength ) {
	syscall(11, n, buffer, bufferLength);
}

void trap_CM_BoxTrace(trace_t * results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, int model, int brushmask)
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

int Cmd_Argc() {
	if(CODPATCH == 1)
        return *(int*)0x8930F0;
    return *(int*)0x8D5020;
}

char *Cmd_Argv(unsigned int index) {
    if(index > Cmd_Argc())
        return "";
    return cmd_argv[index];
}
