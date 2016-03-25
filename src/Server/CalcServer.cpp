#include <iostream>
#include "Server.hpp"

int main()
{
	Server * server = new Server();

	server->run();

	server->stop();

	delete server;

	return 0;
}




