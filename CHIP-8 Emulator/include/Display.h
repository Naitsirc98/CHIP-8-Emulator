#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdint.h>
#include <SDL.h>
#include <string>


#define SCALE 16
#define WIDTH 64
#define HEIGHT 32
#define BUFFER_SIZE WIDTH*HEIGHT 
#define DELTA_TIME 1


class Display
{
private:
	SDL_Window* window;
	SDL_Surface* screen;
	int nextFrame;
	uint8_t pixels[BUFFER_SIZE];
public:
	Display();
	~Display();
	void init(const char* title);
	void quit();
	void clear();
	uint8_t& operator[](int i);
	uint8_t operator [](int i) const; 
	void render();
	void handleEvents(bool& running);
};

#endif
