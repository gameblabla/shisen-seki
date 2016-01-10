#include "input.h"

#include <SDL/SDL.h>
#include "main.h"

SDL_Event event;
int keys[2048];
int mouse[2];
int mouseMoved;
int enableJoystick = 1;
int joyCanMoveX = 1;
int joyCanMoveY = 1;

void input()
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				quit = 1;
			break;
			case SDL_KEYDOWN:			// Button press
				keys[event.key.keysym.sym] = 1;
			break;
			case SDL_KEYUP:				// Button release
				keys[event.key.keysym.sym] = 0;
			break;
			default:
			break;
		}
	}

	updateMouse();
}

void updateMouse()
{
	int x;
	int y;

	SDL_GetMouseState(&x, &y);

	if (x != mouse[0] || y != mouse[1])
	{
		mouseMoved = 1;
		mouse[0] = x;
		mouse[1] = y;

		SDL_ShowCursor(SDL_ENABLE);
	}
	else
	{
		mouseMoved = 0;
	}
}
