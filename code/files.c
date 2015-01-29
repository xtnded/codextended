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

static searchpath_t    *fs_searchpaths = (searchpath_t*)0x80DD590;

FS_FileIsInPAK_t FS_FileIsInPAK = (FS_FileIsInPAK_t)0x8070240;

int pak_num = 0;

int FS_IsPakFile(char *name) {
	if(strstr(name, "pak") != NULL)
		return 1;
	if(strstr(name, "localized") != NULL)
		return 1;
	return 0;
}

bool FS_IsServerFile(char* basename) {
	if(strstr(basename, "srv") != NULL)
		return 1;
	if(strstr(basename, "svr") != NULL)
		return 1;
	if(strstr(basename, "server") != NULL)
		return 1;
	return 0;
}

bool FS_IsExtendedFile(char* file) {
	return 1;
	#if 0
	if(strstr(file, "xtnded") != NULL)
		return 1;
	if(strstr(file, "codextended") != NULL)
		return 1;
	if(!FS_IsServerFile(file))
		return 1;
	return 0;
	#endif
}

static bool count_flag = 1;

const char *FS_LoadedPakChecksums() {
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
			if ( *info ) {
				sprintf(info, "%s%s", info, " " );
			}
			//if(search->pak->referenced || !strcmp(fs_game, search->pak->pakGamename)) {
			if(FS_IsPakFile(search->pak->pakBasename) || search->pak->referenced || FS_IsExtendedFile(search->pak->pakBasename)) {
				sprintf(info, "%s%i", info, search->pak->checksum);
			}
		}
	}
	return info;
}

const char *FS_LoadedPakPureChecksums( void ) {
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
			if ( *info ) {
				sprintf(info, "%s%s", info, " " );
			}
			//if(search->pak->referenced || !strcmp(fs_game, search->pak->pakGamename)) {
			if(FS_IsPakFile(search->pak->pakBasename) || search->pak->referenced || FS_IsExtendedFile(search->pak->pakBasename)) {
				sprintf(info, "%s%i", info, search->pak->checksum_pure);
				if(count_flag)
					pak_num++;
			}
		}
	}
	if(count_flag)
		count_flag=0;
	return info;
}

const char *FS_ReferencedPakChecksums() {
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
			if ( *info ) {
				sprintf(info, "%s%s", info, " " );
			}
			//if(search->pak->referenced || !strcmp(fs_game, search->pak->pakGamename)) {
			if(FS_IsPakFile(search->pak->pakBasename) || search->pak->referenced || FS_IsExtendedFile(search->pak->pakBasename)) {
				sprintf(info, "%s%i", info, search->pak->checksum);
			}
		}
	}
	
	return info;
}

const char *FS_ReferencedPakNames() {
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
			if ( *info ) {
				sprintf(info, "%s%s", info, " " );
			}
			//if(search->pak->referenced || !strcmp(fs_game, search->pak->pakGamename)) {
			if(FS_IsPakFile(search->pak->pakBasename) || search->pak->referenced || FS_IsExtendedFile(search->pak->pakBasename)) {
				//printf("PAK: %s with checksum %d and PURE = %d GAMENAME = %s\n", search->pak->pakBasename, search->pak->checksum, search->pak->checksum_pure, search->pak->pakGamename);
				sprintf(info, "%s%s", info, search->pak->pakGamename );
				sprintf(info, "%s%s", info, "/" );
				sprintf(info, "%s%s", info, search->pak->pakBasename );
			}
		}
	}
	
	return info;
}

const char *FS_UpdateName() {
	return CL_UPDATE_PAK_BASENAME;
}

const char *FS_UpdateChecksum() {
	searchpath_t *search;
	for(search = fs_searchpaths->next; search; search = search->next) {
		if(search->pak) {
			//if(search->pak->referenced || !strcmp(fs_game, search->pak->pakGamename)) {
			if(!strcmp(search->pak->pakBasename, CL_UPDATE_PAK_BASENAME))
				return va("%d", search->pak->checksum);
		}
	}
	return "0";
}

const char *FS_ReferencedUpdateName() {
	char *fs_game = Cvar_VariableString("fs_game");
	if(!*fs_game)
		fs_game = "main";
	return va("%s/%s", fs_game, CL_UPDATE_PAK_BASENAME);
}

const char *FS_LoadedPakNames() {
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
			if ( *info ) {
				sprintf(info, "%s%s", info, " " );
			}
			//if(search->pak->referenced || !strcmp(fs_game, search->pak->pakGamename)) {
			if(FS_IsPakFile(search->pak->pakBasename) || search->pak->referenced || FS_IsExtendedFile(search->pak->pakBasename)) {
				sprintf(info, "%s%s", info, search->pak->pakBasename );
			}
		}
	}
	
	return info;
}
