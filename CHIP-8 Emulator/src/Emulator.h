#ifndef EMULATOR_H
#define EMULATOR_H
#include "chip_8/Chip_8.h"

class Emulator
{
private:
	Chip_8* chip;
	bool running = false;
public:
	Emulator();
	~Emulator();
	void loadFile(char* file);
	void start();
	void stop();
};

#endif

