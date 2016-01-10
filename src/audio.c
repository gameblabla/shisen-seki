#include "audio.h"

int hasAudio;
int enableMusic = 1;
int enableSfx = 1;
Mix_Music *bgdMusic = NULL;
Mix_Chunk *clearSfx = NULL;

int initAudio()
{

	return 0;
}

void deinitAudio()
{
}

Mix_Music *loadMusic(Mix_Music *track, char *fileName)
{
	return 0;
}

void unloadMusic(Mix_Music **track)
{
}

void playMusic(Mix_Music *track)
{
}

void resumeMusic()
{
}

void pauseMusic()
{
}

Mix_Chunk *loadSfx(Mix_Chunk *effect, char *fileName)
{
	return 0;
}

void unloadSfx(Mix_Chunk **effect)
{
}

void playSfx(Mix_Chunk *effect)
{
}
