#include "RAM.h"

uint8_t RAM::read(uint16_t address)
{
	return memory[address];
}

void RAM::write(uint16_t address, uint8_t data)
{
	memory[address] = data;

}

void RAM::write(uint16_t address, uint16_t data)
{
	memory[address] = data >> 8;
	memory[address + 1] = data & 0xff;
}


