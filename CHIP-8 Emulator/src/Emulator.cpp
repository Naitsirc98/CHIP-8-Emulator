#include "Emulator.h"

Emulator::Emulator()
{
	chip = new Chip_8();
}

Emulator::~Emulator()
{
	delete chip;
}

void Emulator::loadFile(char * file)
{
	// Get the file and read it
	uint8_t buffer[] = {1,1,1,1,1,1,1,2,2,3,4,5,3,5,6,5,4,6,7,4,67,5,7,54,6,7,4,76,4};
	chip->load(sizeof(buffer), buffer);
}

void Emulator::start()
{
	running = true;
	chip->debug();
}

void Emulator::stop()
{
	running = false;
}
