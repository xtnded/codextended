#include "../script.hpp"
#include "../../server/server.hpp"

void PlayerCmd_setVelocity(int entityIndex) {
    vec3_t vec;
    Script_GetVector(0, vec);
    ENTITY* ent = game->getEntity(entityIndex);
    if(ent) {
        ent->toPlayerState();
        ent->set(POFF_VELOCITY, vec, sizeof(vec));
        ent->toEntityState();
    }
    Script_AddUndefined();
}

void PlayerCmd_getVelocity(int entityIndex) {
    vec3_t vec;
    ENTITY* ent = game->getEntity(entityIndex);
    if(ent) {
        ent->toPlayerState();
        ent->get(POFF_VELOCITY, vec, sizeof(vec));
        ent->toEntityState();
        Script_AddVector(vec);
    }
}

/*
    why not just use one function for them
    //richard
*/

void PlayerCmd_forwardButtonPressed(int a1) {
    ENTITY* ent = game->getEntity(a1);
    if(ent) {
        client_t *cl = (client_t*)*(clients+a1);
        if(cl) {
            byte b = *(byte*)(((int)cl) + 0x10621 + UCMD_FORWARDMOVE);
            if(b == 127) {
                Script_AddBool(true);
                return;
            }
        }
    }
    Script_AddBool(false);
}

void PlayerCmd_aimButtonPressed(int a1) {
    ENTITY* ent = game->getEntity(a1);
    if(ent) {
        client_t *cl = (client_t*)*(clients+a1);
        if(cl) {
            byte b = *(byte*)(((int)cl) + 0x10621 + UCMD_WBUTTONS);
            if((b & 16) == 16) {
                Script_AddBool(true);
                return;
            }
        }
    }
    Script_AddBool(false);
}


void PlayerCmd_reloadButtonPressed(int a1) {
    ENTITY* ent = game->getEntity(a1);
    if(ent) {
        client_t *cl = (client_t*)*(clients+a1);
        if(cl) {
            byte b = *(byte*)(((int)cl) + 0x10621 + UCMD_BUTTONS);
            if((b & 8) == 8) {
                Script_AddBool(true);
                return;
            }
        }
    }
    Script_AddBool(false);
}


void PlayerCmd_backButtonPressed(int a1) {
    ENTITY* ent = game->getEntity(a1);
    if(ent) {
        client_t *cl = (client_t*)*(clients+a1);
        if(cl) {
            byte b = *(byte*)(((int)cl) + 0x10621 + UCMD_FORWARDMOVE);
            if(b == 129) {
                Script_AddBool(true);
                return;
            }
        }
    }
    Script_AddBool(false);
}

void PlayerCmd_leftButtonPressed(int a1) {
    ENTITY* ent = game->getEntity(a1);
    if(ent) {
        client_t *cl = (client_t*)*(clients+a1);
        if(cl) {
            byte b = *(byte*)(((int)cl) + 0x10621 + UCMD_RIGHTMOVE);
            if(b == 129) {
                Script_AddBool(true);
                return;
            }
        }
    }
    Script_AddBool(false);
}

void PlayerCmd_rightButtonPressed(int a1) {
    ENTITY* ent = game->getEntity(a1);
    if(ent) {
        client_t *cl = (client_t*)*(clients+a1);
        if(cl) {
            byte b = *(byte*)(((int)cl) + 0x10621 + UCMD_RIGHTMOVE);
            if(b == 127) {
                Script_AddBool(true);
                return;
            }
        }
    }
    Script_AddBool(false);
}

void PlayerCmd_moveupButtonPressed(int a1) {
    ENTITY* ent = game->getEntity(a1);
    if(ent) {
        client_t *cl = (client_t*)*(clients+a1);
        if(cl) {
            byte b = *(byte*)(((int)cl) + 0x10621 + UCMD_UPMOVE);
            if(b == 127) {
                Script_AddBool(true);
                return;
            }
        }
    }
    Script_AddBool(false);
}

void PlayerCmd_movedownButtonPressed(int a1) {
    ENTITY* ent = game->getEntity(a1);
    if(ent) {
        client_t *cl = (client_t*)*(clients+a1);
        if(cl) {
            byte b = *(byte*)(((int)cl) + 0x10621 + UCMD_UPMOVE);
            if(b == 129) {
                Script_AddBool(true);
                return;
            }
        }
    }
    Script_AddBool(false);
}

void PlayerCmd_getIP(int a1) {
    ENTITY* ent = game->getEntity(a1);
    if(ent) {
        char* ip = getClientIP(a1);
        Script_AddString(ip);
    }
}

void PlayerCmd_getPlayerAngles(int entityIndex) {
    ENTITY* ent = game->getEntity(entityIndex);
    if(ent) {
        vec3_t angles;
        ent->toPlayerState();
        ent->get(POFF_ANGLES, angles, sizeof(angles));
        ent->toEntityState();
        Script_AddVector(angles);
    }
}
