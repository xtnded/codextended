#include "../server/server.hpp"

/*
//adding this to prevent of creating a new file net_chan.c
NET_OutOfBandPrint_t NET_OutOfBandPrint = (NET_OutOfBandPrint_t)0x8080920;
*/

NET_SendPacket_t NET_SendPacket = (NET_SendPacket_t)0x8080D28;

#define MAX_MSGLEN              32768

/*
===============
NET_OutOfBandPrint

Sends a text message in an out-of-band datagram
================
*/
void QDECL NET_OutOfBandPrint( netsrc_t sock, netadr_t adr, const char *format, ... ) {
	va_list argptr;
	char string[MAX_MSGLEN];

	// set the header
	string[0] = -1;
	string[1] = -1;
	string[2] = -1;
	string[3] = -1;

	va_start( argptr, format );
	vsnprintf( string + 4, sizeof( string ) - 4, format, argptr );
	va_end( argptr );

	// send the datagram
	NET_SendPacket( sock, strlen( string ), string, adr );
}
