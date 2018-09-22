#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdint.h>
#include <SDL.h>

class Display
{
public:
	static const int SCREEN_SIZE = 64 * 32;
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* screen;
	uint8_t pixels[SCREEN_SIZE];
public:
	Display();
	~Display();
	void init();
	void quit();
	void clear();
	uint8_t& operator[](int i);
	uint8_t operator [](int i) const; 
	void render();
	void handleEvents(bool& running);
};

#endif
