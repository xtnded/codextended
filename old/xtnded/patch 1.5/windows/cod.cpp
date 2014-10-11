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

#include "cod.hpp"

COD *game;

void DLLInit();

void *Sys_LoadDll( const char *name, char *fqpath, int( **entryPoint ) ( int, ... ), int ( *systemcalls )( int, ... ) ) {
    void *l = call<void*, const char*, char*, int(**)(int,...)>(0x4686D0, name, fqpath, entryPoint, systemcalls);
    DLLInit();
    return l;
}

void DLLInit() {
    game_mp = (int)GetModuleHandle("game_mp_x86.dll");
    Com_Printf("[game_mp: %02x]\n", game_mp);
    cracking_hook_call(0x20031AD8, (int)GScr_LoadGameTypeScript);
    cracking_hook_call(0x2002747C, (int)myClientCommand);

    //cracking_hook_function(0x20038030, (int)Scr_LoadGameType);
    //_memset((void*)0x200198F0, NOP, 5);

    level = *(level_locals_t*)0x202CDB80;
}

void setCracked() {
    int argc = Cmd_Argc();
    if(argc < 2) {
        Com_Printf("Usage: cracked <0|1>\n");
        return;
    }
    int arg = atoi(Cmd_Argv(0));
    if(arg != 0) {
        int loc = 0x456954;
        DWORD old;
        VirtualProtect((void*)loc, 2, PAGE_EXECUTE_READWRITE, &old);
        *(BYTE*)0x456954 = 0x74;
        *(BYTE*)0x456955 = 0x73;
        VirtualProtect((void*)loc, 2, old, &old);
        Com_Printf("Server is now cracked.\n");
    } else {
        _memset((void*)0x456954, 0x90, 2);
        Com_Printf("Server is now not cracked.\n");
    }
}

void version() {
    Com_Printf(VERSION_STRING, EXTENDEDVERSION);
}

COD::COD() {
    /*
    Check for CoD 1.5 patch
    if(memcmp((void*)0x5A60D0, "1.5", 3) == 0)
    }*/

    for(int i = 0; i < MAX_ENTITY_SIZE; i++) {
        this->entities[i].index = i;
        this->entities[i].base = g_entities + GENTITY_SIZE * i;
        this->entities[i].ptr = g_entities + GENTITY_SIZE * i;
    }

    version();

    Cmd_AddCommand("codextended", version);
    Cmd_AddCommand("codversion", version);
    Cmd_AddCommand("help", version);
    Cmd_AddCommand("cracked", setCracked);

    InitScript();
    initializeClient();

    cracking_hook_call(0x465629, (int)Sys_LoadDll);

    if(GetModuleHandle("game_mp_x86.dll") != NULL)
        DLLInit();
}

Entity* COD::getEntities() {
    return entities;
}

Entity* COD::getEntity(unsigned short index) {
    return &entities[index];
}
