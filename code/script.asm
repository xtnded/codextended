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
	

SECTION .data
str1: db "address is 0x%02X",10,0	
	
SECTION .text

extern printf
extern gsc_hook_to_js_cb

get_eip:
	mov eax, [esp]
	ret

extern gsc_hook_caller
	
global _Scr_AddStuff
_Scr_AddStuff:
	;get eip
	mov eax, [esp]
	sub eax, 5 ;get the caller
	mov [gsc_hook_caller], eax
	
	call gsc_hook_to_js_cb
	cmp [gsc_hook_caller], eax
	je ret_now
	mov eax, [gsc_hook_caller]
	
	ret_now:
	retn
;	push ebp
;	mov ebp, esp
;	push eax
;	push str1
;	call printf
;	mov esp, ebp
;	pop ebp
;	debug print stuff
	
	; mov eax, 80A9A3Ch ; It's Scr_AddInt for testing
	call eax
	retn

global GetAddStuffAddr
GetAddStuffAddr:
	mov eax, _Scr_AddStuff
	retn