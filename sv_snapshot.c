#include "server.h"

int SV_AddEntitiesVisibleFromPoint_Stub(sharedEntity_t *ent, unsigned clientNum) {
	//printf("ent = %x, clientNum = %x\n", ent, clientNum);
	if(clientNum > 1023)
		return 1;
	
	if(ent == NULL)
		return 1;
	
	//printf("ent number = %u, svFlags = %u, singleClient = %u\n", ent->s.number, ent->r.svFlags, ent->r.singleClient);
	if(ent->r.svFlags & SVF_SINGLECLIENT) {
		//printf("singleclient ent, clientNum = %u, r.singleClient = %u\n", clientNum, ent->r.singleClient);
		if(clientNum == ent->r.singleClient) {
			return 0;
		} else
			return 1;
	}
	return 0;
}