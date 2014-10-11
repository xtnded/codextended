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
#ifndef X_UTIL_H
#define X_UTIL_H

/*
	riicchhaarrd
*/

/*
typedef struct mpatch_s {
	int offset;
	byte* bytes;
	byte* patched_bytes;
	size_t size;
} mpatch;

static mpatch* create_patch(int off, byte* bytes, size_t len) {
	mpatch* p = (mpatch*)malloc(sizeof(mpatch));
	if(0 != p) {
		p->size = len;
		p->offset = off;
		p->bytes = (byte*)off;
		p->patched_bytes = (byte*)malloc(sizeof(byte) * len); //just use len???
		if(!p->patched_bytes) {
			free(p);
			return 0;
		}
		memcpy(p->patched_bytes, bytes, len);
	}
	return p;
}

static void delete_patch(mpatch* p) {
	free(p->patched_bytes);
	free(p);
}

static void p_undo(mpatch* p) {
	memcpy((void*)p->offset, p->bytes, p->size);
}

static void p_patch(mpatch* p) {
	memcpy((void*)p->offset, p->patched_bytes, p->size);
}
*/

typedef unsigned char byte;

static int search_memory(int current, int end, byte* bytes, size_t len) {
	while((memcmp((void*)current, bytes, len)) && (current < end))
		current++;
	return (current == end) ? -1 : current;
}

static int patch_memory(int start, int end, byte* search_for, byte* patched, size_t len) {
	int s = search_memory(start, end, search_for, len);
	if(-1 != s)
		memcpy((void*)s, patched, len);
	return s;
}

/*
	Hooking
*/

static void __nop(unsigned int start, unsigned int end) {
	int len = ( end < start ) ? end : ( end - start), i;	
	mprotect((void *)start, len, PROT_READ | PROT_WRITE | PROT_EXEC);
	memset((void*)start, 0x90, len);
}

static void __jmp(unsigned int off, unsigned int loc) {
	mprotect((void*)off, 5, PROT_READ | PROT_WRITE | PROT_EXEC);
	*(unsigned char*)off = 0xe9;
	int foffset = loc - (off + 5);
	memcpy((void*)(off + 1), &foffset, 4);
}

static void __call(unsigned int off, unsigned int loc) {
	mprotect((void *)off, 5, PROT_READ | PROT_WRITE | PROT_EXEC);
	int foffset = loc - (off + 5);
	memcpy((void *)(off + 1), &foffset, 4);
}

#include "openssl/md5.h"

/*
	MD5
*/

static char xhash_md5[33];

static char* get_md5(char* str) {
	unsigned char digest[MD5_DIGEST_LENGTH];
	MD5((unsigned char*)str, strlen(str), (unsigned char*)&digest);
	for(int i = 0; i < 16; i++)
		sprintf(&xhash_md5[i*2], "%02x", (unsigned int)digest[i]);
	return &xhash_md5[0];
}

static char* get_pass_hash(char* password, char* salt) {
	char tmp[32*((!strcmp(salt,""))?1:2)+1];
	if(strcmp(salt,"")) {
		sprintf(tmp, "%s", get_md5(salt));
		sprintf(tmp, "%s%s", tmp, get_md5(password));
	} else {
		sprintf(tmp, "%s", get_md5(password));
	}
	return get_md5(tmp);
}


/*
	Thanks to kung foo man for providing unprotect_lib
*/

static int unprotect_lib(char *libname)
{
    char buf[512];
    char flags[4];
    void *low, *high;
    FILE *fp;
    fp = fopen("/proc/self/maps", "r");
    if ( ! fp)
        return 0;
    while (fgets(buf, sizeof(buf), fp))
    {
        if ( ! strstr(buf, libname))
            continue;
        if (sscanf (buf, "%p-%p %4c", &low, &high, flags) != 3)
            continue;
        //printf(buf);
        //printf("%08x %08x diff:%.8p\n", low, high, (int)high-(int)low);
        mprotect((void *)low, (int)high-(int)low, PROT_READ | PROT_WRITE | PROT_EXEC);
    }
    fclose(fp);
    return 1;
}

#endif