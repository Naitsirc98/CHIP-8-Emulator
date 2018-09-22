#include "Display.h"
#include "Keyboard.h"
#include <iostream>

Display::Display()
{
	memset(pixels, 0, BUFFER_SIZE);
}


Display::~Display()
{
	quit();
}

void Display::init(const char* title)
{

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0)
	{
		std::cout << "ERROR initializing SDL " << SDL_GetError() << std::endl;
	}

	window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIDTH * SCALE, HEIGHT * SCALE, NULL);

	screen = SDL_GetWindowSurface(window);

	nextFrame = SDL_GetTicks() + DELTA_TIME;

	clear();

}

void Display::quit()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Display::clear()
{
	memset(pixels, 0, WIDTH * HEIGHT);
	SDL_UpdateWindowSurface(window);
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

	if(!SDL_TICKS_PASSED(SDL_GetTicks(), nextFrame)) 
	{
		return;
	}

	nextFrame = SDL_GetTicks() + DELTA_TIME;

	SDL_Rect rect;

	for(int x = 0; x < WIDTH*SCALE; x++)
	{
		for(int y = 0; y < HEIGHT*SCALE; y++)
		{

			rect.x = x * SCALE;
			rect.y = y * SCALE;
			rect.w = SCALE;
			rect.h = SCALE;

			if(pixels[x + (y * WIDTH)] == 0) 
			{
				
				SDL_FillRect(screen, &rect,
					SDL_MapRGB(screen->format, 0, 0, 0));
			}
			else
			{
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
			Keyboard::getKeyboard().setKeyPressed(e.key.keysym.sym);
		}
		else if(e.type == SDL_KEYUP)
		{
			Keyboard::getKeyboard().setKeyReleased(e.key.keysym.sym);
		}
	}

}
