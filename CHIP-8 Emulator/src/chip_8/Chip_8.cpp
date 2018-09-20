#include "Chip_8.h"
#include <iostream>
#define print(x) std::cout << x;
#define println(x) std::cout << x << std::endl;
#define operror println("ERROR: Could not find opcode!")
#define VF 15
#define Vx(opcode) (uint8_t) ((opcode & 0x0f00) >> 8)
#define Vy(opcode) (uint8_t) ((opcode & 0x00f0) >> 4)
#define nibble(opcode) (uint8_t) (opcode & 0x000f)
#define byte(opcode) (uint8_t) (opcode & 0x00ff)
#define addr(opcode) (uint16_t) (opcode & 0x0fff)


Chip_8::Chip_8()
{
	init();
}

void Chip_8::init()
{
	I = 0;
	pc = 0x200;
	sp = 255;
	delayTimer = 0;
	soundTimer = 0;
	instruction = nullptr;
	incrementPC = 1;

	for(int i = 0; i < 16; i++)
	{
		registers[i] = 0;
		stack[i] = 0;
	}

}

void Chip_8::debug()
{

	println("-- Chip_8 Status --");
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

void Chip_8::load(uint16_t size, uint8_t * buffer)
{

	for(int i = 0; i < size; i++)
	{
		write(pc+i, buffer[i]);
	}

}

void Chip_8::cycle()
{
	incrementPC = 1;

	uint16_t opcode = read16(pc);

	decode(opcode);

	(this->*instruction)(opcode);

	pc += incrementPC;
}

/* MEMORY FUNCTIONS */

uint8_t Chip_8::read(uint16_t address)
{
	return RAM[address];
}

uint16_t Chip_8::read16(uint16_t address)
{
	return RAM[address] << 8 | RAM[address+1];
}

void Chip_8::write(uint16_t address, uint8_t data)
{
	RAM[address] = data;
}

void Chip_8::write16(uint16_t address, uint16_t data)
{
	RAM[address] = data >> 8;
	RAM[address + 1] = data & 0xff;
}

/*...*/


/* INSTRUCTIONS */


void Chip_8::cls(uint16_t opcode)
{
	// TODO
}

void Chip_8::ret(uint16_t opcode)
{
	pc = stack[sp--];
}

void Chip_8::jp(uint16_t opcode)
{
	pc = opcode & 0xfff;
}

void Chip_8::call(uint16_t opcode)
{
	stack[++sp] = pc;
	pc = addr(opcode);
}

void Chip_8::se(uint16_t opcode)
{
	if(registers[Vx(opcode)] == byte(opcode))
	{
		incrementPC = 2;
	}
}

void Chip_8::sne(uint16_t opcode)
{
	if(registers[Vx(opcode)] != byte(opcode))
	{
		incrementPC = 2;
	}
}

void Chip_8::sev(uint16_t opcode)
{
	if(registers[Vx(opcode)] == registers[Vy(opcode)])
	{
		incrementPC = 2;
	}
}

void Chip_8::ld(uint16_t opcode)
{
	registers[Vx(opcode)] = byte(opcode);
}

void Chip_8::add(uint16_t opcode)
{
	registers[Vx(opcode)] += byte(opcode);
}

void Chip_8::ldv(uint16_t opcode)
{
	registers[Vx(opcode)] = registers[Vy(opcode)];
}

void Chip_8:: or(uint16_t opcode)
{
	registers[Vx(opcode)] |= registers[Vy(opcode)];
}

void Chip_8:: and(uint16_t opcode)
{
	registers[Vx(opcode)] &= registers[Vy(opcode)];
}

void Chip_8:: xor(uint16_t opcode)
{
	registers[Vx(opcode)] ^= registers[Vy(opcode)];
}

void Chip_8::addv(uint16_t opcode)
{
	uint8_t result = registers[Vx(opcode)] += registers[Vy(opcode)];
	registers[VF] = result > 255 ? 1 : 0;
}

void Chip_8::sub(uint16_t opcode)
{
	uint8_t x = registers[Vx(opcode)];
	uint8_t y = registers[Vy(opcode)];
	registers[VF] = x > y ? 1 : 0;
	registers[Vx(opcode)] -= y;
	
}

void Chip_8::shr(uint16_t opcode)
{
	uint8_t vx = registers[Vx(opcode)] >>= 1;
	registers[VF] = vx & 0x000f;
}

void Chip_8::subn(uint16_t opcode)
{
	uint8_t x = registers[Vx(opcode)];
	uint8_t y = registers[Vy(opcode)];
	registers[VF] = y > x ? 1 : 0;
	registers[Vx(opcode)] = y - x;
}

void Chip_8::shl(uint16_t opcode)
{
	uint8_t vx = registers[Vx(opcode)] <<= 1;
	registers[VF] = (vx & 0xf000) >> 12;
}

void Chip_8::snev(uint16_t opcode)
{
	incrementPC = registers[Vx(opcode)] != registers[Vy(opcode)] ?
		2 : 1;
}

void Chip_8::ldI(uint16_t opcode)
{
	I = addr(opcode);
}

void Chip_8::jpv0(uint16_t opcode)
{
	pc = registers[0] + addr(opcode);
}

void Chip_8::rnd(uint16_t opcode)
{
	uint8_t random = rand() % 256;
	registers[Vx(opcode)] = random & byte(opcode);
}

void Chip_8::drw(uint16_t opcode)
{
	// TODO
}

void Chip_8::skp(uint16_t opcode)
{
	// TODO
}

void Chip_8::sknp(uint16_t opcode)
{
	// TODO
}

void Chip_8::ldvdt(uint16_t opcode)
{
	registers[Vx(opcode)] = delayTimer;
}

void Chip_8::ldkey(uint16_t opcode)
{
	// TODO
}

void Chip_8::lddtv(uint16_t opcode)
{
	delayTimer = registers[Vx(opcode)];
}

void Chip_8::ldst(uint16_t opcode)
{
	soundTimer = registers[Vx(opcode)];
}

void Chip_8::addI(uint16_t opcode)
{
	I += registers[Vx(opcode)];
}

void Chip_8::ldf(uint16_t opcode)
{
	// TODO
}

void Chip_8::ldbcd(uint16_t opcode)
{
	// TODO
}

void Chip_8::ldIv(uint16_t opcode)
{
	for(int i = 0; i < Vx(opcode); i++)
	{
		write(I+i, registers[i]);
	}
}

void Chip_8::ldvI(uint16_t opcode)
{
	for(int i = 0; i < Vx(opcode); i++)
	{
		registers[i] = read(I+i);
	}
}


void Chip_8::decode(uint16_t opcode)
{
	uint8_t hbit = (opcode & 0xf000) >> 12;

	switch(hbit)
	{
	case 0x0:
		switch0(opcode);
		break;
	case 0x1:
		instruction = &Chip_8::jp;
		break;
	case 0x2:
		instruction = &Chip_8::call;
		break;
	case 0x3:
		instruction = &Chip_8::se;
		break;
	case 0x4:
		instruction = &Chip_8::sne;
		break;
	case 0x5:
		instruction = &Chip_8::sev;
		break;
	case 0x6:
		instruction = &Chip_8::ld;
		break;
	case 0x7:
		instruction = &Chip_8::add;
		break;
	case 0x8:
		switch8(opcode);
		break;
	case 0x9:
		instruction = &Chip_8::snev;
		break;
	case 0xa:
		instruction = &Chip_8::ldI;
		break;
	case 0xb:
		instruction = &Chip_8::jpv0;
		break;
	case 0xc:
		instruction = &Chip_8::rnd;
		break;
	case 0xd:
		instruction = &Chip_8::drw;
		break;
	case 0xe:
		switche(opcode);
		break;
	case 0xf:
		switchf(opcode);
		break;
	default:
		operror;
	}
}

void Chip_8::switch0(uint16_t opcode)
{

	switch(opcode & 0x000f)
	{
	case 0x0:
		instruction = &Chip_8::cls;
		break;
	case 0xe:
		instruction = &Chip_8::ret;
	default:
		operror;
	}
}

void Chip_8::switch8(uint16_t opcode)
{

	switch(opcode & 0x000f)
	{
	case 0x0:
		instruction = &Chip_8::ldv;
		break;
	case 0x1:
		instruction = &Chip_8::or ;
		break;
	case 0x2:
		instruction = &Chip_8::and;
		break;
	case 0x3:
		instruction = &Chip_8::xor;
		break;
	case 0x4:
		instruction = &Chip_8::addv;
		break;
	case 0x5:
		instruction = &Chip_8::sub;
		break;
	case 0x6:
		instruction = &Chip_8::shr;
		break;
	case 0x7:
		instruction = &Chip_8::subn;
		break;
	case 0xe:
		instruction = &Chip_8::shl;
		break;
	default:
		operror;
	}
}

void Chip_8::switche(uint16_t opcode)
{
	switch(opcode & 0x00ff)
	{
	case 0x009e:
		instruction = &Chip_8::skp;
		break;
	case 0x00a1:
		instruction = &Chip_8::sknp;
		break;
	default:
		operror;
	}
}

void Chip_8::switchf(uint16_t opcode)
{
	switch(opcode & 0x00ff)
	{
	case 0x0007:
		instruction = &Chip_8::ldvdt;
		break;
	case 0x000a:
		instruction = &Chip_8::ldkey;
		break;
	case 0x0015:
		instruction = &Chip_8::lddtv;
		break;
	case 0x0018:
		instruction = &Chip_8::ldst;
		break;
	case 0x001e:
		instruction = &Chip_8::addI;
		break;
	case 0x0029:
		instruction = &Chip_8::ldf;
		break;
	case 0x0033:
		instruction = &Chip_8::ldbcd;
		break;
	case 0x0055:
		instruction = &Chip_8::ldIv;
		break;
	case 0x0065:
		instruction = &Chip_8::ldvI;
		break;
	default:
		operror;
	}
}