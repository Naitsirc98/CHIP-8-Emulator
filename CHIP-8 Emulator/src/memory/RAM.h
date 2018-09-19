#ifndef RAM_H
#define RAM_H
#include <stdint.h>

class RAM
{
private:
	uint8_t memory[4096]; // 4KB Memory
public:
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);
	void write(uint16_t address, uint16_t data);
};

#endif