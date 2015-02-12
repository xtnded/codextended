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

int __attribute__((visibility ("default"))) codextended_module_load() {
	CoDExtended();
	return BUILDNUMBER;
}

void __attribute__ ((constructor)) __attribute__((visibility ("default"))) codextended_load(void) {
	CoDExtended();
}
// Odd, isn't it? 1.5 destructor works (apparently it should work but the stdout is closed or something... cba to rewrite for now)
#if CODPATCH == 5
void __attribute__ ((destructor)) __attribute__((visibility ("default"))) codextended_unload( void ) {
	COD_Destructor();
}
#endif