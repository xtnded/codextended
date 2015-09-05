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