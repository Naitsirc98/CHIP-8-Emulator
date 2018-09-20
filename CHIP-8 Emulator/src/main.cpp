#include "Emulator.h"
#include <iostream>

int main()
{

	Emulator e;

	e.loadFile("");

	e.start();

	e.stop();

	system("pause");

}