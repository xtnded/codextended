#include "server.hpp"

typedef struct {
	char        *name;
	void ( *func )( client_t *cl );
} ucmd_t;

static ucmd_t* ucmds = (ucmd_t*)0x80EE6E0;

int *clients = (int*)svsclients_ptr;

SV_StopDownload_f_t SV_StopDownload_f = (SV_StopDownload_f_t)0x808B318;
SV_BeginDownload_f_t SV_BeginDownload_f = (SV_BeginDownload_f_t)0x808B4D6;

char	*ConcatArgs( int start ) {
	int		i, c, tlen;
	static char	line[MAX_STRING_CHARS];
	int		len;
	char	arg[MAX_STRING_CHARS];

	len = 0;
	c = Cmd_Argc();
	for ( i = start ; i < c ; i++ ) {
		Cmd_ArgvBuffer( i, arg, sizeof( arg ) );
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

/*
int __cdecl Cmd_God_f(int a1)
{
  int result; // eax@1
  int v2; // eax@6
  int v3; // [sp+10h] [bp-8h]@4

  result = CheatsOk(a1);
  if ( result )
  {
    *(_DWORD *)(a1 + 388) ^= 1u;
    if ( *(_DWORD *)(a1 + 388) & 1 )
      v3 = (int)"GAME_GODMODEON";
    else
      v3 = (int)"GAME_GODMODEOFF";
    v2 = va("e \"%s\"", v3);
    result = trap_SendServerCommand(-280575753 * ((a1 - (_DWORD)g_entities) >> 2), 0, v2);
  }
  return result;
}
*/

void Cmd_God(ENTITY* ent) {
	int flag;
	ent->get(EOFF_FLAGS, &flag, sizeof(int));
	flag ^= 1;
	ent->set(EOFF_FLAGS, &flag, sizeof(int));
    SV_SendServerCommand(ent->index, 0, "e \"^2GODMODE ^7toggled\"");
}

void myClientCommand(int num) {
	/*
    Scr_AddInt(num);
    int result = Scr_ExecEntThread(num, 0, callbackPlayerCommand, 1);
    Scr_FreeThread(result);
	*/
	
    char cmd[MAX_STRING_CHARS];
    Cmd_ArgvBuffer(0, cmd, sizeof(cmd));
	
	//printf("%d[%s]\n", num, cmd);
    ENTITY* ent = game->getEntity(num);
	
	if(strcmp(cmd, "say") == 0) {
        char* saidn = ConcatArgs(1);
		char* said = &said[2];

        FILE *f = fopen("/home/cod/chatlog.txt", "a");
        if(f != NULL) {
            fprintf(f, "%d:", num);
            fprintf(f, said);
            fprintf(f, "\n");
            fclose(f);
        }
    } else if(strcmp(cmd, "codextended") == 0) {
        SV_SendServerCommand(num, 0, "e \"This server is running Call of Duty Extended\"");
        return;
    } else if(!strcmp(cmd, "god")) {
		Cmd_God(ent);
		return;
	}
	void (*call)(int);
	*((int*)(&call)) = GAME("ClientCommand");
	call(num);
}

void SV_BeginDownload(client_t* cl) {
    int argc = Cmd_Argc();

    if(argc > 1) {
        const char* arg1 = Cmd_Argv(1);
        char* dlname = (char*)(cl+68196);
        SV_StopDownload_f(cl);
        if(strstr(arg1, "..") != NULL)
            return;
			
        if(!strstr(arg1, ".pk3") || strstr(arg1, "svr") != NULL || strstr(arg1, "pak") != NULL) {
			printf("WARNING: %s is trying to download %s\n", cl->name, arg1);
            return;
        }
        //strcpy(*arg1, "main/php.exe");
    }
    asm ("movl %0, 8(%%esp)"::"r"(cl));
    SV_BeginDownload_f(cl);
}
/*
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
}*/

void getClientIP(int num, char* ip) {
    client_t *cl = (client_t*)*(clients+num);
    if(cl) {
        char buf[256];
        sprintf(buf, "%d.%d.%d.%d", cl->remoteAddress[0], cl->remoteAddress[1], cl->remoteAddress[2], cl->remoteAddress[3]);
		memcpy(ip, buf, 256);
    }
}

void SV_DumpUcmd() {
	printf("Listing ucmds\n");
	ucmd_t  *u;
	for ( u = ucmds ; u->name ; u++ ) {
		printf("ucmd: %s [%x]\n", u->name, (int)u->func);
	}
}

void clientInit() {
	/*ucmd_t  *u;
	for ( u = ucmds ; u->name ; u++ ) {
		if(!strcmp("download", u->name)) {
			u->func = SV_BeginDownload;
		}
		printf("ucmd: %s [%x]\n", u->name, (int)u->func);
	}*/
	
	*(int*)0x80EE704 = (int)SV_BeginDownload;
	Cmd_AddCommand("dumpucmd", SV_DumpUcmd);
}
