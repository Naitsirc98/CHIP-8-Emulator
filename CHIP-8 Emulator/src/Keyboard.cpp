#include "Keyboard.h"
#include <SDL.h>

Keyboard* Keyboard::instance = nullptr;

static int getKeyValue(unsigned int key)
{
	switch(key) {

	case SDLK_1:
		return 0x1;
	case SDLK_2:
		return 0x2;
	case SDLK_3:
		return 0x4;
	case SDLK_4:
		return 0xC;
	case SDLK_q:
		return 0x4;
	case SDLK_w:
		return 0x5;
	case SDLK_e:
		return 0x6;
	case SDLK_r:
		return 0xD;
	case SDLK_a:
		return 0x7;
	case SDLK_s:
		return 0x8;
	case SDLK_d:
		return 0x9;
	case SDLK_f:
		return 0xE;
	case SDLK_z:
		return 0xA;
	case SDLK_x:
		return 0x0;
	case SDLK_c:
		return 0xB;
	case SDLK_v:
		return 0xF;
	default:
		return -1;
	}
}

Keyboard::Keyboard()
{

}

Keyboard& Keyboard::getKeyboard()
{
	if(instance == nullptr)
	{
		instance = new Keyboard();
	}

	return *instance;
}

void Keyboard::deleteKeyboard()
{
	if(instance != nullptr)
		delete instance;
}

uint8_t& Keyboard::operator[](int i)
{
	return keys[i];
}

uint8_t Keyboard::operator[](int i) const
{
	return keys[i];
}

void Keyboard::setKeyPressed(unsigned int key)
{
	keys[getKeyValue(key)] = 1;
}

void Keyboard::setKeyReleased(unsigned int key)
{
	keys[getKeyValue(key)] = 0;
}

int Keyboard::anyKeyPressed()
{
	for(int i = 0; i < 16; i++)
	{
		if(keys[i] != 0)
		{
			return i;
		}
	}
	return -1;
}


