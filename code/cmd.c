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

/*
	Patches max execute file, just like in patch 1.3
	
	I quote
	
	`- Increased the max cfg file that can be executed to 64K from 16K.`
	
	- Richard
*/

void Cbuf_Init( void ) {
	void (*original)( void );
	*(int*)&original = 0x805B0B4;
	
	original(); //let's call the original and then modify the limit
	
	*(int*)0x8329328 = 262144; //changed from 16k to 256k //16384;
}