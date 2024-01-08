#include <iostream>
#include "TCPRelayServer.h"

int main()
{
	netfish::TCPRelayServer server;
	server.Start();

	while (true)
	{
		server.NetUpdate();
		server.SessionsUpdate();
	}
	server.Stop();
	return 0;
}

