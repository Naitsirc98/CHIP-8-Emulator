#include "CPU.h"
#include "../memory/RAM.h"
#include <iostream>
#define print(x) std::cout << x;
#define println(x) std::cout << x << std::endl;

RAM ram;

void CPU::init()
{
	I = 0;
	pc = 0x200;
	sp = 255;
	delayTimer = 0;
	soundTimer = 0;

	for(int i = 0; i < 16; i++)
	{
		registers[i] = 0;
		stack[i] = 0;
	}

}

void CPU::debug()
{

	println("-- CPU Status --");
	println("");

	print("pc = ");
	println((int)pc);
	print("I = ");
	println((int)I);
	print("sp = ");
	println((int)sp);
	print("incrementPC = ");
	println((int)incrementPC);
	println("");

	println("-- General Registers V --");
	println("");

	for(int i = 0; i < 16; i++)
	{
		print("V");
		print(i);
		print(" = ");
		println((int)registers[i]);
	}

	println("");

	println("-- Stack --");
	println("");
	for(int i = 15; i >= 0; i--)
	{
		print("Stack[");
		print(i);
		print("] = ");
		println((int)stack[i]);
	}
	println("");
	println("-- --");
	println("");
}

void CPU::cycle()
{
	incrementPC = 1;
	// Fetch
	uint16_t opcode = ram.read(pc) << 8 | ram.read(pc+1);

	// std::cout << std::hex << (int)opcode << std::endl;

	// execute

	// increment pc
	pc += incrementPC;

}

void CPU::cls()
{
	// TODO
}

void CPU::ret()
{
	pc = stack[sp--];
}

void CPU::jp(uint16_t opcode)
{
	pc = opcode & 0xfff;
}

void CPU::call(uint16_t opcode)
{
	stack[++sp] = pc;
	pc = opcode & 0x0fff;
}

void CPU::se(uint16_t opcode)
{
	if(registers[(opcode & 0x0f00) >> 8] == (uint8_t) opcode & 0x00ff)
	{
		incrementPC = 2;
	}
}

void CPU::sne(uint16_t opcode)
{
	if(registers[(opcode & 0x0f00) >> 8] != (uint8_t) opcode & 0x00ff)
	{
		incrementPC = 2;
	}
}

void CPU::se2(uint16_t opcode)
{
	if(registers[(opcode & 0x0f00) >> 8] == registers[(opcode & 0x00f0) >> 4])
	{
		incrementPC = 2;
	}
}

void CPU::ld(uint16_t opcode)
{
	registers[(opcode & 0x0f00) >> 8] = (uint8_t) (opcode & 0x00ff);
}

void CPU::add(uint16_t opcode)
{
	registers[(opcode & 0x0f00) >> 8] += (uint8_t) (opcode & 0x00ff);
}

void CPU::ld2(uint16_t opcode)
{
	registers[(opcode & 0x0f00) >> 8] = registers[(opcode & 0x00f0) >> 4];
}

void CPU:: or (uint16_t opcode)
{
	registers[(opcode & 0x0f00) >> 8] |= registers[(opcode & 0x00f0) >> 4];
}

void CPU:: and (uint16_t opcode)
{
	registers[(opcode & 0x0f00) >> 8] &= registers[(opcode & 0x00f0) >> 4];
}

void CPU:: xor (uint16_t opcode)
{
	registers[(opcode & 0x0f00) >> 8] ^= registers[(opcode & 0x00f0) >> 4];
}


