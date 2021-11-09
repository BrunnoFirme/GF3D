#include <SDL.h> 
#include "time.h"

void UpdateTime()
{
    ticksPast = (SDL_GetTicks() - totalTime);
    totalTime = SDL_GetTicks();
    deltaTime = ((double)ticksPast * .001);
}