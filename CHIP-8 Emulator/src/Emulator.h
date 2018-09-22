#ifndef EMULATOR_H
#define EMULATOR_H
#include "cpu/CPU.h"
#include "display/Display.h"

class Emulator
{
private:
	Display* display;
	CPU* cpu;
	bool running = false;
	void run();
public:
	Emulator();
	~Emulator();
	void loadFile(char* filename);
	void start();
	void stop();
};

#endif

