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

#include "script.hpp"
#include "server.hpp"

void GScr_setVelocity(int entityIndex) {
    vec3_t vec;
    Scr_GetVector(0, vec);
    Entity *ent = game->getEntity(entityIndex);
    if(ent) {
        ent->toPlayerState();
        ent->set(POFF_VELOCITY, vec);
        ent->toEntityState();
    }
    Scr_AddUndefined();
}

void GScr_getVelocity(int entityIndex) {
    vec3_t vec;
    Entity *ent = game->getEntity(entityIndex);
    if(ent) {
        ent->toPlayerState();
        ent->get(POFF_VELOCITY, vec);
        ent->toEntityState();
        Scr_AddVector(vec);
    }
}

/*
    why not just use one function for them
    //richard
*/

void GScr_forwardButtonPressed(int a1) {
    Entity *ent = game->getEntity(a1);
    if(ent) {
        client_t *cl = (client_t*)*(clients+a1);
        if(cl) {
            BYTE b = *(BYTE*)(((int)cl) + 0x10621 + UCMD_FORWARDMOVE);
            if(b == 127) {
                Scr_AddBool(true);
                return;
            }
        }
    }
    Scr_AddBool(false);
}

void GScr_aimButtonPressed(int a1) {
    Entity *ent = game->getEntity(a1);
    if(ent) {
        client_t *cl = (client_t*)*(clients+a1);
        if(cl) {
            BYTE b = *(BYTE*)(((int)cl) + 0x10621 + UCMD_WBUTTONS);
            if((b & 16) == 16) {
                Scr_AddBool(true);
                return;
            }
        }
    }
    Scr_AddBool(false);
}


void GScr_reloadButtonPressed(int a1) {
    Entity *ent = game->getEntity(a1);
    if(ent) {
        client_t *cl = (client_t*)*(clients+a1);
        if(cl) {
            BYTE b = *(BYTE*)(((int)cl) + 0x10621 + UCMD_BUTTONS);
            if((b & 8) == 8) {
                Scr_AddBool(true);
                return;
            }
        }
    }
    Scr_AddBool(false);
}


void GScr_backButtonPressed(int a1) {
    Entity *ent = game->getEntity(a1);
    if(ent) {
        client_t *cl = (client_t*)*(clients+a1);
        if(cl) {
            BYTE b = *(BYTE*)(((int)cl) + 0x10621 + UCMD_FORWARDMOVE);
            if(b == 129) {
                Scr_AddBool(true);
                return;
            }
        }
    }
    Scr_AddBool(false);
}

void GScr_leftButtonPressed(int a1) {
    Entity *ent = game->getEntity(a1);
    if(ent) {
        client_t *cl = (client_t*)*(clients+a1);
        if(cl) {
            BYTE b = *(BYTE*)(((int)cl) + 0x10621 + UCMD_RIGHTMOVE);
            if(b == 129) {
                Scr_AddBool(true);
                return;
            }
        }
    }
    Scr_AddBool(false);
}

void GScr_rightButtonPressed(int a1) {
    Entity *ent = game->getEntity(a1);
    if(ent) {
        client_t *cl = (client_t*)*(clients+a1);
        if(cl) {
            BYTE b = *(BYTE*)(((int)cl) + 0x10621 + UCMD_RIGHTMOVE);
            if(b == 127) {
                Scr_AddBool(true);
                return;
            }
        }
    }
    Scr_AddBool(false);
}

void GScr_moveupButtonPressed(int a1) {
    Entity *ent = game->getEntity(a1);
    if(ent) {
        client_t *cl = (client_t*)*(clients+a1);
        if(cl) {
            BYTE b = *(BYTE*)(((int)cl) + 0x10621 + UCMD_UPMOVE);
            if(b == 127) {
                Scr_AddBool(true);
                return;
            }
        }
    }
    Scr_AddBool(false);
}

void GScr_movedownButtonPressed(int a1) {
    Entity *ent = game->getEntity(a1);
    if(ent) {
        client_t *cl = (client_t*)*(clients+a1);
        if(cl) {
            BYTE b = *(BYTE*)(((int)cl) + 0x10621 + UCMD_UPMOVE);
            if(b == 129) {
                Scr_AddBool(true);
                return;
            }
        }
    }
    Scr_AddBool(false);
}

void GScr_getIP(int a1) {
    Entity *ent = game->getEntity(a1);
    if(ent) {
        char* ip = getClientIP(a1);
        Scr_AddString(ip);
    }
}

void GScr_getPlayerAngles(int entityIndex) {
    Entity *ent = game->getEntity(entityIndex);
    if(ent) {
        vec3_t angles;
        ent->toPlayerState();
        ent->get(POFF_ANGLES, angles);
        ent->toEntityState();
        Scr_AddVector(angles);
    }
}
