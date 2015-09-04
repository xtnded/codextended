
#define STEAM_SUPPORT

#ifdef STEAM_SUPPORT

int CSteamClient_Init();
void CSteamClient_Shutdown();
char *CSteamClient_GetPersonaName();

//Server Functions
void CSteamServer_Shutdown();
int CSteamServer_Init(uint32_t ip, uint16_t steamPort, uint16_t gamePort,
	uint16_t queryPort, const char *versionString);
void CSteamServer_RunFrame();

#endif