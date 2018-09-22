#include "Emulator.h"
#include <iostream>

int main(int argc, char* args[])
{

	if(argc < 2)
	{
		std::cout << "You need to specify a ROM to run the emulator!!" << std::endl;
		std::cout << "You can do it by typing emulator.exe rom_name" << std::endl;
		std::cout << "Or just drag any ROM file on the emulator.exe!" << std::endl;
		
	}

	Emulator emulator;

	emulator.loadFile(args[1]);

	emulator.start();

	return 0;
}