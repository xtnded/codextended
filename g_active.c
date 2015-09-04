#include "shared.h"
#include "script.h"

vec3_t tmp_vec = {0};

void GScr_SetTempVec(unsigned n) {
	vec3_t vec;
	if(Scr_GetType(0) != VT_VECTOR) {
		printf("NOT A VECTOR!\n");
		return;
	}
	Scr_GetVector(0, vec);
	VectorCopy(vec, tmp_vec);
}

void _SpectatorClientEndFrame(gentity_t *ent) {
	void (*orig)(gentity_t*) = (void(*)(gentity_t*))GAME("SpectatorClientEndFrame");
	orig(ent);
	
//	VectorCopy(tmp_vec, ent->client->ps.origin); //so u can set origin in killcam/spectate mode
	
	#if 0
	//clientsize = 16868, gclientsize orig = 8400

	char client[0x20D0] = {0};
	#define GCLIENT_SIZE 0x20d0
	//gclient_t client;
	static bool once = 0;
	if(!once) {
		printf("clientsize = %d, gclientsize orig = %d\n", sizeof(gclient_t), GCLIENT_SIZE);
		once = true;
	}
	void (*_HudElem_UpdateClient)(playerState_t *, int, int) = (void(*)(playerState_t*,int,int))GAME("HudElem_UpdateClient");
	void (*_StopFollowing)(gentity_t*) = (void(*)(gentity_t*))GAME("StopFollowing");
	int (*GetArchivedPlayerState)(int,int*,char *) = (int(*)(int,int*,char*))GAME("trap_GetArchivedPlayerState");
	ent->svFlags = ent->svFlags & 0xFD | 1;
	ent->takedamage = 0;
	ent->contents = 0;
	ent->client->field_220C = 0;
	ent->client->field_2210 = 0;
	ent->client->ps.field_BC = 0;
	
	ent->client->ps.pm_flags &= 0xfb;
	
	if(ent->client->spectatorClient < 0) {
		LABEL_9:
		if(ent->client->forceSpectatorClient < 0) {
			_StopFollowing(ent);
			return;
		}
		
		if(!GetArchivedPlayerState(ent->client->forceSpectatorClient, &ent->client->archivetime, &client)) {
			_StopFollowing(ent);
			return;
		}
	} else {
		
		ent->client->forceSpectatorClient = ent->client->spectatorClient;
		while(1) {
			if(ent->client->archivetime < 0)
				ent->client->archivetime = 0;
			if(GetArchivedPlayerState(ent->client->spectatorClient, &ent->client->archivetime, &client))
				break;
			if(!ent->client->archivetime) {
				ent->client->spectatorClient = -1;
				ent->client->forceSpectatorClient = -1;
				goto LABEL_9;
			}
			ent->client->archivetime -= 50;
		}
	}
	
	int savedFlags = ent->client->ps.eFlags & 0x20000 | (0xFFFDFFFF);
	memcpy(ent->client, &client, sizeof(client));
	//VectorCopy(client.ps.viewangles, ent->client->ps.viewangles);
	_HudElem_UpdateClient(&ent->client->ps, ent->s.number, 2);
	ent->client->ps.pm_flags &= 0xfb;
	ent->client->ps.pm_flags |= 1;
	if(ent->client->spectatorClient >= 0)
		ent->client->ps.pm_flags |= 2;
	else
		ent->client->ps.pm_flags &= 0xfd;
	ent->client->ps.eFlags = savedFlags;
	#endif
}