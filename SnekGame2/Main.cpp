#include "Program.h"

int main()

{
	sf::err().rdbuf(NULL);

	Program p(1000, 1000);
	return p.mainLoop();
}