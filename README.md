# CHIP-8-Emulator

A CHIP-8 Emulator written in modern C++. I used SDL2 for graphics.

Instructions are not implemented within the switch statement but each one has it own function.
So, when the CPU object decodes the opcode, it gets a function pointer and executes it, making the code easy to read and debug.

To separate the CPU with the graphics stuff, I implemented the SDL2 functionality into the Display class. Thus, graphics and logic are
independent and makes the code easy to follow.

Also, Keyboard class keeps track of what key has been pressed/released and maps the original CHIP-8 keyboard into modern ones.

Finally, the class Emulator manage all the previous classes to make them work together.


I have based my work by following the Multigesture.net tutorial (http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/),
Wikipedia and Cowgod's Chip-8 Technical Reference (http://devernay.free.fr/hacks/chip8/C8TECH10.HTM).

