#ifndef EMULATOR_H
#define EMULATOR_H
#include "CPU.h"
#include "Display.h"

class Emulator
{
private:
	Display* display;
	CPU* cpu;
	std::string title = "CHIP-8 Emulator by Naitsirc98. Exectuting ";
	bool running = false;
	void run();
	void createTitle(char* filename);
public:
	Emulator();
	~Emulator();
	void loadFile(char* filename);
	void start();
};

#endif

