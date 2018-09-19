#include <iostream>
#include "../../src/cpu/CPU.h"
#include "../../src/memory/RAM.h"
#define printlnhex(x) std::cout << std::hex << x << std::endl;

int main()
{
	CPU cpu;
	RAM ram;
	
	cpu.init();

	cpu.registers[5] = 0x66;

	cpu.se(0x3566);

	cpu.debug();

	system("pause");
}