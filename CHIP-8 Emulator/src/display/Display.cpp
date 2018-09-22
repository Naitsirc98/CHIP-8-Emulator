#include "Display.h"
#include <iostream>

Display::Display()
{
	for(int i = 0; i < SCREEN_SIZE; i++)
	{
		pixels[i] = 0;
	}
}


Display::~Display()
{
	quit();
}

void Display::init()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// ...
	}

	window = SDL_CreateWindow("CHIP-8 Emulator",500,400,640,320, NULL);

	renderer = SDL_CreateRenderer(window, 0, NULL);

	screen = SDL_GetWindowSurface(window);

}

void Display::quit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Display::clear()
{
	SDL_RenderClear(renderer);
}

uint8_t& Display::operator[](int i)
{
	return pixels[i];
}

uint8_t Display::operator[](int i) const
{

	std::cout << i << std::endl;
	return pixels[i];
}

void Display::render()
{

	SDL_Rect rect;
	int scale = 10;

	//Loop through and draw each pixel
	for(int x = 0; x < 64*10; x++) {
		for(int y = 0; y < 32*10; y++) {
			rect.x = x * scale;
			rect.y = y * scale;
			rect.w = scale;
			rect.h = scale;

			if(pixels[x + (y * 64)] == 0) {
				//Draw black pixel
				SDL_FillRect(screen, &rect,
					SDL_MapRGB(screen->format, 0, 0, 0));
			}
			else {
				//Draw white pixel
				SDL_FillRect(screen, &rect,
					SDL_MapRGB(screen->format, 255, 255, 255));
			}
		}
	}

	SDL_UpdateWindowSurface(window);

}

void Display::handleEvents(bool& running)
{
	SDL_Event e;

	while(SDL_PollEvent(&e) != 0)
	{
		if(e.type == SDL_QUIT)
		{
			running = false;
		}
		else if(e.type == SDL_KEYDOWN)
		{

		}
		else if(e.type == SDL_KEYUP)
		{

		}
	}

}
