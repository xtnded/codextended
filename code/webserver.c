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

#ifndef F_WEBSERVER
#include <stdbool.h>
void WebServer_Stop() {}
bool WebServer_Start() {}
#else

#include "server.h"
#include <pthread.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h> // bk001204

#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <errno.h>
#include <net/if.h>


bool bServRunning = false;

volatile int server_sock;
pthread_t h_thread_serverListener;


char *responseHeader = "HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n";

char response[] = "HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title>"
"<style>body { background-color: #111 }"
"h1 { font-size:4cm; text-align: center; color: black;"
" text-shadow: 0 0 2mm red}</style></head>"
"<body><h1>Goodbye, world!</h1></body></html>\r\n";

volatile char r_contents[40321];

void *WebServer_Listen(void *arg) {
	int client_fd;
	
	struct sockaddr_in client_addr;
	socklen_t sin_len = sizeof(client_addr);
	
	char string[1024] = {0};
	int send_len, i;
	
	while(bServRunning) {
		client_fd = accept(server_sock, (struct sockaddr*)&client_addr, &sin_len);
		cprintf(PRINT_GOOD, "Got connection on webserver.\n");
		
		if(client_fd == -1) {
			cprintf(PRINT_ERR | PRINT_UNDERLINE, "Error: Couldn't accept.\n");
			goto exthr;
		}
		
		*r_contents = 0;
		Com_sprintf(r_contents, sizeof(r_contents), "%s", responseHeader);
		/* add the html header etc */
		
		Q_strcat(r_contents, sizeof(r_contents), "Webserver info CoDExtended (v20)<br />Players on server<br />");
		client_t *cl;
		
		for(i = 0; i < sv_maxclients->integer; i++) {
			cl = getclient(i);
			if(cl->state >= CS_CONNECTED) {
				
				string[0] = '\0';
				
				Com_sprintf(string, sizeof(string), "%d: %s<br />", get_client_number(cl), cl->name);
				
				Q_strcat(r_contents, sizeof(r_contents), string);
			}
		}
		
		send_len = strlen(r_contents);
		
		write(client_fd, r_contents, send_len);
		close(client_fd);
	}
	exthr:
	bServRunning = false;
	return NULL;
}

bool WebServer_Start() {
	struct sockaddr_in serveraddr;
	
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(server_sock < 0) {
		cprintf(PRINT_ERR | PRINT_UNDERLINE, "Error: Can't open socket.\n");
		return false;
	}
	
	int one = 1;
	
	setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
	
	int port = 28960;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(port);
	
	if(bind(server_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
		cprintf(PRINT_UNDERLINE | PRINT_ERR, "Error: Failed to bind.\n");
		return false;
	}
	
	listen(server_sock, 5);
	bServRunning = true;
	
	int err = pthread_create(&(h_thread_serverListener), NULL, &WebServer_Listen, NULL);
	
	if(err != 0) {
		cprintf(PRINT_ERR | PRINT_UNDERLINE, "Error: Could not create webserver listen thread.\n");
		return false;
	} else
		cprintf(PRINT_GOOD, "Webserver listening thread created.\n");
}

void WebServer_Stop() {
	bServRunning = false;
}
#endif