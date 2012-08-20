#include "Server.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 8082 
#define MAX_CONCURRENT_CONNECTIONS 2

int main()
{
	AI *m_ArtificialIntelligence = AI::GetInstance();
	
	m_ArtificialIntelligence->StartConnection(PORT, MAX_CONCURRENT_CONNECTIONS);
	
	AI::FreeInstance();

    return EXIT_SUCCESS;
}