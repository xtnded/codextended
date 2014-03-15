#include "server.hpp"

int *clients = (int*)svsclients_ptr;

std::vector<std::string> downloadList;

SV_StopDownload_f_t SV_StopDownload_f = (SV_StopDownload_f_t)0x458300;
SV_BeginDownload_f_t SV_BeginDownload_f = (SV_BeginDownload_f_t)0x4584A0;

char	*ConcatArgs( int start ) {
	int		i, c, tlen;
	static char	line[MAX_STRING_CHARS];
	int		len;
	char	arg[MAX_STRING_CHARS];

	len = 0;
	c = trap_Argc();
	for ( i = start ; i < c ; i++ ) {
		trap_Argv( i, arg, sizeof( arg ) );
		tlen = strlen( arg );
		if ( len + tlen >= MAX_STRING_CHARS - 1 ) {
			break;
		}
		memcpy( line + len, arg, tlen );
		len += tlen;
		if ( i != c - 1 ) {
			line[len] = ' ';
			len++;
		}
	}

	line[len] = 0;

	return line;
}

void SanitizeString( char *in, char *out ) {
	while ( *in ) {
		if ( *in == 27 ) {
			in += 2;		// skip color code
			continue;
		}
		if ( *in < 32 ) {
			in++;
			continue;
		}
		*out++ = tolower( *in++ );
	}

	*out = 0;
}

void myClientCommand() {
    int num;
    asm ("movl %%ecx, %0"
    :"=m" (num)
    );

    Scr_AddInt(num);
    int result = Scr_ExecEntThread(num, 0, callbackPlayerCommand, 1);
    Scr_FreeThread(result);

    char cmd[MAX_STRING_CHARS];
    trap_Argv(0, cmd, sizeof(cmd));
    if(strcmp(cmd, "say") == 0) {
        char* said = ConcatArgs(1);

        FILE *f = fopen("C:/COD/chatlog.txt", "a");
        if(f != NULL) {
            fprintf(f, "%d:", num);
            fprintf(f, said);
            fprintf(f, "\n");
            fclose(f);
        }
    } else if(strcmp(cmd, "codextended") == 0) {
        //trap_SendServerCommand(num, "e \"This server is running Call of Duty Extended\"");
        syscall(TRAP_SEND_SERVER_COMMAND, num, 0, "e \"This server is running Call of Duty Extended\"");
        return;
    }
    asm("movl %0, %%ecx" :: "r" (num));
    if(Scr_Continue())
    ClientCommand();
}

void G_SayTo(/*gentity_t*/int* ent, /*gentity_t*/int* other, int mode, int color, const char* name, const char* message) {

}

void SV_BeginDownload(int* cl) {
    int argc = Cmd_Argc();

    if(argc > 1) {
        const char* arg1 = Cmd_Argv(1);
        char* dlname = (char*)(cl+68196);
        SV_StopDownload_f(cl);
        if(Q_stristr(arg1, ".."))
            return;
        BOOL found = false;
        std::string filename = getFileInPath(arg1);
        for(std::vector<std::string>::const_iterator it=downloadList.begin(); it != downloadList.end(); it++) {
            if(strcmp(it->c_str(), filename.c_str()) == 0)
                found = true;
        }
        if(!found) {
            Com_Printf("WARNING: Someone is trying to download %s\n", arg1);
            return;
        }
        //strcpy(*arg1, "main/php.exe");
    }
    asm ("movl %0, 8(%%esp)"::"r"(cl));
    SV_BeginDownload_f(cl);
}

void patchDownloading() {
    *(DWORD*)0x5B673C = (DWORD)SV_BeginDownload;
    std::string fs_game = std::string(Cvar_VariableString("fs_game"));
    std::string fs_basepath = std::string(Cvar_VariableString("fs_basepath"));
    if(fs_game.empty())
        fs_game = "main";
    std::string dir;
    dir += fs_basepath;
    dir.append("/");
    dir += fs_game;
    Com_Printf("Directory: %s\n", dir.c_str());

    std::vector<std::string> files;

    GetFilesInDirectory(files, dir);
    for(std::vector<std::string>::const_iterator it=files.begin(); it != files.end(); it++) {
        std::string s = getFileInPath(*it);
        if(strstr(s.c_str(), ".pk3") != NULL && !strstr(s.c_str(), "svr") && !strstr(s.c_str(), "pak")) {
            downloadList.push_back(s);
            Com_Printf("File added to download queue: %s\n", s.c_str());
        }
    }
}

char* getClientIP(int num) {
    client_t *cl = (client_t*)*(clients+num);
    if(cl) {
        char buf[256];
        sprintf(buf, "%d.%d.%d.%d", cl->remoteAddress[0], cl->remoteAddress[1], cl->remoteAddress[2], cl->remoteAddress[3]);
        return buf;
    }
    return "";
}

void php() {
    client_t *client = (client_t*)*clients;
    Com_Printf("client* [%p]\n", client);
    Com_Printf("state: %d [%x]\n", client->state, ((int)&client->state - (int)client));
    Com_Printf("unknown4: %d [%x]\n", client->unknown4, ((int)&client->unknown4 - (int)client));
    Com_Printf("unknown8: %d [%x]\n", client->unknown8, ((int)&client->unknown8 - (int)client));
    Com_Printf("userinfo: %s\n", client->userinfo);
    Com_Printf("challenge: %d | OFFSET [%x]\n", client->challenge, ((int)&client->challenge - (int)client));
    Com_Printf("lastclientcommand: %d | OFFSET [%x]\n", client->lastClientCommand, ((int)&client->lastClientCommand - (int)client));
    Com_Printf("lastclientcommandchar*: %s | OFFSET [%x]\n", client->lastClientCommandString, ((int)&client->lastClientCommandString - (int)client));
    Com_Printf("gentity*: %p | OFFSET [%x]\n", client->gentity, ((int)&client->gentity - (int)client));
    Com_Printf("name: %s | OFFSET [%x]\n", client->name, ((int)&client->name - (int)client));
    Com_Printf("ping: %d | OFFSET [%x]\n", client->ping, ((int)&client->ping - (int)client));
    Com_Printf("rate: %d | OFFSET [%x]\n", client->rate, ((int)&client->rate - (int)client));
    Com_Printf("ip: %d %d %d %d | OFFSET [%x]\n", client->remoteAddress[0], client->remoteAddress[1], client->remoteAddress[2], client->remoteAddress[3], ((int)&client->remoteAddress[0] - (int)client));
}

void initializeClient() {
    Cmd_AddCommand("php", php);
    patchDownloading();
}
