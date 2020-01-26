#include "Server.h"
#include <ctime>
int main(int argc, char** argv)

{
	srand(clock());

	Server server(5000);
	return server.mainLoop();
}