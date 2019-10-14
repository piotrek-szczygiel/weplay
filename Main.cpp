#include "Console.h"
#include <ctime>

int main() {
	srand(static_cast<unsigned>(time(nullptr)));

	Console c;
	c.Initialize();
	c.Run();

	return 0;
}