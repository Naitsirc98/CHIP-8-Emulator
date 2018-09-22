#include <iostream>
#include <time.h>
#include "CPU.h"
#include "Keyboard.h"

#define print(x) std::cout << x;
#define println(x) std::cout << x << std::endl;
#define operror println("ERROR: Could not find opcode!")
#define Vx(opcode) (uint8_t) ((opcode & 0x0f00) >> 8)
#define Vy(opcode) (uint8_t) ((opcode & 0x00f0) >> 4)
#define nibble(opcode) (uint8_t) (opcode & 0x000f)
#define byte(opcode) (uint8_t) (opcode & 0x00ff)
#define addr(opcode) (uint16_t) (opcode & 0x0fff)


CPU::CPU(Display& display) : display(display)
{
	init();
}

void CPU::init()
{

	I = 0;
	pc = 0x200;
	sp = 0;
	delayTimer = 0;
	soundTimer = 0;
	instruction = nullptr;
	incrementPC = 2;
	drawFlag = true;

	memset(RAM, 0, 4096);

	Keyboard& keyboard = Keyboard::getKeyboard();

	for(int i = 0; i < 16; i++)
	{
		registers[i] = 0;
		stack[i] = 0;
		keyboard[i] = 0;
	}

	loadFont();

	for(int i = 0; i < 80; i++)
	{
		std::cout << i << " = " << std::hex << (int) readRAM(i) << std::endl;
	}

	srand(time(NULL));
}

void CPU::debug()
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

uint8_t CPU::getV(int i)
{
	return registers[i];
}

void CPU::setV(int i, uint8_t value)
{
	registers[i] = value;
}

uint16_t CPU::getPC()
{
	return pc;
}

uint16_t CPU::getI()
{
	return I;
}

uint8_t CPU::getSP()
{
	return sp;
}

uint16_t CPU::top()
{
	return stack[sp];
}

uint16_t CPU::pop()
{
	return stack[--sp];
}

uint16_t CPU::push(uint16_t value)
{
	stack[sp++] = value;
	return sp;
}

void CPU::load(int size, char * buffer)
{

	for(int i = 0; i < size; i++)
	{
		writeRAM(pc+i, buffer[i]);
	}

}


void CPU::cycle()
{
	static int c = 0;

	// print("cycle ");
	// println(c++);

	incrementPC = 2;

	uint16_t opcode = readRAM16(pc);

	std::cout << "opcode = " << std::hex << opcode << std::endl;

	decode(opcode);

	(this->*instruction)(opcode);

	if(incrementPC == 0xf) // Skip cycle
		return;

	if(delayTimer > 0)
		delayTimer--;
	
	if(soundTimer > 0)
	{
		if(soundTimer == 1)
		{
			println("BEEP\7");
		}
		soundTimer--;
	}

	pc += incrementPC;
}

/* MEMORY FUNCTIONS */

uint8_t CPU::readRAM(int address)
{
	return RAM[address];
}

uint16_t CPU::readRAM16(int address)
{
	return RAM[address] << 8 | RAM[address+1];
}

void CPU::writeRAM(int address, uint8_t data)
{
	RAM[address] = data;
}

void CPU::writeRAM16(int address, uint16_t data)
{
	RAM[address] = data >> 8;
	RAM[address + 1] = data & 0xff;
}

/*...*/


/* INSTRUCTIONS */


void CPU::cls(uint16_t opcode)
{
	display.clear();
	drawFlag = true;
}

void CPU::ret(uint16_t opcode)
{
	pc = pop();
}

void CPU::jp(uint16_t opcode)
{
	pc = addr(opcode);
	incrementPC = 0;
}

void CPU::call(uint16_t opcode)
{
	push(pc);
	pc = addr(opcode);
	incrementPC = 0;
}

void CPU::se(uint16_t opcode)
{
	if(registers[Vx(opcode)] == byte(opcode))
	{
		incrementPC = 4;
	}
}

void CPU::sne(uint16_t opcode)
{
	if(registers[Vx(opcode)] != byte(opcode))
	{
		incrementPC = 4;
	}
}

void CPU::sev(uint16_t opcode)
{
	if(registers[Vx(opcode)] == registers[Vy(opcode)])
	{
		incrementPC = 4;
	}
}

void CPU::ld(uint16_t opcode)
{
	registers[Vx(opcode)] = byte(opcode);
}

void CPU::add(uint16_t opcode)
{
	registers[Vx(opcode)] += byte(opcode);
}

void CPU::ldv(uint16_t opcode)
{
	registers[Vx(opcode)] = registers[Vy(opcode)];
}

void CPU::or(uint16_t opcode)
{
	registers[Vx(opcode)] |= registers[Vy(opcode)];
}

void CPU::and(uint16_t opcode)
{
	registers[Vx(opcode)] &= registers[Vy(opcode)];
}

void CPU::xor(uint16_t opcode)
{
	registers[Vx(opcode)] ^= registers[Vy(opcode)];
}

void CPU::addv(uint16_t opcode)
{
	uint8_t result = registers[Vx(opcode)] += registers[Vy(opcode)];
	registers[VF] = result > 255 ? 1 : 0;
}

void CPU::sub(uint16_t opcode)
{
	uint8_t x = registers[Vx(opcode)];
	uint8_t y = registers[Vy(opcode)];
	registers[VF] = x > y ? 1 : 0;
	registers[Vx(opcode)] -= y;
	
}

void CPU::shr(uint16_t opcode)
{
	registers[VF] = registers[Vx(opcode)] & 0x1;
	registers[Vx(opcode)] >>= 1;
}

void CPU::subn(uint16_t opcode)
{
	uint8_t x = registers[Vx(opcode)];
	uint8_t y = registers[Vy(opcode)];
	registers[VF] = y > x ? 1 : 0;
	registers[Vx(opcode)] = y - x;
}

void CPU::shl(uint16_t opcode)
{
	registers[VF] = (registers[Vx(opcode)]) >> 7;
	registers[Vx(opcode)] <<= 1;
}

void CPU::snev(uint16_t opcode)
{
	incrementPC = registers[Vx(opcode)] != registers[Vy(opcode)]
		? 4 
		: 2;
}

void CPU::ldI(uint16_t opcode)
{
	I = addr(opcode);
}

void CPU::jpv0(uint16_t opcode)
{
	pc = registers[0] + addr(opcode);
	incrementPC = 0;
}

void CPU::rnd(uint16_t opcode)
{
	uint8_t random = rand() % 0xff;
	registers[Vx(opcode)] = random & byte(opcode);
}

void CPU::drw(uint16_t opcode)
{

	uint8_t x = registers[Vx(opcode)]; 
	uint8_t y = registers[Vy(opcode)];

	registers[VF] = 0; 

	for(int yLine = 0; yLine < nibble(opcode); yLine++)
	{
		uint8_t pixel = readRAM(I + yLine);

		for(int xLine = 0; xLine < 8; xLine++)
		{
			if((pixel & (0x80 >> xLine)) != 0)
			{

				if(display[x + xLine + ((y + yLine) * 64)] == 1)
				{
					registers[VF] = 1; // Collision
				}
					
				display[x + xLine + ((y + yLine) * 64)] ^= 1;
			}
		}
	}
	drawFlag = true;
}

void CPU::skp(uint16_t opcode)
{
	
	if(Keyboard::getKeyboard()[registers[Vx(opcode)]] != 0)
	{
		incrementPC = 4;
	}
}

void CPU::sknp(uint16_t opcode)
{
	
	if(Keyboard::getKeyboard()[registers[Vx(opcode)]] == 0)
	{
		incrementPC = 4;
	}
}

void CPU::ldvdt(uint16_t opcode)
{
	registers[Vx(opcode)] = delayTimer;
}

void CPU::ldkey(uint16_t opcode)
{
	int key = Keyboard::getKeyboard().anyKeyPressed();

	if(key == -1)
	{
		incrementPC = 0xf;
	}
	else
	{
		registers[Vx(opcode)] = (uint8_t) key;
	}
}

void CPU::lddtv(uint16_t opcode)
{
	delayTimer = registers[Vx(opcode)];
}

void CPU::ldst(uint16_t opcode)
{
	soundTimer = registers[Vx(opcode)];
}

void CPU::addI(uint16_t opcode)
{
	uint8_t x = registers[Vx(opcode)];
	registers[VF] = I + x > 0xfff ? 1 : 0;
	I += x;
}

void CPU::ldf(uint16_t opcode)
{
	std::cout << "V[" <<(int) Vx(opcode) << "] = " << (int) registers[Vx(opcode)] << std::endl;
	I = registers[Vx(opcode)] * 5;
	std::cout << std::hex << (int)I << std::endl;
}

void CPU::ldbcd(uint16_t opcode)
{
	uint8_t vx = registers[Vx(opcode)];
	writeRAM(I, vx / 100);
	writeRAM(I + 1, (vx / 10) % 10);
	writeRAM(I + 2, (vx % 100) % 10);
}

void CPU::ldIv(uint16_t opcode)
{
	for(int i = 0; i <= Vx(opcode); i++)
	{
		writeRAM(I+i, registers[i]);
	}
	I += Vx(opcode) + 1;
}

void CPU::ldvI(uint16_t opcode)
{
	for(int i = 0; i <= Vx(opcode); i++)
	{
		registers[i] = readRAM(I+i);
	}
	I += Vx(opcode) + 1;
}


/* DECODING FUNCTIONS */

void CPU::decode(uint16_t opcode)
{
	uint8_t hbit = (opcode & 0xf000) >> 12;

	switch(hbit)
	{
	case 0x0:
		switch0(opcode);
		break;
	case 0x1:
		instruction = &CPU::jp;
		break;
	case 0x2:
		instruction = &CPU::call;
		break;
	case 0x3:
		instruction = &CPU::se;
		break;
	case 0x4:
		instruction = &CPU::sne;
		break;
	case 0x5:
		instruction = &CPU::sev;
		break;
	case 0x6:
		instruction = &CPU::ld;
		break;
	case 0x7:
		instruction = &CPU::add;
		break;
	case 0x8:
		switch8(opcode);
		break;
	case 0x9:
		instruction = &CPU::snev;
		break;
	case 0xa:
		instruction = &CPU::ldI;
		break;
	case 0xb:
		instruction = &CPU::jpv0;
		break;
	case 0xc:
		instruction = &CPU::rnd;
		break;
	case 0xd:
		instruction = &CPU::drw;
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

void CPU::switch0(uint16_t opcode)
{

	switch(opcode & 0x000f)
	{
	case 0x0:
		instruction = &CPU::cls;
		break;
	case 0xe:
		instruction = &CPU::ret;
		break;
	default:
		operror;
	}
}

void CPU::switch8(uint16_t opcode)
{

	switch(opcode & 0x000f)
	{
	case 0x0:
		instruction = &CPU::ldv;
		break;
	case 0x1:
		instruction = &CPU::or ;
		break;
	case 0x2:
		instruction = &CPU::and;
		break;
	case 0x3:
		instruction = &CPU::xor;
		break;
	case 0x4:
		instruction = &CPU::addv;
		break;
	case 0x5:
		instruction = &CPU::sub;
		break;
	case 0x6:
		instruction = &CPU::shr;
		break;
	case 0x7:
		instruction = &CPU::subn;
		break;
	case 0xe:
		instruction = &CPU::shl;
		break;
	default:
		operror;
	}
}

void CPU::switche(uint16_t opcode)
{
	switch(opcode & 0x00ff)
	{
	case 0x009e:
		instruction = &CPU::skp;
		break;
	case 0x00a1:
		instruction = &CPU::sknp;
		break;
	default:
		operror;
	}
}

void CPU::switchf(uint16_t opcode)
{
	switch(opcode & 0x00ff)
	{
	case 0x0007:
		instruction = &CPU::ldvdt;
		break;
	case 0x000a:
		instruction = &CPU::ldkey;
		break;
	case 0x0015:
		instruction = &CPU::lddtv;
		break;
	case 0x0018:
		instruction = &CPU::ldst;
		break;
	case 0x001e:
		instruction = &CPU::addI;
		break;
	case 0x0029:
		instruction = &CPU::ldf;
		break;
	case 0x0033:
		instruction = &CPU::ldbcd;
		break;
	case 0x0055:
		instruction = &CPU::ldIv;
		break;
	case 0x0065:
		instruction = &CPU::ldvI;
		break;
	default:
		operror;
	}
}

void CPU::loadFont()
{

	static const uint8_t font[80] = 
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, //0
		0x20, 0x60, 0x20, 0x20, 0x70, //1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
		0x90, 0x90, 0xF0, 0x10, 0x10, //4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
		0xF0, 0x10, 0x20, 0x40, 0x40, //7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
		0xF0, 0x90, 0xF0, 0x90, 0x90, //A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
		0xF0, 0x80, 0x80, 0x80, 0xF0, //C
		0xE0, 0x90, 0x90, 0x90, 0xE0, //D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
		0xF0, 0x80, 0xF0, 0x80, 0x80  //F
	};

	for(int i = 0;i < 80;i++)
	{
		RAM[i] = font[i];
	}

}
