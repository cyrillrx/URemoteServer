#include "Server.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 8082 
#define MAX_CONCURRENT_CONNECTIONS 2

int main()
{
	Server *uRemoteServer = new Server(PORT, MAX_CONCURRENT_CONNECTIONS);
	
	uRemoteServer->Launch();
	
	delete(uRemoteServer);
	uRemoteServer = NULL;

    return EXIT_SUCCESS;
}