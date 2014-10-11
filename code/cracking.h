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
#ifndef _CRACKING_HPP_
#define _CRACKING_HPP_

#include <string.h>

int cracking_nop(int from, int to);
void cracking_hook_function(int from, int to);
void cracking_hook_call(int from, int to);
int cracking_write_hex(int address, char *hex);

int singleHexToNumber(char hexchar);
int hexToBuffer(char *hex, char *buffer, int bufferLen);

#endif
