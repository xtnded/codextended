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

void CoDExtended();

#ifdef _WIN32
extern "C" DLL_EXPORT void xtnded() {
}
#endif

#ifdef _WIN32
BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
#elif defined __linux__
void __attribute__ ((constructor)) codextended_load(void) {
#endif
	#ifdef _WIN32
		//if(lpvReserved!=NULL)
      //  return 0;
		switch(fdwReason) {
			case DLL_PROCESS_ATTACH:
				DisableThreadLibraryCalls(hinstDLL);
				#endif
				
				CoDExtended();
				#ifdef _WIN32
			break;
			
			case DLL_PROCESS_DETACH:
				COD_Destructor();
			break;
		}
		return 1;
	#endif
}

#if 0
void __attribute__ ((destructor)) codextended_unload( void ) {
}
#endif