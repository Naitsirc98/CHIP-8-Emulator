#ifndef CHIP_8 
#define CHIP_8
#include <stdint.h>
#include <iostream>

class CPU
{
public:
	void init();
	void cycle(); // Performs a cpu cycle
	void debug();
public:

	uint16_t incrementPC;

	/* General Registers */
	uint8_t registers[16]; // V0-V15

	/* Special Registers */
	uint16_t I;
	uint16_t pc;
	uint8_t sp;
	uint16_t stack[16];

	/* Timers */
	uint8_t delayTimer;
	uint8_t soundTimer;

	/* Instruction set */

	void cls(); /*00E0 CLS*/
	void ret(); /*00ee RET*/
	void jp(uint16_t opcode); /*1nnn JP addr*/
	void call(uint16_t opcode); /*2nnn CALL addr*/
	void se(uint16_t opcode); /*3xkk SE Vx, byte*/
	void sne(uint16_t opcode); /*4xkk SNE Vx, byte*/
	void se2(uint16_t opcode); /*5xy0 SE Vx, Vy*/
	void ld(uint16_t opcode); /*6xkk LD Vx, byte*/
	void add(uint16_t opcode); /*7xkk ADD Vx, byte*/ 
	void ld2(uint16_t opcode); /*8xy0 LD Vx, Vy*/
	void or(uint16_t opcode); /*8xy2 OR Vx, Vy*/
	void and(uint16_t opcode); /*8xy3 AND Vx, Vy*/
	void xor(uint16_t opcode); /*8xy4 XOR Vx, Vy*/
};


/* Vx += nn */
/*
void add(uint16_t opcode, CPU& cpu)
{
	opcode &= 0x0fff;

	std::cout << "opcode add: " << std::hex << opcode << std::endl;

	uint8_t reg = opcode >> 8;

	uint8_t value = opcode & 0xff;

	std::cout << "V[" << (int)reg << "] = " << (int)value << std::endl;

	cpu.registers[reg] = value;

}
*/

/* */

#endif // CHIP-8
