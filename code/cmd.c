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

typedef struct cmd_function_s
{
	struct cmd_function_s   *next;
	char                    *name;
	xcommand_t function;
} cmd_function_t;

static cmd_function_t  *cmd_functions = (cmd_function_t*)0x810EA20;

void Cmd_AddCommand( const char *cmd_name, xcommand_t function ) {
	cmd_function_t  *cmd;
	/*
	for ( cmd = cmd_functions ; cmd ; cmd = cmd->next ) {
		if ( !strcmp( cmd_name, cmd->name ) ) {
			if ( function != NULL ) {
				cmd->function = function;
			}
			return;
		}
	}
	*/
	
	_Cmd_AddCommand(cmd_name, function);
}