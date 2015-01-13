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

#include <sys/mman.h>
#include <string.h>
#include "openssl/md5.h"

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
	int len = ( end < start ) ? end : ( end - start ), i;	
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

static char *get_md5b(unsigned char *buf, size_t len) {
	unsigned char digest[MD5_DIGEST_LENGTH];
	MD5(buf, len, (unsigned char*)&digest);
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


static void dumpbase(int* base, size_t len, const char *fn) {
	static char dumpbase_path[64];
	
	snprintf(dumpbase_path, 63, "./dumps/%s.dump", fn);
	
	unsigned int result;
	float rf;
	char* rstr;
	
	FILE* f = fopen(dumpbase_path, "wb");
	if(f) {
		fprintf(f, "File: %s\nDUMPFILE\n\n");
		
		for(unsigned int i = 0; i < len; i++) {
			result = *(unsigned int*)(base + i);
			rf = *(float*)(base + i);
			rstr = (char*)(base + i);
			fprintf(f, "%d: Hex: %x, Decimal: %d, Float: %f, String: %s\n", i, result, result, rf, rstr);
		}
		fclose(f);
	}
	
	printf("Successfully dumped '%s' to location '%s'.\n", fn, dumpbase_path);
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

static inline void xor_crypt(char *key, char *string, int n) {
    int i;
	int c_use = 'x';
    int keyLength = strlen(key);
    for(i = 0; i < n; i++)
        string += string[i] ^ (c_use /*+i*/) % 255;//string[i] = string[i] ^ key[i%keyLength];
}

#endif