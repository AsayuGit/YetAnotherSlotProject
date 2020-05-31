#ifndef INCLUDE
#define INCLUDE

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "types.h"

#include "GLOBAL_Functions.h"
#include "TEXT_Functions.h"
#include "SDL_Functions.h"

#define LINUX // Target os

#define SLOTMACHINE_NAME "YetAnotherSlotProject"

#define NB_OF_THEMES 1

#define LINES 77
#define COLUMNS 263

#define NBL 7
#define WIN 13

// Pour la futur portabilité
#ifdef LINUX
#define CLEAR "clear"
#elif WINDOWS
#define CLEAR "cls"
#endif

#define FontPath "Fonts/"
#define ImagePath "Images/"
#define SoundPath "Sounds/"


#endif