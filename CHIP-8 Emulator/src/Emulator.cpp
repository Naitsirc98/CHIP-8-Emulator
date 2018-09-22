#include "Emulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

extern int errno;

void Emulator::run()
{
	while(running)
	{
		display->handleEvents(running);
		cpu->cycle();
		if(cpu->drawFlag)
		{
			display->render();
		}
	}

}

Emulator::Emulator()
{
	display = new Display();
	cpu = new CPU(*display);
}

Emulator::~Emulator()
{

	delete display;

	delete cpu;
}

inline std::streampos fsize(std::ifstream& file)
{
	file.seekg(0, std::ios::end);
	std::streampos size = file.tellg();
	file.seekg(0, std::ios::beg);
	return size;
}

void Emulator::loadFile(char* filename)
{
	// "C:/Users/naits/Downloads/myChip8-bin-src/myChip8-bin-src/tetris.c8"

	std::ifstream file(filename, std::ios_base::binary);

	if(!file.is_open())
	{
		std::cerr << "ERROR: " << strerror(errno) << std::endl;
	}

	std::streampos size = fsize(file);

	std::cout << size << std::endl;

	if(size > (4096 - 512))
	{
		std::cout << "Not enough memory error" << std::endl;
	}

	char* buffer = new char[size];

	file.read(buffer, size);

	if(file.fail())
	{
		std::cerr << "ERROR: " << strerror(errno) << std::endl;
	}

	file.close();

	if(file.fail())
	{
		std::cerr << "ERROR: " << strerror(errno) << std::endl;
	}

	cpu->load(size, buffer);

	delete[] buffer;
}

void Emulator::start()
{
	running = true;
	cpu->init();
	display->init();
	display->render();
	run();
}

void Emulator::stop()
{
	running = false;
}
