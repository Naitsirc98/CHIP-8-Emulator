#include "../src/Emulator.h"


int main(int argc, char* args[])
{

	Emulator e;

	e.loadFile("...");

	e.start();

	return 0;
}