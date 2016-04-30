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

/*
	Preprocessor off project WIP
	This only uses the cpp from gcc
		-Richard
*/

char *preprocess(char *source) {
	size_t size = strlen(source) + 1;
	char *n = NULL;
	
	char infn[64];
	sprintf(infn, "%d.in", rand());
	char outfn[64];
	sprintf(outfn, "%d.out", rand());
	
	FILE *in = fopen(infn, "w");
	if(!in)
		return n;
	fprintf(in, "%s", source);
	fclose(in);
	
	system( va("cpp -P %s %s", infn, outfn) );
	
	remove(infn);
	
	FILE *out = fopen(outfn, "rb");
	if(!out)
		return n;
	fseek(out, 0, SEEK_END);
	size_t fs = ftell(out);
	rewind(out);
	n = malloc(fs);
	if(n == NULL)
		goto _ret;
	fread(n, 1, fs, out);
_ret:
	remove(outfn);
	return n;
}

char *Scr_AddSourceBuffer(char *filename, char* a2, int a3) {
	char* (*o)(char*,int,int);
	*(int*)&o=0x809B5D4;
	
	char *r = o(filename, a2, a3), *replace = NULL;
	
	if(strstr(filename, "elevator.gsc") != NULL) {
		replace = preprocess(r);
		if(replace == NULL) {
			return r;
		}
		size_t size = strlen(replace) + 1;
		char *n = Hunk_AllocateTempMemoryHighInternal( size );
		if(!n) {
			cprintf(PRINT_UNDERLINE | PRINT_ERR, "Failed to allocate memory! Increase com_hunkmegs.\n");
			goto def_ret;
		}
		strcpy(n, replace);
		
		free(replace);
		return n;
	}
def_ret:	
	if(replace != NULL)
		free(replace);
	return r;
}