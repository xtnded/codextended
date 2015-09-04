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
TestGetAddr
	mov eax, SV_AddEntitiesVisibleFromPoint_Intercept
	retn