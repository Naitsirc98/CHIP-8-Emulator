#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>

class Keyboard
{
private:
	static Keyboard* instance;
	uint8_t keys[16];
	Keyboard();
public:
	static Keyboard& getKeyboard();
	static void deleteKeyboard();
	uint8_t& operator[](int i);
	uint8_t operator[](int i) const;
	void setKeyPressed(unsigned int key);
	void setKeyReleased(unsigned int key);
	int anyKeyPressed();
};


#endif
