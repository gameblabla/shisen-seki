#include "video.h"
#include "graphics.h"

#include <SDL/SDL.h>

SDL_Surface *screen;
SDL_Surface *screenScaled;
int scale;
Uint32 curTicks;
Uint32 lastTicks = 0;

int initSDL()
{
	if(SDL_Init(SDL_INIT_VIDEO))
	{
		return -1;
	}

	SDL_WM_SetCaption("Shisen-Seki", NULL);
	SDL_ShowCursor(SDL_DISABLE);

	updateScale();

	if(screen == NULL)
	{
		return -1;
	}

	return 0;
}

void deinitSDL()
{
	if (screen && scale > 1)
	{
		SDL_FreeSurface(screen);
	}

	SDL_Quit();
}

void updateScale()
{
	if (screen && screen != screenScaled)
	{
		SDL_FreeSurface(screen);
	}

	screenScaled = SDL_SetVideoMode(SCREEN_W * scale, SCREEN_H * scale, SCREEN_BPP, SDL_SWSURFACE);
	screen = scale > 1 ? SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_W, SCREEN_H, SCREEN_BPP, 0, 0, 0, 0) : screenScaled;
}

SDL_Surface *loadImage(char *fileName)
{
	SDL_RWops *rw;
	SDL_Surface *loadedImage;
	SDL_Surface *optimizedImage;
	Uint32 colorKey;

	if (!fileName)
	{
		fprintf(stderr, "ERROR: Filename is empty.");
		return NULL;
	}
	
	if (strcmp (fileName, "data/gfx/background.bmp") == 0) 			rw = SDL_RWFromMem(IMG_BACKGROUND, BACKGROUND_SIZE);
	else if (strcmp (fileName, "data/gfx/stones.bmp") == 0) 			rw = SDL_RWFromMem(IMG_STONES, STONES_SIZE);
	else if (strcmp (fileName, "data/gfx/fontBlack.bmp") == 0) 		rw = SDL_RWFromMem(IMG_FONTBLACK, FONTBLACK_SIZE);
	else if (strcmp (fileName, "data/gfx/fontGray.bmp") == 0) 		rw = SDL_RWFromMem(IMG_FONTGRAY, FONTGRAY_SIZE);
	else if (strcmp (fileName, "data/gfx/fontLightOrange.bmp") == 0) rw = SDL_RWFromMem(IMG_FONTLIGHTORANGE, FONTLIGHTORANGE_SIZE);
	else if (strcmp (fileName, "data/gfx/fontOrange.bmp") == 0) 		rw = SDL_RWFromMem(IMG_FONTORANGE, FONTORANGE_SIZE);
	else if (strcmp (fileName, "data/gfx/fontRed.bmp") == 0) 		rw = SDL_RWFromMem(IMG_FONTRED, FONTRED_SIZE);
	else if (strcmp (fileName, "data/gfx/fontWhite.bmp") == 0)		rw = SDL_RWFromMem(IMG_FONTWHITE, FONTWHITE_SIZE);
	
	loadedImage = SDL_LoadBMP_RW(rw, 0);

	if (!loadedImage)
	{
		fprintf(stderr, "ERROR: Failed to load image: %s\n", fileName);
		return NULL;
	}

	optimizedImage = SDL_CreateRGBSurface(SDL_SWSURFACE, loadedImage->w, loadedImage->h, SCREEN_BPP, 0, 0, 0, 0);
	SDL_BlitSurface(loadedImage, NULL, optimizedImage, NULL);
	SDL_FreeSurface(loadedImage);

	if (!optimizedImage)
	{
		fprintf(stderr, "ERROR: Failed to optimize image: %s\n", fileName);
		return NULL;
	}

	colorKey = SDL_MapRGB(optimizedImage->format, 255, 0, 255); // Set transparency to magenta.
	SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorKey);
	
	if (rw) SDL_FreeRW(rw);

	return optimizedImage;
}

void clipImage(SDL_Rect *source, int tileWidth, int tileHeight, int rowLength, int numOfTiles)
{
	int i;
	int j;
	int k;
	int l;

	for(i = 0, k = 0; k < numOfTiles; i+= tileHeight)
	{
		for(j = 0, l = 0; l < rowLength; j+= tileWidth)
		{
			if (k >= numOfTiles)
			{
				return;
			}
			source[k].x = j;
			source[k].y = i;
			source[k].w = tileWidth;
			source[k].h = tileHeight;
			++k;
			++l;
		}
		l = 0;
	}
}

void drawImage(SDL_Surface *source, SDL_Rect *clip, SDL_Surface *destination, int x, int y)
{
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, clip, destination, &offset);
}

void drawRectangle(SDL_Surface *source, int x, int y, int w, int h, int color)
{
	SDL_Rect r;

	// Top wall.
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = 1;
	SDL_FillRect(source, &r, color);

	// Bottom wall.
	r.x = x;
	r.y = y + h - 1;
	r.w = w;
	r.h = 1;
	SDL_FillRect(source, &r, color);

	// Left wall.
	r.x = x;
	r.y = y;
	r.w = 1;
	r.h = h;
	SDL_FillRect(source, &r, color);

	// Right wall.
	r.x = x + w - 1;
	r.y = y;
	r.w = 1;
	r.h = h;
	SDL_FillRect(source, &r, color);
}

int frameLimiter()
{
	int t;

#if defined(NO_FRAMELIMIT)
	return 0;
#endif

	curTicks = SDL_GetTicks();
	t = curTicks - lastTicks;

	if(t >= 1000/FPS)
	{
		lastTicks = curTicks;
		return 0;
	}

	SDL_Delay(1);
	return 1;
}

void clearScreen()
{
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
}
