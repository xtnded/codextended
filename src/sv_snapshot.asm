;	This file is part of CoDExtended.
;
;    CoDExtended is free software: you can redistribute it and/or modify
;    it under the terms of the GNU General Public License as published by
;    the Free Software Foundation, either version 3 of the License, or
;    (at your option) any later version.
;
;    CoDExtended is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU General Public License for more details.
;
;    You should have received a copy of the GNU General Public License
;    along with CoDExtended.  If not, see <http://www.gnu.org/licenses/>.
	
SECTION .text

extern SV_AddEntitiesVisibleFromPoint_Stub

global SV_AddEntitiesVisibleFromPoint_Intercept
SV_AddEntitiesVisibleFromPoint_Intercept:
	; eax = ent
	mov esi, [ebp + 0Ch] ; clientNum
	push esi
	mov esi, eax
	push eax
	call SV_AddEntitiesVisibleFromPoint_Stub
	test eax, eax
	mov eax, esi
	jz loc_808E336
	
loc_808E48C: ; not visible
	mov esi, 808E48Ch
	jmp esi
	
loc_808E336: ; visible
	mov esi, 808E336h
	jmp esi

global TestGetAddr
TestGetAddr:
	mov eax, SV_AddEntitiesVisibleFromPoint_Intercept
	retn