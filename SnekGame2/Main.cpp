#include "Program.h"
#include "luaextern.h"
int main()

{
	sf::err().rdbuf(NULL);
	Program p(1000, 1000);
	int ret = p.mainLoop();
	return ret;
}