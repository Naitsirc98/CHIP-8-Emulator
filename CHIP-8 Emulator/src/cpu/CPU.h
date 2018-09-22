#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include "../display/Display.h"

#define print(x) std::cout << x;
#define println(x) std::cout << x << std::endl;
#define operror println("ERROR: Could not find opcode!")
#define Vx(opcode) (uint8_t) ((opcode & 0x0f00) >> 8)
#define Vy(opcode) (uint8_t) ((opcode & 0x00f0) >> 4)
#define nibble(opcode) (uint8_t) (opcode & 0x000f)
#define byte(opcode) (uint8_t) (opcode & 0x00ff)
#define addr(opcode) (uint16_t) (opcode & 0x0fff)

const uint8_t VF = 15;

class CPU
{
public:
	bool drawFlag;
	CPU(Display& display);
	void init();
	void cycle(); // Performs a cpu cycle
	void debug();
	uint8_t getV(int i);
	void setV(int i, uint8_t value);
	uint16_t getPC();
	uint16_t getI();
	uint8_t getSP();
	uint16_t top();
	uint16_t pop();
	uint16_t push(uint16_t value);
	void load(int size, char* buffer);
	uint8_t readRAM(int address); // Reads a single byte from memory
	uint16_t readRAM16(int address); // Reads 2 bytes from memory
	void writeRAM(int address, uint8_t data); // Writes 1 byte in memory
	void writeRAM16(int address, uint16_t data); // Writes 2 bytes in memory
private:
	uint8_t RAM[4096]; // Memory of 4KB

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

	Display& display;

	typedef void (CPU::*Instruction)(uint16_t);

	Instruction instruction = nullptr;

	/* Instructions */

	/*
	 * 00E0 - CLS
	 * Clears the display.
	*/
	void cls(uint16_t opcode);

	/*
	 * 00EE - RET
	 * Return from a subroutine.
     * The interpreter sets the program counter to the address 
	 * at the top of the stack, then subtracts 1 from the 
	 * stack pointer.
	*/
	void ret(uint16_t opcode); 
	
	/*
	 * 1nnn - JP addr
	 * Jump to location nnn.
	 * The interpreter sets the program counter to nnn.
	*/
	void jp(uint16_t opcode);

	/*
	 * 2nnn - CALL addr
	 * Call subroutine at nnn.

     * The interpreter increments the stack pointer, 
     * then puts the current PC 
     * on the top of the stack. The PC is then set to nnn.
    */
	void call(uint16_t opcode);

	/*
	 * 3xkk - SE Vx, byte
	 * Skip next instruction if Vx = kk.
	 * The interpreter compares register Vx to kk, 
	 * and if they are equal, 
	 * increments the program counter by 2.*/
	void se(uint16_t opcode);

	/*4xkk - SNE Vx, byte
	 * Skip next instruction if Vx != kk.
     * The interpreter compares register Vx to kk, and if they are
	 * not equal, increments the program counter by 2.
	 */
	void sne(uint16_t opcode); 

	/*5xy0 - SE Vx, Vy
	 * Skip next instruction if Vx = Vy.

	 * The interpreter compares register Vx to register Vy, 
     * and if they are equal, 
     * increments the program counter by 2.*/
	void sev(uint16_t opcode); 

	/*6xkk - LD Vx, byte
	 * Set Vx = kk.
     * The interpreter puts the value kk into register Vx.*/
	void ld(uint16_t opcode); 

	/*7xkk - ADD Vx, byte
	 * Set Vx = Vx + kk.

	 * Adds the value kk to the value of register Vx,
     * then stores the result in Vx. */
	void add(uint16_t opcode); 

	/*8xy0 - LD Vx, Vy
	 * Set Vx = Vy.
	 * Stores the value of register Vy in register Vx.*/
	void ldv(uint16_t opcode); 

	/*8xy1 - OR Vx, Vy
	 * Set Vx = Vx OR Vy.
	 * Performs a bitwise OR on the values of Vx and Vy,
	 * then stores the result in Vx.*/
	void or(uint16_t opcode); 

	/*8xy2 - AND Vx, Vy
	 * Set Vx = Vx AND Vy.
	 * Performs a bitwise AND on the values of Vx and Vy,
	 * then stores the result in Vx*/
	void and(uint16_t opcode);

	/*8xy3 - XOR Vx, Vy
	 * Set Vx = Vx XOR Vy.

	 * Performs a bitwise exclusive OR on the values of Vx and Vy, 
	 * then stores the result in Vx.*/
	void xor(uint16_t opcode);

	/*8xy4 - ADD Vx, Vy
	 * Set Vx = Vx + Vy, set VF = carry.s
	 *The values of Vx and Vy are added together. If the result 
	 *is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. 
	 *Only the lowest 8 bits of the result are kept, and stored in Vx.*/
	void addv(uint16_t opcode);

	/*8xy5 - SUB Vx, Vy
	 * Set Vx = Vx - Vy, set VF = NOT borrow.
	 * If Vx > Vy, then VF is set to 1, otherwise 0.
	 * Then Vy is subtracted from Vx, and the results stored in Vx.*/
	void sub(uint16_t opcode);

	/*8xy6 - SHR Vx {, Vy}
	 * Set Vx = Vx SHR 1.
	 * 
	 * If the least-significant bit of Vx is 1,
	 * then VF is set to 1, otherwise 0. Then Vx is divided by 2.*/
	void shr(uint16_t opcode);

	/*8xy7 - SUBN Vx, Vy
	 * Set Vx = Vy - Vx, set VF = NOT borrow.
	 * 
	 * If Vy > Vx, then VF is set to 1, otherwise 0. 
	 * Then Vx is subtracted from Vy, and the results stored in Vx.*/
	void subn(uint16_t opcode);

	/*8xyE - SHL Vx {, Vy}
	 * Set Vx = Vx SHL 1.
	 * 
	 * If the most-significant bit of Vx is 1, then VF is set to 1, 
	 * otherwise to 0. Then Vx is multiplied by 2.*/
	void shl(uint16_t opcode);


	/*9xy0 - SNE Vx, Vy
	 * Skip next instruction if Vx != Vy.
	 * 
	 * The values of Vx and Vy are compared, and if they are not equal, 
	 * the program counter is increased by 2.*/
	void snev(uint16_t opcode); 

	/*Annn - LD I, addr
	* Set I = nnn.
	* 
	* The value of register I is set to nnn.*/
	void ldI(uint16_t opcode); 

	/*Bnnn - JP V0, addr
	 * Jump to location nnn + V0.
	 * 
	 * The program counter is set to nnn plus the value of V0.*/
	void jpv0(uint16_t opcode);

	/*Cxkk - RND Vx, byte
	 * Set Vx = random byte AND kk.
	 * 
	 * the interpreter generates a random number from 0 to 255,
	 * which is then ANDed with the value kk.*/
	void rnd(uint16_t opcode); 

	/*Dxyn - DRW Vx, Vy, nibble
	 * Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
	 * The interpreter reads n bytes from memory, starting at the address 
	 * stored in I. These bytes are then displayed as sprites on screen 
	 * at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels 
	 * to be erased, VF is set to 1, otherwise it is set to 0. 
	 * If the sprite is positioned so part of it is outside the coordinates
	 * of the display, it wraps around to the opposite side of the screen.*/
	void drw(uint16_t opcode); 

	/*Ex9E - SKP Vx
	 * Skip next instruction if key with the value of Vx is pressed.
	 * 
	 * Checks the keyboard, and if the key corresponding to the value 
	 * of Vx is currently in the down position, PC is increased by 2.*/
	void skp(uint16_t opcode);

	/*ExA1 - SKNP Vx
	 * Skip next instruction if key with the value of Vx is not pressed.
	 * 
	 * Checks the keyboard, and if the key corresponding to the value of Vx 
	 * is currently in the up position, PC is increased by 2.*/
	void sknp(uint16_t opcode); 
	
	/*Fx07 - LD Vx, DT
	 * Set Vx = delay timer value.
	 * 
	 * The value of DT is placed into Vx.*/
	void ldvdt(uint16_t opcode); 

	/*Fx0A - LD Vx, K
	 * Wait for a key press, store the value of the key in Vx.
	 * 
	 * All execution stops until a key is pressed,
	 * then the value of that key is stored in Vx.*/
	void ldkey(uint16_t opcode); 

	/*Fx15 - LD DT, Vx
	 * Set delay timer = Vx.
	 * 
	 * DT is set equal to the value of Vx.*/
	void lddtv(uint16_t opcode); 

	/*Fx18 - LD ST, Vx
	 * Set sound timer = Vx.
	 * 
	 * ST is set equal to the value of Vx.*/
	void ldst(uint16_t opcode);

	/*Fx1E - ADD I, Vx
	 * Set I = I + Vx.
	 * 
	 * The values of I and Vx are added, and the results are stored in I.*/
	void addI(uint16_t opcode); 

	/*Fx29 - LD F, Vx
	 * Set I = location of sprite for digit Vx.
	 * 
	 * The value of I is set to the location for the 
	 * hexadecimal sprite corresponding to the value of Vx.*/
	void ldf(uint16_t opcode); 

	/*Fx33 - LD B, Vx
	 * Store BCD representation of Vx in memory locations I, I+1, and I+2.
	 * 
	 * The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, 
	 * the tens digit at location I+1, and the ones digit at location I+2.*/
	void ldbcd(uint16_t opcode); 

	/*Fx55 - LD [I], Vx
	 * Store registers V0 through Vx in memory starting at location I.
	 * 
	 * The interpreter copies the values of registers V0 through Vx 
	 * into memory, starting at the address in I.*/
	void ldIv(uint16_t opcode); 

	/*Fx65 - LD Vx, [I]
	 * Read registers V0 through Vx from memory starting at location I.
	 * 
	 * The interpreter reads values from memory starting at location I 
	 * into registers V0 through Vx.*/
	void ldvI(uint16_t opcode); 


	/* Decoding */

	void decode(uint16_t opcode);
	void switch0(uint16_t opcode);
	void switch8(uint16_t opcode);
	void switche(uint16_t opcode);
	void switchf(uint16_t opcode);

	void loadFont();

};

#endif // CPU_H
