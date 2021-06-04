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

#include "shared.h"

typedef struct {
    char pakFilename[256];               // c:\program files\call of duty\main\pak6.pk3
    char pakBasename[256];               // pak6
    char pakGamename[256];               // main
    void* handle;
    int checksum;
    int checksum_pure;
    int numfiles;
    int referenced;
    int hashSize;
    //rest idc
} pack_t;

typedef struct searchpath_s {
    struct searchpath_s *next;
    pack_t      *pak;
    void* dir; //cba to check and don't need it
} searchpath_t;

static searchpath_t *fs_searchpaths = (searchpath_t*)0x80DD590;

const char *__cdecl FS_ReferencedPakChecksums() {
    static char info[8192];
    info[0] = 0;
    searchpath_t *search;
    char fs_game[256];
    char* check = Cvar_VariableString("fs_game");

    if(check[0] == '\0')
        sprintf(fs_game, "main");
    else
        sprintf(fs_game, check);
    
    for(search = fs_searchpaths->next; search; search = search->next) {
        if(search->pak) {
            if(FS_IsServerFile(search->pak->pakBasename))
                continue;

            if(*info)
                sprintf(info, "%s%s", info, " " );

            if(FS_IsPakFile(search->pak->pakBasename) || strcasecmp(search->pak->pakGamename, "main") || *(int*)(search->pak + 784)) {
                sprintf(info, "%s%i", info, search->pak->checksum);
            }
        }
    }

    return info;
}

const char *__cdecl FS_ReferencedPakNames() {
    static char info[8192];
    info[0] = 0;
    searchpath_t *search;
    char fs_game[256];
    char* check = Cvar_VariableString("fs_game");

    if(check[0] == '\0')
        sprintf(fs_game, "main");
    else
        sprintf(fs_game, check);
    
    for(search = fs_searchpaths->next; search; search = search->next) {
        if(search->pak) {
            if(FS_IsServerFile(search->pak->pakBasename))
                continue;

            if(*info)
                sprintf(info, "%s%s", info, " " );

            if(FS_IsPakFile(search->pak->pakBasename) || strcasecmp(search->pak->pakGamename, "main") || *(int*)(search->pak + 784)) {
                sprintf(info, "%s%s", info, search->pak->pakGamename );
                sprintf(info, "%s%s", info, "/" );
                sprintf(info, "%s%s", info, search->pak->pakBasename );
            }
        }
    }

    return info;
}