/*
	riicchhaarrd
*/

#ifndef PATCH_H
#define PATCH_H
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

#endif // PATCH_H
