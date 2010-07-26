#include "main.h"
#include "Foreach.h"
#include "../Netlib/MTUDP.h"

bool RUNNING=true;
MTUDP* pServer;

/******************************
* main - program entry point

creates a console based server program
******************************/ 
int main(int argc, const char* argv[]){

	//Server Startup
	pServer = new MTUDP();
	
	pServer->Startup(7500,7501);
	pServer->StartServer();
	pServer->StartListening();

	while (pServer->IsServerOn())
	{



		int a;
		cin >> a;
		pServer->StopServer();
	}

	delete pServer;
	return 0;
}
