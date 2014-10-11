#include "server.hpp"

/*
==================
SV_Version	
==================
*/
void SV_Version() {
	//printf(VERSION_STRING, EXTENDEDVERSION);
	printf("Call of Duty Extended by riicchhaarrd\nA lot of thanks to kungfooman and libcod\n");
}

void SV_Test() {
	int argc = Cmd_Argc();
	printf("trap_argc = %d\n", argc);
}

/*
==================
SV_AddOperatorCommands
==================
*/
void SV_AddOperatorCommands(void) {
	Cmd_AddCommand("codextended", SV_Version);
	Cmd_AddCommand("trap", SV_Test);
}
