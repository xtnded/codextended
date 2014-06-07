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

DWORD WINAPI tExtend(LPVOID);
DWORD g_threadID;
HMODULE g_hModule;

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            g_hModule = hinstDLL;
            DisableThreadLibraryCalls(hinstDLL);
            CloseHandle(CreateThread(NULL, NULL, &tExtend, NULL, NULL, &g_threadID));
            break;

        case DLL_PROCESS_DETACH:
        case DLL_THREAD_DETACH:
            delete game;
            FreeLibraryAndExitThread(g_hModule, 0);
            break;
    }
    return TRUE;
}


DWORD WINAPI tExtend(LPVOID) {
    game = new COD();
    return 0;
}
