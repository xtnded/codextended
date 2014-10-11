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

/*
	unf and don't use
*/

typedef struct {
	int* ps;
	usercmd_t cmd;
} pmove_t;

static int checkjump_result;

int __cdecl X_CheckJump( void ) {
	printf("checjump_result=%x\n",checkjump_result);
	int (*call)( void );
	*(int*)&call = checkjump_result;
	
	int result = call();
	
	printf("result = %d\n", result);
	
	//pmove_t *xm = (pmove_t*)zpm;
	/*
	int *jumpTime = *(int*)(xm->ps + 100);
	
	if ( xm->cmd.serverTime - *jumpTime < 500 ) {
			return qfalse;
	}
	
	if( *(byte*) (xm->ps + 12) & 8) {
		*(byte*)(xm + 26) = 0;
		return 0;
	}
	
	
	*(int *)(zpml + 48) = 0;
	*(int *)(zpml + 44) = 0;
	*(byte*)(xm->ps + 12) |= 8;
	*(int*)(xm->ps + 84) = ENTITYNUM_NONE;
	*(float*)(xm->ps + 40) = 0.0f;
	*(float*)(xm->ps + 104) = 80;
	*/
	/*
	printf("xm = %x\n", (int)xm);
	printf("xm->ps = %x\n", (int)xm->ps);
	printf("xm->ps + 104 = %f\n", *(float*)(xm->ps+104));
	*/
	return result;
}

void CODPatch_CheckJump(void) {
	int start = GAME("BG_EvaluateTrajectory");
	int end = GAME("PM_FootstepEvent");
	byte search[] = {0x55,0x89,0xE5,0x83,0xEC,0x30,0x56,0x53,0xA1};
	int result = search_memory(start,end,search,sizeof(search));
	
	if(result!=-1) {
		cracking_hook_function(result,(int)X_CheckJump);
		checkjump_result=result;
		printf("Patched CheckJump at %x\n", result);
		return;
	}
	printf("Failed to patch CheckJump (-1)\n");
}