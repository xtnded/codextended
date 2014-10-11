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

void band_test() {
	int argc = Cmd_Argc();
	if(argc != 3) {
		printf("Error\n");
		return;
	}
	int index = 0;
	if(index < 0 || index >= sv_maxclients->integer) {
		printf("Error, wrong index.\n");
		return;
	}
	client_t* cl = getclient(index);
	NET_OutOfBandPrint( NS_SERVER, cl->remoteAddress, Cmd_Argv(2));
}

/*
==================
SV_AddOperatorCommands
==================
*/
void SV_AddOperatorCommands(void) {
	Cmd_AddCommand("codextended", SV_Version);
	Cmd_AddCommand("trap", SV_Test);
	Cmd_AddCommand("net", band_test);
}
